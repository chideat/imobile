
#include "itdb.h"
#include "ringtone-write.h"
#include "ringtone-parse.h"


void init_header(char *buf){
    Itdb_Mhod *mhod = (Itdh_Mhod *)buf;
    memcpy(mhod->header, "mhod", 4);
    mhod->header_len = 0x24;
}


void update(Itdb_Ringtone *itdb, char *mhit) {
    if (! itdb || !mhit)
        return NULL;
    It_Content *cts = itdb->cts;
    char *src = ctc->contents;
    uint32 len_1 = *(uint32 *)(src + 8);
    uint32 len_2 = *(uint32 *)(mhit + 8);
    
    char *buf = new char[len_1 + len_2];
    // index of last mhit
    char *mhsd_c = itdb_mhsd(src);
    char *mhlt_c = itdb_mhlt(mhsd_c);
    char *mhit_c = itdb_mhit(mhlt_c);
    char *point = mhit_c + *(uint32 *)(mhit_c + 8);
    uint32 len = point - src;
    memcpy(buf, src, len);
    memcpy(buf + len, mhit_c, len_2);
    memcpy(buf + len + len_2, point, len_1 - len);
    delete mhit_c;
    delete src;
    
    mhsd = itdb_mhsd(buf);
    mhlt = itdb_mhlt(mhsd);
    // update info of mhlt
    uint32 *mhlt_count = (uint32 *)(mhlt + 8);
    mhlt_count ++;
 
    // update info of mhsd
    uint32 *mhsd_total = (uint32 *)(mhsd + 8);
    mhsd_total += len_2;

    // update info of mhbd
    // header_len
    // total_len
    // hash
    Itdb_Mhdb *mhbd = (Itdb_Mhbd *)buf;
    cts->length = mhbd->header_len + len_2;

    // compress
    if (mhbd->compressed == 2) {
        // do compress
        itdb_zlib_compress(itdb);
    }


    cts->contents = buf;
}




/**
 * write mhbd
 */
char *update_mhbd(char *cts) {
}


/**
 * write mhsd
 *     update mhsd
 */
char *update_mhsd(char *cts) {
    
}


/**
 * write mhlt
 * append the mhit to mhlt
 */
char *update_mhlt(char *src, char *mhit) {
    

    
    


}

/**
 * deal with
 *     header len
 *     total len
 *     ......
 * the lenght of it may at most 1024
 */
char *mk_mhit(const Ringtone *ringtone) {
    It_Content *cts = new It_Content;
    cts->contents = new char[MAX_BUF];
    cts->length = 0;
    memset(cts->contents, 0, sizeof(cts->contents);
    char *point =  cts->contents;

    Itdb_Mhit *mhit = (Itdb_Mhit *)point;
    memcpy(mhit->header, "mhit", 4);
    mhit->header_len = 584; //sizeof(Itdb_Mhit_Header);
    mhit->total_len = mhit->header_len;
    
    // byte rebuild
    mhit->id = _id();
    mhit->visible = 1;
    mhit->filetype_marker = 0x4d345220;
    mhit->type2 = 1;
    mhit->time_modified = get_mac_time();
    /**************/
    mhit->size = 0;
    mhit->track_len = 0; // ??
    mhit->bitrate = 0; // ??

    mhit->time_added = mhit->time_modified;
    mhit->dbid = 0; // ??
    mhit->unk126 = 0xffff;
    mhit->unk144 = 0x0c;
    mhit->has_artwork = 2;
    mhit->mark_unplayed = 2; // 178
    mhit->unk179 = 1;
    mhit->mediatype = 0x4000;
    mhit->gapless_track_flag = 0x01; // 256
    memcpy(mhit->unk288_id, mhit->id, 4);  // 288
    memset(mhit->unk308, 0x80, 6);
    
    mhit->unk344 = 0; // ????? 8byte
    mhit->unk360 = 0x01;
    memcpy(mhit->unk480_id, mhit->id, 4);
    // memset(mhit->unk500, 0, 4);
    
    int count = 0;
    point = mhit + mhit->header_len;
    write_mode_type_2(point, ringtone);
    mhit->total_len += *(uint32 *)(point + 8);
    count ++;

    point = point + *(uint32 *)(point + 8);
    write_mode_type_1(point, ringtone);
    mhit->total_len += *(uint32 *)(point + 8);
    count ++;

    point = point + *(uint32 *)(point + 8);
    write_mode_type_3(point);
    mhit->total_len += *(uint32 *)(point + 8);
    count ++;
    // update length, children
    mhit->num_children = count;
    return cts;
}



/**
 * deal with
 *     header length
 *     total length
 *     encode
 * name to display without suffix
 */
void write_mode_type_1(char *cts, Ringtone *ringtone) {
    uint32 size;
    char **buf;
    convert_utf8_to_utf16(ringtone->name, strlen(ringtone->name), buf, &size);
    init_header(cts);
    Itdb_Mhod *mhod = (Itdb_Mhod *)cts;
    memcpy(cts, *buf, size);
    delete *buf;

    // set length
    mhod->length = size;
    mhod->unk_0xa8 = 0x1;  // encoding: 0 UTF-8, 1 UTF-16 
    mhod->type = 0x1;
    mhod->total_len = mhod->header_len + 4 + 4 + 8 + size;
}


/**
 * deal with
 *     header length
 *     total length
 *     encode
 * iTunes_Control:Ringtones:FLOW.m4r
 */
Itdb_Mhod * write_mode_type_2(char *cts, Ringtone *ringtone) {
    char *data_n = new char[strlen(ringtone->index) + 25];
    char ringtone[] = "iTunes_Control:Ringtones:";
    memcpy(data_n, ringtone, strlen(ringtone));
    memcpy(data_n + strlen(ringtone), ringtone->index, strlen(ringtone->index));
    unsigned int len = strlen(data_n);
    
    char **buf;
    uint32 size;
    convert_utf8_to_utf16(data_n, len, buf, &size);
    init_header(cts);
    Itdb_Mhod *mhod = (Itdb_Mhod *)cts;
    memcpy(mhod->data, *buf, size);
    delete *buf;

    // set length
    mhod->length = size;
    mhod->unk_0xa8 = 0x1;  // encoding: 0 UTF-8, 1 UTF-16 
    mhod->type = 0x2;
    mhod->total_len = mhod->header_len + 4 + 4 + 8 + size;
}

/**
 * deal with
 *     header length
 *     total length
 *     encode
 * Ringtone
 */
void write_mode_type_6(char *cts) {
    char data[] = "Ringtone";
    uint32 len = 0;
    char **buf;
    uint32 size;
    convert_utf8_to_utf16(data, sizeof(data), buf, &len);
    init_header(cts);
    Itdb_Mhod *mhod = (Itdb_Mhod *)cts;
    memcpy(mhod->data, *buf, size);
    delete *buf;

    // set length
    mhod->length = size;
    mhod->unk_0xa8 = 0x1;  // encoding: 0 UTF-8, 1 UTF-16 
    mhod->type = 0x6;
    mhod->total_len = mhod->header_len + 4 + 4 + 8 + size;
}




/**
 * deal with
 *     header length
 *     total length
 *     encode
 * Ringtone
 */
Itdb_Mhod * write_mode_type_3(char *data, uint32 len) {
}

/**
 * deal with
 *     header length
 *     total length
 *     encode
 * Ringtone
 */
Itdb_Mhod * write_mode_type_4(char *data, uint32 len) {
}

/**
 * deal with
 *     header length
 *     total length
 *     encode
 * Ringtone
 */
Itdb_Mhod * write_mode_type_0xa6(char *data, uint32 len) {
}

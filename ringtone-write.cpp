
#include "itdb.h"
#include "ringtone-write.h"


char *init_header(char header[4]){
    char *buf = new char[MAX_BUF];
    memset(buf, 0, sizeof(buf));

    Itdb_Mhod *mhod = (Itdh_Mhod *)buf;
    memcpy(mhod->header, header, 4);
    mhod->header_len = sizeof(Itdb_Mhod_Header);

    return buf;
}

/**
 * deal with
 *     header len
 *     total len
 *     ......
 * the lenght of it may at most 1024
 */
It_Content *write_mhit(uint32 children) {
    It_Content *cts = new It_Content;
    cts->contents = new char[MAX_BUF];
    cts->length = 0;
    memset(cts->contents, 0, sizeof(cts->contents);
    char *point =  cts->contents;

    Itdb_Mhit *mhit = (Itdb_Mhit *)point;
    memcpy(mhit->header, "mhit", 4);
    mhit->header_len = sizeof(Itdb_Mhit_Header);
    
    // byte rebuild

    // add some info here
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









    
    


    return cts;
}



/**
 * deal with
 *     header length
 *     total length
 *     encode
 * name
 */
Itdb_Mhod *write_mode_type_1(char *data, uint32 len) {
    uint32 size;
    char **buf;
    convert_utf8_to_utf16(data, len, buf, &size);
    Itdb_Mhod *mhod = (Itdb_Mhod *) init_header();
    memcpy(mhod->data, *buf, size);
    delete *buf;

    // set length
    mhod->length = size;
    mhod->unk_0xa8 = 0x1;  // encoding: 0 UTF-8, 1 UTF-16 
    mhod->type = 0x1;
    mhod->total_len = mhod->header_len + 4 + 4 + 8 + size;
    return mhod;
}


/**
 * deal with
 *     header length
 *     total length
 *     encode
 * iTunes_Control:Ringtones:FLOW.m4r
 */
Itdb_Mhod * write_mode_type_2(char *data, uint32 len) {
    char *data_n = new char[len + 25];
    char ringtone[] = "iTunes_Control:Ringtones:";
    memcpy(data_n, ringtone, strlen(ringtone));
    memcpy(data_n + strlen(ringtone), data, len);
    len = strlen(ringtone) + len;
    
    char **buf;
    uint32 size;
    convert_utf8_to_utf16(data_n, len, buf, &size);

    Itdb_Mhod *mhod = (Itdb_Mhod *) init_header();
    memcpy(mhod->data, *buf, size);


    delete *buf;
    // set length
    mhod->length = size;
    mhod->unk_0xa8 = 0x1;  // encoding: 0 UTF-8, 1 UTF-16 
    mhod->type = 0x2;
    mhod->total_len = mhod->header_len + 4 + 4 + 8 + size;
    return mhod;
}

/**
 * deal with
 *     header length
 *     total length
 *     encode
 * Ringtone
 */
Itdb_Mhod * write_mode_type_6(char *data, uint32 len) {
    char data[] = "Ringtone";
    char **buf;
    uint32 size;
    convert_utf8_to_utf16(data, sizeof(data), buf, &len);

    Itdb_Mhod *mhod = (Itdb_Mhod *) init_header();
    memcpy(mhod->data, *buf, size);

    delete *buf;
    // set length
    mhod->length = size;
    mhod->unk_0xa8 = 0x1;  // encoding: 0 UTF-8, 1 UTF-16 
    mhod->type = 0x6;
    mhod->total_len = mhod->header_len + 4 + 4 + 8 + size;
    return mhod;
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

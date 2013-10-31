#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include "itdb.h"
#include "itdb_tz.h"
#include "itdb_zlib.h"
#include "util.h"

#define iT_DB "./sample/iTunesCDB"


void itdb_mhods(char *mhit, int index = 1);

char *itdb_mhbd(char *cts, long seek = 0) {
    Itdb_Mhbd *mhbd = new Itdb_Mhbd;
    mhbd->header_id = cts;
    mhbd->header_len = get32uint(cts, seek + 4);
    mhbd->total_len = get32uint(cts, seek + 8);
    mhbd->unknown1 = get32uint(cts, seek + 12);
    mhbd->version = get32uint(cts, seek + 16);
    mhbd->num_children = get32uint(cts, seek + 20);
    mhbd->db_id = get64uint(cts, seek + 24);
    mhbd->platform = get16uint(cts, seek + 32);
    mhbd->unk_0x22 = get16uint(cts, seek + 34);
    mhbd->id_0x24 = get64uint(cts, seek + 36);
    mhbd->unk_0x2c = get32uint(cts, seek + 44);
    mhbd->hashing_scheme = get16uint(cts, seek + 48);
    mhbd->unk_0x32 = get8uint(cts, seek + 50); // 20
    mhbd->language_id = get8int(cts, seek + 70);
    mhbd->db_persistent_id = get64uint(cts, seek + 72);
    mhbd->unk_0x50 = get32uint(cts, seek + 80);
    mhbd->unk_0x54 = get32uint(cts, seek + 0x54);
    mhbd->hash58 = getuchar(cts, seek + 88);
    mhbd->timezone_offset = get32int(cts, seek + 108);
    mhbd->unk_0x70 = get16uint(cts, seek + 0x70);
    mhbd->hash72 = getuchar(cts, seek + 114);
    mhbd->audio_language = get16uint(cts, seek + 160);
    mhbd->subtitle_language = get16uint(cts, seek + 162);
    mhbd->unk_0xa4 = get16uint(cts, seek + 0xa4);
    mhbd->unk_0xa6 = get16uint(cts, seek + 0xa6);
    mhbd->unk_0xa8 = get16uint(cts, seek + 0xa8);
    mhbd->align_0xa9 = getuchar(cts, seek + 0xa9);
    mhbd->hashAB = getuchar(cts, seek + 0xaa);
    mhbd->padding = getuchar(cts, seek + 0xaa + 57);


    printf("mhbd header_len: %u\n", *mhbd->header_len);
    printf("mhbd total_len: %u\n", *mhbd->total_len);
    printf("mhbd version: %u\n", *mhbd->version);
    printf("mhbd num_children: %u\n", *mhbd->num_children);
    printf("mhbd db_id: %llu\n", *mhbd->db_id);
    printf("mhbd paltform: %u\n", *mhbd->platform);
    printf("mhbd hashing_scheme: %llu\n", *mhbd->hashing_scheme);
    printf("mhbd language_id: %c%c\n", mhbd->language_id[0], mhbd->language_id[1]);
    printf("mhbd timezone offset: %llu\n", mhbd->timezone_offset);
    // printf("mhbd hash58: ");
    // for (int i = 0; i < 20; i ++) {
    //     putchar(mhbd->hash58[i]);
    // }
    // putchar(10);
    // printf("mhbd hash72: ");
    // for (int i = 0; i < 46; i ++) {
    //     putchar(mhbd->hash72[i]);
    // }
    // putchar(10);
    // printf("mhbd hashAB: ");
    // for (int i = 0; i < 57; i ++) {
    //     putchar(mhbd->hashAB[i]);
    // }
    // putchar(10);
}


char *itdb_mhsd(Itdb_Ringtone *itdb, int index = 1) {
    char *cts = itdb->cts->contents;
    uint32 header_len = *(uint32 *)(cts + 4);
    uint32 total_len = *(uint32 *)(cts + 8);
    char *mhsd = cts + header_len;
    int child_count = 0;
    while (mhsd < cts + total_len && child_count < *(uint32 *)(cts + 20)) {
        child_count ++;
        if (mhsd[0] == 'm' && mhsd[1] == 'h' && mhsd[2] == 's' && mhsd[3] == 'd') {
            // ringtone type
            if (*(uint32 *)(mhsd + 12) == 1)
                break;
            mhsd = mhsd + *(uint32 *)(mhsd + 8);
        }
    }
    return mhsd;
}


char *itdb_mhlt(char *mhsd, int index = 1) {
    char *mhlt = mhsd + *(uint32 *)(mhsd + 4);
    return mhlt;
}

static long get_mhit(char *cts, long seek = 0) {
    Itdb_Mhit *mhit = new Itdb_Mhit;
    uint32 header_len = *get32uint(cts, seek + 4);
    if (header_len > 0x9c) {
        mhit->id = get32uint(cts, seek + 16);
        mhit->visible = get32uint(cts, seek + 20);
        mhit->filetype_marker = get32uint(cts, seek + 24);
        mhit->type1 = get8uint(cts, seek + 28);
        mhit->type2 = get8uint(cts, seek + 29);
        mhit->compilation = get8uint(cts, seek + 30);
        mhit->rating = get8uint(cts, seek + 31);
        mhit->time_modified = get32uint(cts, seek + 32);  // time added
        mhit->size = get32int(cts, seek + 36);
        mhit->tracklen = get32int(cts, seek + 40);
        mhit->track_nr = get32int(cts, seek + 44);
        mhit->tracks = get32int(cts, seek + 48);
        mhit->year = get32int(cts, seek + 52);
        mhit->bitrate = get32int(cts, seek + 56);
        // val32 = get32uint(cts, seek + 60);
        // mhit->samplerate = val32 >> 16;
        // mhit->samplerate_low = val32 & 0xffff;
        mhit->volume = get32int(cts, seek + 64);
        mhit->starttime = get32uint(cts, seek + 68);
        mhit->stoptime = get32uint(cts, seek + 72);
        mhit->soundcheck = get32uint(cts, seek + 76);
        mhit->playcount = get32uint(cts, seek + 80);
        mhit->playcount2 = get32uint(cts, seek + 84);
        mhit->time_played = get32uint(cts, seek + 88);
        mhit->cd_nr = get32int(cts, seek + 92);
        mhit->cds = get32int(cts, seek + 96);
        mhit->drm_userid = get32uint(cts, seek + 100);
        mhit->time_added = get32uint(cts, seek + 104);
        mhit->bookmark_time = get32uint(cts, seek + 108);
        mhit->dbid = get64uint(cts, seek + 112);
        mhit->checked = get8uint(cts, seek + 120);
        mhit->app_rating = get8uint(cts, seek + 121);
        mhit->BPM = get16int(cts, seek + 122);
        mhit->artwork_count = get16uint(cts, seek + 124);
        mhit->unk126 = get16uint(cts, seek + 126);
        mhit->artwork_size = get32uint(cts, seek + 128);
        mhit->unk132 = get32uint(cts, seek + 132);
        mhit->samplerate2 = get32float(cts, seek + 136);
        mhit->time_released = get32uint(cts, seek + 140);
        mhit->unk144 = get16uint(cts, seek + 144);
        mhit->explicit_flag = get16uint(cts, seek + 146);
        mhit->unk148 = get32uint(cts, seek + 148);
        mhit->unk152 = get32uint(cts, seek + 152);
    }
    if (header_len >= 0xf4) {
        mhit->skipcount = get32uint(cts, seek + 156);
        mhit->last_skipped = get32uint(cts, seek + 160);
        mhit->has_artwork = get8uint(cts, seek + 164);
        mhit->skip_when_shuffling = get8uint(cts, seek + 165);
        mhit->skip_when_shuffling = get8uint (cts, seek+165);
        mhit->remember_playback_position = get8uint (cts, seek+166);
        mhit->flag4 = get8uint (cts, seek+167);
        mhit->dbid2 = get64uint (cts, seek+168);
        mhit->lyrics_flag = get8uint (cts, seek+176);
        mhit->movie_flag = get8uint (cts, seek+177);
        mhit->mark_unplayed = get8uint (cts, seek+178);
        mhit->unk179 = get8uint (cts, seek+179);
        mhit->unk180 = get32uint (cts, seek+180);
        mhit->pregap = get32uint (cts, seek+184);
        mhit->samplecount = get64uint (cts, seek+188);
        mhit->unk196 = get32uint (cts, seek+196);
        mhit->postgap = get32uint (cts, seek+200);
        mhit->unk204 = get32uint (cts, seek+204);
        mhit->mediatype = get32uint (cts, seek+208);
        mhit->season_nr = get32uint (cts, seek+212);
        mhit->episode_nr = get32uint (cts, seek+216);
        mhit->unk220 = get32uint (cts, seek+220);
        mhit->unk224 = get32uint (cts, seek+224);
        mhit->unk228 = get32uint (cts, seek+228);
        mhit->unk232 = get32uint (cts, seek+232);
        mhit->unk236 = get32uint (cts, seek+236);
        mhit->unk240 = get32uint (cts, seek+240);
    }
    if (header_len >= 0x148) {
        mhit->unk244 = get32uint (cts, seek+244);
        mhit->gapless_data = get32uint (cts, seek+248);
        mhit->unk252 = get32uint (cts, seek+252);
        mhit->gapless_track_flag = get16uint (cts, seek+256);
        mhit->gapless_album_flag = get16uint (cts, seek+258);
    }
    if (header_len >= 584) {
        mhit->unk288_id = get32uint(cts, seek + 288);
        mhit->unk480_id = get32uint(cts, seek + 480);
        mhit->unk500_0 = get32uint(cts, seek + 500);
    }
    
    printf("id: %u\n", *mhit->id);
    printf("type1: %d\n", *mhit->type1);
    printf("type2: %d\n", *mhit->type2);
    printf("rating: %d\n", *mhit->rating);
    printf("time_modified: %u\n", *mhit->time_modified - 2082844800);
    printf("filesize: %d\n", *mhit->size);


    printf("unk 288: %u,  %X\n", *mhit->unk288_id, *mhit->unk288_id);
    printf("unk 480: %u,  %X\n", *mhit->unk480_id, *mhit->unk480_id);
    printf("unk 500: %u,  %X\n", *mhit->unk500_0, *mhit->unk500_0);
}

char *itdb_mhit(char *mhlt, int index = 1) {
    char *mhit = mhlt + *(uint32 *)(mhlt + 4);
    uint32 header_len = *(uint32 *)(mhlt + 4);
    uint32 count = *(uint32 *)(mhlt + 8);
    uint32 mhit_total_len;
    for (int i = 0; i < count; i ++) {
        if (mhit[0] == 'm' && mhit[1] == 'h' && mhit[2] == 'i' && mhit[3] == 't') {
            get_mhit(mhit, 0);
            itdb_mhods(mhit);
            mhit_total_len = *(uint32 *)(mhit + 8);
            mhit = mhit + mhit_total_len;
        }
        else 
            fprintf(stderr, "escape\n");
        printf("===============================\n");
    }
    return mhit;
}

void itdb_mhods(char *mhit, int index) {
    uint32 mhit_header_len = *(uint32 *)(mhit + 4);
    uint32 count = *(uint32 *)(mhit + 12);
    fprintf(stderr, "mhit header len: %u\n", mhit_header_len);
    fprintf(stderr, "mhit count : %u\n", count);
    char *mhod = mhit + *(uint32 *)(mhit + 4);
    uint32 mhod_total_len;
    for (int i = 0; i < count; i ++) {
        if (mhod > mhit + *(uint32 *)(mhit + 8)) {
            mhod = NULL;
            break;
        }
        if (mhod[0] == 'm' && mhod[1] == 'h' && mhod[2] == 'o' && mhod[3] == 'd') {
            mhod_total_len = *(uint32 *)(mhod + 8);
            printf("mhod header len: %u, %u\n", *(uint32 *)(mhod + 4), *(uint32 *)(mhod + 8));
            // for (int j = 0; j < mhod_total_len; j++) {
            //     printf("%c", mhod[j]);
            // }
            mhod = mhod + mhod_total_len;
        }
    }
}


int main(int argc, char **argv) {
    char name[128] = "sample/iTunesCDB";
    if (argc == 2) {
        sprintf(name, "%s%s", name, argv[1]);
    }
    FILE *file = fopen(name, "r");
    
    Itdb_Ringtone *itdb = new Itdb_Ringtone;
    itdb->cts = new It_Content;
    int size = fseek(file, 0, SEEK_END);
    if (size == 0) {
        size = ftell(file);
        itdb->cts->contents = new char[size];
    }
    else {
        fprintf(stderr, "file seek failed\n");
        return 0;
    }
    if (fseek(file, 0, SEEK_SET) != 0){
        fprintf(stderr, "file seek failed\n");
        return 0;
    }
    itdb->cts->length = fread(itdb->cts->contents, sizeof(char), size, file);
    itdb_zlib_decompress(itdb);
    // fprintf(stderr, "mhbd header size:%u\n", *(uint32 *)(itdb->cts->contents + 4));

    // for (int i = 0; i < itdb->cts->length; i++) {
    //     putchar(itdb->cts->contents[i]);
    // }
    // return 0;


    // itdb_zlib_compress(itdb);


    itdb_mhbd(itdb->cts->contents);
    
    char *cts = itdb_mhsd(itdb, 3);
    char *mhlt = itdb_mhlt(cts);
    char *mhit = itdb_mhit(mhlt, 5);

    fclose(file);
    delete itdb->cts->contents;
    delete itdb->cts;
    delete itdb;
    return 0;
}

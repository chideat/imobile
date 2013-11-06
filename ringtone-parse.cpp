#include <string.h>
#include <stdio.h>
#include "ringtone-parse.h"

Itdb_Mhbd *parse_mhbd(char *cts, long seek) {
    Itdb_Mhbd *mhbd = (Itdb_Mhbd *)cts;

    printf("mhbd header_len: %u\n", mhbd->header_len);
    printf("mhbd total_len: %u\n", mhbd->total_len);
    printf("mhbd version: %u\n", mhbd->version);
    printf("mhbd num_children: %u\n", mhbd->num_children);
    printf("mhbd db_id: %llu\n", mhbd->db_id);
    printf("mhbd paltform: %u\n", mhbd->platform);
    printf("mhbd hashing_scheme: %llu\n", mhbd->hashing_scheme);
    printf("mhbd language_id: %c%c\n", mhbd->language_id[0], mhbd->language_id[1]);
    printf("mhbd timezone offset: %llu\n", mhbd->timezone_offset);
    
    return mhbd;
}

char *itdb_mhsd(char *mhbd, int index) {
    uint32 total_len = *(uint32 *)(mhbd + 8);
    char *mhsd = mhbd + *(uint32 *)(mhbd + 4);
    uint32 child_count = 0;
    while (mhsd < mhbd + total_len && child_count < *(uint32 *)(mhbd + 20)) {
        child_count ++;
        if (mhsd[0] == 'm' && mhsd[1] == 'h' && mhsd[2] == 's' && mhsd[3] == 'd') {
            // ringtone type
            if (*(uint32 *)(mhsd + 12) == 0x01)
                return mhsd;
            mhsd = mhsd + *(uint32 *)(mhsd + 8);
        }
    }
    return NULL;
}


char *itdb_mhlt(char *mhsd, int index) {
    char *mhlt = mhsd + *(uint32 *)(mhsd + 4);
    return mhlt;
}

long parse_mhit(char *cts, long seek) {
    Itdb_Mhit *mhit = (Itdb_Mhit *)cts;
    
    printf("id: %u\n", mhit->id);
    printf("type1: %d\n", mhit->type1);
    printf("type2: %d\n", mhit->type2);
    printf("rating: %d\n", mhit->rating);
    printf("time_modified: %u\n", mhit->time_modified - 2082844800);
    printf("time_added: %u\n", mhit->time_added - 2082844800);
    printf("filesize: %d\n", mhit->size);
    printf("filetype_maker: %u\n", mhit->filetype_marker);
    printf("trackLen: %u\n", mhit->tracklen);
    printf("media type: %u, unk204: %u\n", mhit->mediatype, mhit->unk204);
    printf("mark unplayed: %u\n", mhit->mark_unplayed);
    printf("mark unplayed unk: %u\n", mhit->unk179);
    printf("unk144: %u\n", mhit->unk144);
    printf("164 has_artwork: %u\n", mhit->has_artwork);

    printf("unk 288: %u,  %X\n", mhit->unk288_id, mhit->unk288_id);
    printf("unk 308: %X\n", mhit->unk308);
    printf("unk344 %llu\n", mhit->unk344);
    printf("360 %u\n", mhit->unk360);
    printf("unk 480: %u,  %X\n", mhit->unk480_id, mhit->unk480_id);
    printf("unk 500: %u,  %X\n", mhit->unk500, mhit->unk500);
}

char *itdb_mhit(char *mhlt, int index) {
    char *mhit = mhlt + *(uint32 *)(mhlt + 4);
    uint32 header_len = *(uint32 *)(mhit + 4);
    uint32 mhit_total_len;
    for (int i = 0; i < *(uint32 *)(mhlt + 8); i ++) {
        if (mhit[0] == 'm' && mhit[1] == 'h' && mhit[2] == 'i' && mhit[3] == 't') {
            parse_mhit(mhit, 0);
            itdb_mhods(mhit);
            mhit = mhit + *(uint32 *)(mhit + 8);
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

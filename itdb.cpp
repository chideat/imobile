#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include "itdb.h"
#include "itdb_tz.h"

#define iT_DB "./iTunesCDB"

#define CHUNK 16384


static int zlib_inflate(char *outbuf, char *zdata, size_t compressed_size, size_t *uncompressed_size)
{
    int ret;
    uint32 inpos = 0;
    uint32 outpos = 0;
    unsigned have;
    z_stream strm;
    unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    *uncompressed_size = 0;

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = CHUNK;
	if (inpos+strm.avail_in > compressed_size) {
	    strm.avail_in = compressed_size - inpos;
	}
        strm.next_in = (unsigned char*)zdata+inpos;
	inpos+=strm.avail_in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            if (outbuf)  {
                strm.next_out = (unsigned char*)(outbuf + outpos);
            } else {
                strm.next_out = out;
            }
            ret = inflate(&strm, Z_NO_FLUSH);
           // g_assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - strm.avail_out;
	    *uncompressed_size += have;
	    if (outbuf) {
		outpos += have;
	    }
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}


typedef struct {
    char *contents;
    unsigned int length;
} It_Content;

typedef struct {
    It_Content *cts;
}Itdb_Ringtone;

int itdb_zlib_decompress(Itdb_Ringtone *itdb) {
    if (itdb == NULL || itdb->cts == NULL || itdb->cts->contents == NULL) {
        throw 1;
    }


    It_Content *cts = itdb->cts;
    unsigned int c_size = *(unsigned int*)(cts->contents + 8),
                 h_size = *(unsigned int*)(cts->contents + 4);
    size_t u_size = 0;

    if (h_size < 0xA9) {
#ifdef DEBUG
        fprintf(stderr, "header is too small for iTunesCDB!\n");
#endif
        throw 1;
    }
    if (*(uchar*)(cts->contents + 0xA8) == 1) {
        *(uchar*)(cts->contents + 0xA8) = 0;
    }
    else {
        fprintf(stderr, "Warning: unknown value for 0xa8 in header, should be 1 for uncompressed");
    }

    if (zlib_inflate(NULL, cts->contents + h_size, c_size - h_size, &u_size) == 0) {
        char *n_contents = new char[u_size + h_size];
        memcpy(n_contents, cts->contents, h_size);

        if (zlib_inflate(n_contents + h_size, cts->contents + h_size, c_size - h_size, &u_size) == 0) {
            delete cts->contents;
            cts->contents = n_contents;
            cts->length = u_size + h_size;
        }
        else
            throw 1;
    }
}


int itdb_zlib_compress (Itdb_Ringtone *itdb) {
    if (itdb == NULL || itdb->cts == NULL || itdb->cts->contents == NULL) {
        throw 1;
    }

    It_Content *cts = itdb->cts;
    uint32 header_len;
    uLongf compressed_len;
    uLongf uncompressed_len;
    char *new_contents;
    int status;

    header_len = *(uint32*)(cts->contents+4);
    uncompressed_len = *(uint32*)(cts->contents+8) - header_len;

    if (header_len < 0xA9) {
        fprintf(stderr, "Header is too small for iTunesCDB!\n");
        throw 1;
    }

    /* compression flag */
    if (*(uchar*)(cts->contents+0xa8) == 0) {
	    *(uchar*)(cts->contents+0xa8) = 1;
    }
    else {
	    fprintf (stderr, "Unknown value for 0xa8 in header: should be 0 for uncompressed, is %d.\n", *(uchar*)(cts->contents+0xa8));
    }

    compressed_len = compressBound (uncompressed_len);

    new_contents = new char[header_len + compressed_len];
    memcpy (new_contents, cts->contents, header_len);
    status = compress2 ((uchar*)new_contents + header_len, &compressed_len, (uchar*)cts->contents + header_len, uncompressed_len, 1);
    if (status != Z_OK) {
	    delete new_contents;
#ifdef DEBUG
		fprintf(stderr, "Error compressing iTunesCDB file!\n");
#endif
        throw 1;
    }
    delete cts->contents;
    /* update mhbd size */
    *(uint32*)(new_contents+8) = compressed_len + header_len;
    cts->contents = new_contents;
    // cts->pos = compressed_len + header_len;

    return 1;
}


char *itdb_mhsd(Itdb_Ringtone *itdb, int index = 1) {
    char *cts = itdb->cts->contents;
    uint32 header_len = *(uint32 *)(cts + 4);
    uint32 total_len = *(uint32 *)(cts + 8);
    uint32 mhsd_total_len;
    char *mhsd = cts + header_len;
    for (int i = 1; i < index; i++) {
        if (mhsd[0] == 'm' && mhsd[1] == 'h' && mhsd[2] == 's' && mhsd[3] == 'd') {
            mhsd_total_len = *(uint32 *)(mhsd + 8);
            mhsd = mhsd + mhsd_total_len;
        }
    }
    return mhsd;
}


char *itdb_mhlt(char *mhsd, int index = 1) {
    char *mhlt = mhsd + *(uint32 *)(mhsd + 4);
    return mhlt;
}

static uint64 *get64uint(char *cts, int seek) {
    return (uint64 *)(cts + seek);
}

static uint32 *get32uint(char *cts, int seek) {
    return (uint32 *)(cts + seek);
}

static uint16 *get16uint(char *cts, int seek) {
    return (uint16 *)(cts + seek);
}

static uint8 *get8uint(char *cts, int seek) {
    return (uint8 *)(cts + seek);
}

static int8 *get8int(char *cts, int seek) {
    return (int8 *)(cts + seek);
}

static float *get32float(char *cts, int seek) {
    return (float *)(cts + seek);
}

static long get_mhit(char *cts, long seek = 0) {
    Itdb_mhit *mhit = new Itdb_mhit;
    uint32 header_len = *get32uint(cts, seek + 4);
    if (header_len > 0x9c) {
        uint32 val32;
        mhit->id = get32uint(cts, seek + 16);
        mhit->visible = get32uint(cts, seek + 20);
        mhit->filetype_marker = get32uint(cts, seek + 24);
        mhit->type1 = get8uint(cts, seek + 28);
        mhit->type2 = get8uint(cts, seek + 29);
        mhit->compilation = get8uint(cts, seek + 30);
        mhit->rating = get8uint(cts, seek + 31);
        mhit->time_modified = get32uint(cts, seek + 32);  // time added
        mhit->size = get32uint(cts, seek + 36);
        mhit->tracklen = get32uint(cts, seek + 40);
        mhit->track_nr = get32uint(cts, seek + 44);
        mhit->tracks = get32uint(cts, seek + 48);
        mhit->year = get32uint(cts, seek + 52);
        mhit->bitrate = get32uint(cts, seek + 56);
        val32 = get32uint(cts, seek + 60);
        mhit->samplerate = val32 >> 16;
        mhit->samplerate_low = val32 & 0xffff;
        mhit->volume = get32uint(cts, seek + 64);
        mhit->starttime = get32uint(cts, seek + 68);
        mhit->stoptime = get32uint(cts, seek + 72);
        mhit->soundcheck = get32uint(cts, seek + 76);
        mhit->playcount = get32uint(cts, seek + 80);
        mhit->playcount2 = get32uint(cts, seek + 84);
        mhit->time_played = get32uint(cts, seek + 88);
        mhit->cd_nr = get32uint(cts, seek + 92);
        mhit->cds = get32uint(cts, seek + 96);
        mhit->drm_userid = get32uint(cts, seek + 100);
        mhit->time_added = get32uint(cts, seek + 104);
        mhit->bookmark_time = get32uint(cts, seek + 108);
        mhit->dbid = get64uint(cts, seek + 112);
        mhit->checked = get8int(cts, seek + 120);
        mhit->app_rating = get8int(cts, seek + 121);
        mhit->BPM = get16uint(cts, seek + 122);
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
        mhit->has_artwork = get8int(cts, seek + 164);
        mhit->skip_when_shuffling = get8int(cts, seek + 165);
        mhit->skip_when_shuffling = get8int (cts, seek+165);
        mhit->remember_playback_position = get8int (cts, seek+166);
        mhit->flag4 = get8int (cts, seek+167);
        mhit->dbid2 = get64uint (cts, seek+168);
        mhit->lyrics_flag = get8int (cts, seek+176);
        mhit->movie_flag = get8int (cts, seek+177);
        mhit->mark_unplayed = get8int (cts, seek+178);
        mhit->unk179 = get8int (cts, seek+179);
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
    
    printf("id: %u\n", *mhit->id);
    printf("type1: %d\n", *mhit->type1);
    printf("type2: %d\n", *mhit->type2);
    printf("rating: %d\n", *mhit->rating);
    printf("time_modified: %d\n", *mhit->time_modified);
    printf("filesize: %d\n", *mhit->size);


    uint32 *unk288 = get32uint(cts, seek + 288);
    uint32 *unk480 = get32uint(cts, seek + 480);
    uint32 *unk500 = get32uint(cts, seek + 500);
    printf("unk 288: %u,  %X\n", *unk288, *unk288);
    printf("unk 480: %u,  %X\n", *unk480, *unk480);
    printf("unk 500: %u,  %X\n", *unk500, *unk500);
}

char *itdb_mhit(char *mhlt, int index = 1) {
    char *mhit = mhlt + *(uint32 *)(mhlt + 4);
    uint32 header_len = *(uint32 *)(mhlt + 4);
    uint32 count = *(uint32 *)(mhlt + 8);
    uint32 mhit_total_len;
    for (int i = 0; i < count; i ++) {
        if (mhit[0] == 'm' && mhit[1] == 'h' && mhit[2] == 'i' && mhit[3] == 't') {
            get_mhit(mhit, 0);
            mhit_total_len = *(uint32 *)(mhit + 8);
            mhit = mhit + mhit_total_len;
        }
        else 
            fprintf(stderr, "escape\n");
        printf("===============================\n");
    }
    return mhit;
}

void itdb_mhods(char *mhit, int index = 1) {
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
            // for (int j = 0; j < mhod_total_len; j++) {
            //     printf("%c", mhod[j]);
            // }
            mhod = mhod + mhod_total_len;
        }
    }
}


int main(int argc, char **argv) {
    char name[128] = "iTunesCDB";
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

    // itdb_zlib_compress(itdb);
    
    char *cts = itdb_mhsd(itdb, 3);
    char *mhlt = itdb_mhlt(cts);
    char *mhit = itdb_mhit(mhlt, 5);

    fclose(file);
    delete itdb->cts->contents;
    delete itdb->cts;
    delete itdb;
    return 0;
}

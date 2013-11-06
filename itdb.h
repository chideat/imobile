#ifndef Itdb_H
#define Itdb_H

#include "util.h"

enum ItdbChecksumType {
    ITDB_CHECKSUM_UNKNOWN = -1,
    ITDB_CHECKSUM_NONE    = 0,
    ITDB_CHECKSUM_HASH58  = 1,
    ITDB_CHECKSUM_HASH72  = 2,
    ITDB_CHECKSUM_HASHAB  = 3
};


typedef struct {
    char *contents;
    unsigned int length;
} It_Content;

typedef struct {
    It_Content *cts;
} Itdb_Ringtone;


typedef struct _Itdb_Node {
    char name[5];
    char *buf;
} Itdb_Node;

typedef struct {
    char name[64];
    char index[32];
    uint32 bitrate;
    uint32 duration;
} Ringtone;

typedef struct _Itdb_Mhbd {
    char   header_id[4];    // 0
    uint32 header_len;      // 4
    uint32 total_len;       // 8
    uint32 compressed;      // 12  1 not compressed, 2 compressed
    uint32 version;         // 16
    uint32 num_children;    // 20
    uint64 db_id;           // 24
    uint16 platform;        // 32
    uint16 unk_0x22;        // 34
    uint32 id_0x24_1;       // 36
    uint32 id_0x24_2;       // 40
    uint32 unk_0x2c;        // 44
    uint16 hashing_scheme;  // 48
    uint16 unk0x32;         // 50
    uint32 unk0x34;         // 52
    uint32 unk_0x38;        // 56 [20];
    uint32 unk_0x3c;        // 60
    uint32 unk_0x40;        // 64
    uint16 unk_0x44;        // 68
    char   language_id[2];  // 70 [2];
    uint64 db_persistent_id; // 72
    uint32 unk_0x50;         // 80
    uint32 unk_0x54;         // 84
    uchar  hash58[20]; //[20];  // 88
    int32  timezone_offset;     // 108
    uint16 checksum_type;       // 112
    uchar  hash72[46]; //[46];  // 114
    uint16 audio_language;      // 160
    uint16 subtitle_language;   // 162
    uint16 unk_0xa4;            // 164
    uint16 unk_0xa6;            // 166
    uint16 unk_0xa8;            // 168
    // // uchar  align_0xa9;          // 170
    uchar  hashAB[57]; //[57];  // 170
    uchar  padding[]; // 227
} Itdb_Mhbd;


typedef struct _Itdb_Mhit_Header {
    char header[4];
    uint32 header_len;
    uint32 total_len;
    uint32 children;
    char   buf[568];
} Itdb_Mhit_Header;


typedef struct _Itdb_Mhit{
    char   header[4];         // 0
    uint32 header_len;        // 4
    uint32 total_len;         // 8
    uint32 children;          // 12
    uint32 id;                // 16
    uint32 visible;           // 20
    uint32 filetype_marker;   // 24
    uint8  type1;             // 28
    uint8  type2;             // 29
    uint8  compilation;       // 30
    uint8  rating;            // 31
    uint32 time_modified;     // 32
    int32  size;              // 36
    int32  tracklen;          // 40
    int32  track_nr;          // 44
    int32  tracks;            // 48
    int32  year;              // 52
    int32  bitrate;           // 56
    uint16 samplerate;        // 60
    uint16 samplerate_low;    // 62
    int32  volume;            // 64
    uint32 starttime;         // 68
    uint32 stoptime;          // 72
    uint32 soundcheck;        // 76
    uint32 playcount;         // 80
    uint32 playcount2;        // 84
    uint32 time_played;       // 88
    int32  cd_nr;             // 92
    int32  cds;               // 96
    uint32 drm_userid;        // 100
    uint32 time_added;        // 104
    uint32 bookmark_time;     // 108
    uint64 dbid;              // 112
    uint8  checked;           // 120
    uint8  app_rating;        // 121
    int16  BPM;               // 122
    uint16 artwork_count;     // 124
    uint16 unk126;            // 126
    uint32 artwork_size;      // 128
    uint32 unk132;            // 132
    float samplerate2;       // 136
    uint32 time_released;     // 140
    uint16 unk144;            // 144
    uint16 explicit_flag;     // 146
    uint32 unk148;            // 148
    uint32 unk152;            // 152
    // header_len > 0xf4
    uint32 skipcount;         // 156
    uint32 last_skipped;      // 160
    uint8  has_artwork;       // 164
    uint8  skip_when_shuffling; // 165
    uint8  remember_playback_position;  // 166
    uint8  flag4;             // 167
    uint64 dbid2;             // 168
    uint8  lyrics_flag;       // 176
    uint8  movie_flag;        // 177
    uint8  mark_unplayed;     // 178
    uint8  unk179;            // 179
    uint32 unk180;            // 180
    uint32 pregap;            // 184
    uint32 samplecount_1;       // 188
    uint32 samplecount_2;       // 192
    uint32 unk196;            // 196
    uint32 postgap;           // 200
    uint32 unk204;            // 204
    uint32 mediatype;         // 208
    uint32 season_nr;         // 212
    uint32 episode_nr;        // 216
    uint32 unk220;            // 220
    uint32 unk224;
    uint32 unk228;
    uint32 unk232;
    uint32 unk236;
    uint32 unk240;
    uint32 unk244;            // 244
    uint32 gapless_data;      // 248
    uint32 unk252;            // 252
    uint16 gapless_track_flag;// 256
    uint16 gapless_album_flag;// 258
    uint32 unk260;            // 260
    char   unk264[24];        // 264
    uint32 unk288_id;         // 288
    uint32 unk292;            // 292
    uint32 unk296;            // 296
    uint32 unk300;            // 300
    uint32 unk304;            // 304
    uint32 unk308;            // 308   8080 * 6
    char   unk312[32];        // 312
    uint64 unk344;            // 344
    uint32 mhii_link;         // 352
    uint32 unk356;            // 356
    uint32 unk360;            // 360
    char   unk364[116];       // 364
    uint32 unk480_id;         // 480
    uint32 unk484;            // 484
    uint64 unk488;            // 488
    uint32 unk496;            // 496
    uint32 unk500;            // 500
    char   unk504[80];        // 504
} Itdb_Mhit;


typedef struct _Itdb_Mhod_Header {
    char   header[4]; // 4
    uint32 header_len;
    uint32 total_len;
    uint32 type;
    uint64 unk_0xa0;
} Itdb_Mhod_Header;


typedef struct _Itdb_Mhod {
    char   header[4]; // 4
    uint32 header_len;
    uint32 total_len;
    uint32 type;
    uint64 unk_0xa0;
    uint32 unk_0xa8; // 1
    uint32 length;
    uint64 unk_0xb0; // 0
    char   data[]; // padding
    // with end 0000
} Itdb_Mhod;
#endif

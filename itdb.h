#ifndef Itdb_H
#define Itdb_H

#ifndef uint32
#define uint32 unsigned int
#endif

#ifndef uint64
#define uint64 unsigned long long
#endif

#ifndef uint16
#define uint16 unsigned short
#endif

#ifndef uint8
#define uint8 unsigned char
#endif

#ifndef uchar
#define uchar uint8
#endif

#ifndef int32
#define int32 int
#endif

#ifndef int64
#define int64 long long
#endif

#ifndef int16
#define int16 short
#endif

#ifndef int8 
#define int8 char
#endif


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


typedef struct _MhbdHeader {
    char   *header_id; // 4
    uint32 *header_len;
    uint32 *total_len;
    uint32 *unknown1;
    uint32 *version;
    uint32 *num_children;
    uint64 *db_id;
    uint16 *platform;
    uint16 *unk_0x22;
    uint64 *id_0x24;
    uint32 *unk_0x2c;
    uint16 *hashing_scheme;
    uchar  *unk_0x32;// [20];
    char   *language_id;// [2];
    uint64 *db_persistent_id;
    uint32 *unk_0x50;
    uint32 *unk_0x54;
    uchar  *hash58; //[20];
    int32  *timezone_offset;
    uint16 *unk_0x70;
    uchar  *hash72; //[46];
    uint16 *audio_language;
    uint16 *subtitle_language;
    uint16 *unk_0xa4;
    uint16 *unk_0xa6;
    uint16 *unk_0xa8;
    uchar  *align_0xa9;
    uchar  *hashAB; //[57];
    uchar  *padding; // 
} Itdb_Mhbd;

typedef struct _Itdb_Mhit {
    Itdb_Node node;
    /* end of new fields in libgpod 0.5.0 */
    uint32 *id;
    int32  *size;
    int32  *tracklen;
    int32  *cd_nr;
    int32  *cds;
    int32  *track_nr;
    int32  *tracks;
    int32  *bitrate;
    uint16 *samplerate;
    uint16 *samplerate_low;
    int32  *year;
    int32  *volume;
    uint32 *soundcheck;
    uint32 *time_added;
    uint32 *time_modified;
    uint32 *time_played;
    uint32 *bookmark_time;
    uint8 *rating;
    uint32 *playcount;
    uint32 *playcount2;
    uint32 *recent_playcount;
    bool   *transferred;
    int16  *BPM;
    uint8  *app_rating;
    uint8  *type1;
    uint8  *type2;
    uint8  *compilation;
    uint32 *starttime;
    uint32 *stoptime;
    uint8  *checked;
    uint64 *dbid;
    uint32 *drm_userid;
    uint32 *visible;
    uint32 *filetype_marker;
    uint16 *artwork_count;
    uint32 *artwork_size;
    float  *samplerate2;
    uint16 *unk126;
    uint32 *unk132;
    uint32 *time_released;
    uint16 *unk144;
    uint16 *explicit_flag;
    uint32 *unk148;
    uint32 *unk152;
    uint32 *skipcount;
    uint32 *recent_skipcount;
    uint32 *last_skipped;
    uint8  *has_artwork;
    uint8  *skip_when_shuffling;
    uint8  *remember_playback_position;
    uint8  *flag4;
    uint64 *dbid2;
    uint8  *lyrics_flag;
    uint8  *movie_flag;
    uint8  *mark_unplayed;
    uint8  *unk179;
    uint32 *unk180;
    uint32 *pregap;
    uint64 *samplecount;
    uint32 *unk196;
    uint32 *postgap;
    uint32 *unk204;
    uint32 *mediatype;
    uint32 *season_nr;
    uint32 *episode_nr;
    uint32 *unk220;
    uint32 *unk224;
    uint32 *unk228, *unk232, *unk236, *unk240, *unk244;
    uint32 *gapless_data;
    uint32 *unk252;
    uint16 *gapless_track_flag;
    uint16 *gapless_album_flag;

    uint32 *unk288_id;
    uint32 *unk480_id;
    uint32 *unk500_0;
} Itdb_Mhit;


#endif

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

typedef struct _Itdb_Node {
    char name[5];
    char *buf;
} Itdb_Node;


struct _Itdb_Track {
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
};

typedef _Itdb_Track Itdb_Track;

static uint64 *get64uint(char *cts, int seek) {
    return (uint64 *)(cts + seek);
}

static uint32 *get32uint(char *cts, int seek) {
    return (uint32 *)(cts + seek);
}

static int32 *get32int(char *cts, int seek) {
    return (int32 *)(cts + seek);
}

static uint16 *get16uint(char *cts, int seek) {
    return (uint16 *)(cts + seek);
}

static int16 *get16int(char *cts, int seek) {
    return (int16 *)(cts + seek);
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

#define getuchar get8uint
#define getchar get8int

#include "ringtone-parse.h"
#include "itdb_zlib.h"
#include <stdio.h>


int main(int argc, char **argv) {
    time_t seed;
    time(&seed);
    srand(seed);
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


    parse_mhbd(itdb->cts->contents);
    
    char *cts = itdb_mhsd(itdb->cts->contents);
    char *mhlt = itdb_mhlt(cts);
    char *mhit = itdb_mhit(mhlt);

    fclose(file);
    delete itdb->cts->contents;
    delete itdb->cts;
    delete itdb;

    printf("sizeof float: %d\n", sizeof(Itdb_Mhit));
    printf("seed: %u, id: %u\n", seed,  _id());
    return 0;
}

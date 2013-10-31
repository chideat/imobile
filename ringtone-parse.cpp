
#include "itdb.h"
#include "ConvertUTF.h"


char *init_header(char header[4]){
    char *buf = new char[MAX_BUF];
    memset(buf, 0, sizeof(buf));

    Itdb_Mhod *mhod = (Itdh_Mhod *)buf;
    strncpy(mhod->header, header, 4);
    mhod->header_len = sizeof(Itdb_Mhod_Header);




}

/**
 * deal with
 *     header length
 *     total length
 *     encode
 */
Itdb_Mhod * write_mode_type_1() {
    Itdb_Mhod *mhod = (Itdb_Mhod *) init_header();




}



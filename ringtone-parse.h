#ifndef RINGTONE_PARSE_H
#define RINGTONE_PARSE_H

#include "itdb.h"

Itdb_Mhbd *parse_mhbd(char *cts, long seek = 0);
long parse_mhit(char *cts, long seek = 0);

char *itdb_mhsd(char *mhbd, int index = 1);
char *itdb_mhlt(char *mhsd, int index = 1);
char *itdb_mhit(char *mhlt, int index = 1);
void itdb_mhods(char *mhit, int index = 1);
#endif

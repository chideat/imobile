
#include "itdb.h"
#include "util.h"


char *init_header(char header[4]);
Itdb_Mhod * write_mode_type_1(char *data, uint32 len);
Itdb_Mhod * write_mode_type_2(char *data, uint32 len);
Itdb_Mhod * write_mode_type_6(char *data, uint32 len = 0);
Itdb_Mhod * write_mode_type_3(char *data, uint32 len);
Itdb_Mhod * write_mode_type_4(char *data, uint32 len);
Itdb_Mhod * write_mode_type_0xa6(char *data, uint32 len);

#ifndef TZ_H
#define TZ_H
#include <time.h>

#define timezone_shift 0
#ifndef uint64
#define uint64 unsigned long long
#endif

#define TIME_GAP 2082844800

#include "itdb.h"

uint32 get_mac_time() {
    time_t now;
    time(&now);
    return (uint32)now + TIME_GAP + timezone_shift;
}


time_t device_time_mac_to_time_t(uint64 mactime) {
    if (mactime != 0) 
        return (time_t)(mactime - TIME_GAP - timezone_shift);
    else
        return (time_t)mactime;
}

uint64 device_time_time_t_to_mac(time_t timet) {
    if (timet != 0) 
        return ((uint64) timet) + TIME_GAP + timezone_shift;
    else 
        return 0;
}

#endif

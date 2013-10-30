
#include <time.h>

#define timezone_shift 0
#ifndef uint64
#define uint64 unsigned long long
#endif

time_t device_time_mac_to_time_t(uint64 mactime) {
    if (mactime != 0) 
        return (time_t)(mactime - 2082844800 - timezone_shift);
    else
        return (time_t)mactime;
}

uint64 device_time_time_t_to_mac(time_t timet) {
    if (timet != 0) 
        return ((uint64) timet) + 2082844800 + timezone_shift;
    else 
        return 0;
}

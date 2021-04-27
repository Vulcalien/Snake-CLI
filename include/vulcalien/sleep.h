/*
 * Vulcalien's sleep header
 *
 * version : 2021.04.25
 */

#ifndef VULC_VULCALIEN_SLEEP
#define VULC_VULCALIEN_SLEEP

#ifdef __unix__
    #include <unistd.h>
    #define SLEEP(millis) usleep(millis * 1000)
#elif _WIN32
    #include <windows.h>
    #define SLEEP(millis) Sleep(millis)
#endif

#endif // VULC_VULCALIEN_SLEEP

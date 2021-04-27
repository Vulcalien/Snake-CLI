/*
 * Vulcalien's core header
 *
 * version: 0.0.1
 */
#ifndef VULC_VULCALIEN_CORE
#define VULC_VULCALIEN_CORE

#if !(defined(__unix__) || defined(_WIN32))
    #error The system is not supported
#endif

#include <stdbool.h>
#include <stdint.h>

// --- Data Types ---

typedef int8_t   i8;
typedef uint8_t  ui8;

typedef int16_t  i16;
typedef uint16_t ui16;

typedef int32_t  i32;
typedef uint32_t ui32;

typedef int64_t  i64;
typedef uint64_t ui64;

// --- SLEEP macro

#ifdef __unix__
    #include <unistd.h>
    #define SLEEP(millis) usleep(millis * 1000)
#elif _WIN32
    #include <windows.h>
    #define SLEEP(millis) Sleep(millis)
#endif

// --- nano_time function

extern ui64 nano_time(void);

#endif // VULC_VULCALIEN_CORE

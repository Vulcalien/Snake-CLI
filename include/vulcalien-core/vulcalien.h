/*
 * Vulcalien Core Library
 *
 * version: 0.0.2
 */
#ifndef VULC_VULCALIEN_CORE
#define VULC_VULCALIEN_CORE

#if !(defined(__unix__) || defined(_WIN32))
    #error The system is not supported
#endif

#include <stdbool.h>
#include <stdint.h>

// --- Data Types ---

typedef int8_t   i8;    // signed 8 bit integer
typedef uint8_t  ui8;   // unsigned 8 bit integer

typedef int16_t  i16;   // signed 16 bit integer
typedef uint16_t ui16;  // unsigned 16 bit integer

typedef int32_t  i32;   // signed 32 bit integer
typedef uint32_t ui32;  // unsigned 32 bit integer

typedef int64_t  i64;   // signed 64 bit integer
typedef uint64_t ui64;  // unsigned 64 bit integer

// --- SLEEP macro

#ifdef __unix__
    #include <unistd.h>
    #define SLEEP(millis) usleep(millis * 1000)
#elif _WIN32
    #include <windows.h>
    #define SLEEP(millis) Sleep(millis)
#endif

// --- nanotime function

extern ui64 nanotime(void);

#endif // VULC_VULCALIEN_CORE

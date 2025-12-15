#ifndef _API_H_
#define _API_H_
#include <stdio.h>

// USER API
int api_syscall(void);
int api_putchar(void);
int api_seconds(void);
int api_millis(void);
int api_micros(void);
int api_delay(void);
int api_delayMicroseconds(void);
int api_digitalRead(void);
int api_digitalWrite(void);
int api_digitalToggle(void);
int api_analogRead(void);
int api_analogWrite(void);

// LIBC
int api_memset(void);
int api_memcpy(void);
int api_memchr(void);
int api_memcmp(void);
int api_memmove(void);
int api_strlen(void);

// DEBUG

#if 1
#define PRINT_API(FORMAT, ...)         \
    do                                 \
    {                                  \
        printf("[A] ");                \
        printf("%s(", __func__);       \
        printf(FORMAT, ##__VA_ARGS__); \
        printf(")\n");                 \
        fflush(stdout);                \
    } while (0)
#else
#define PRINTF_API ((void)0)
#endif

#endif // _API_H_
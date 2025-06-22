#ifndef _API_H_
#define _API_H_

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>

// clang-format off
// clang-format on

uint32_t seconds(void);
uint32_t millis(void);
uint32_t micros(void);

void delay(uint32_t);
void delayMicroseconds(uint32_t);

int digitalRead(uint32_t pin);
void digitalWrite(uint32_t pin, uint32_t val);
void digitalToggle(uint32_t pin);

int analogRead(uint32_t pin);
void analogWrite(uint32_t pin, int val);

typedef enum
{
    IO_VOID = 0,
    __IO_END = 0xFFFFFFFF
} io_commands;
int api_syscall(io_commands, uint32_t variable_0, va_list);

void api_putchar(char);

#endif // _API_H_
#include "RV.h"

#include <windows.h>
extern unsigned long millis();
extern unsigned long micros();
extern void delay(uint32_t ms);
extern void delayMicroseconds(uint32_t us);
extern void digitalWrite(uint8_t pin, uint8_t val);

int api_syscall(void)
{
    PRINT_API("");
    return RV_OK;
}

int api_putchar(void)
{
    putchar(CPU.REG.a0 & 0xFF);
    return RV_OK;
}

int api_seconds(void)
{
    PRINT_API("");
    return RV_OK;
}

int api_millis(void)
{
    PRINT_API("");
    CPU.REG.a0 = 42;
    return RV_OK;
}

int api_micros(void)
{
    PRINT_API("");
    CPU.REG.a0 = 0x1234;
    return RV_OK;
}

int api_delay(void)
{
    PRINT_API("%u", CPU.REG.a0);
    Sleep(CPU.REG.a0);
    return RV_OK;
}

int api_delayMicroseconds(void)
{
    PRINT_API("%u", CPU.REG.a0);
    Sleep(CPU.REG.a0 / 1000);
    return RV_OK;
}

int api_digitalRead(void)
{
    PRINT_API("");
    return RV_OK;
}

int api_digitalWrite(void)
{
    PRINT_API("");
    return RV_OK;
}

int api_digitalToggle(void)
{
    PRINT_API("");
    return RV_OK;
}

int api_analogRead(void)
{
    PRINT_API("");
    return RV_OK;
}

int api_analogWrite(void)
{
    PRINT_API("");
    return RV_OK;
}
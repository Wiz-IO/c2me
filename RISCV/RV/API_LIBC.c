#include "RV.h"

int api_memset(void) // void * memset(void *ptr, int value, size_t len);
{
    PRINT_API("0x%X, %d, %d", CPU.REG.a0, CPU.REG.a1, CPU.REG.a2);
    uint8_t *ptr = V2R(CPU.REG.a0, CPU.REG.a2);
    if (ptr && IS_RAM(CPU.REG.a0))
    {
        CPU.REG.a0 = R2V(memset(ptr, CPU.REG.a1, CPU.REG.a2));
        return RV_OK;
    }
    return E_NULL;
}

int api_memcpy(void) // void * memcpy( void * dst, const void * src, size_t len );
{
    PRINT_API("0x%X, 0x%X, %d", CPU.REG.a0, CPU.REG.a1, CPU.REG.a2);
    uint8_t *dst = V2R(CPU.REG.a0, CPU.REG.a2);
    uint8_t *src = V2R(CPU.REG.a1, CPU.REG.a2);
    if (dst && src && IS_RAM((uint32_t)dst))
    {
        CPU.REG.a0 = (uint32_t)memcpy(dst, src, CPU.REG.a2);
        return RV_OK;
    }
    return E_NULL;
}

int api_memmove(void) // void * memmove(void * dst, const void * src, size_t len);
{
    PRINT_API("0x%X, 0x%X, %d", CPU.REG.a0, CPU.REG.a1, CPU.REG.a2);
    uint8_t *dst = V2R(CPU.REG.a0, CPU.REG.a2);
    uint8_t *src = V2R(CPU.REG.a1, CPU.REG.a2);
    if (dst && src && IS_RAM((uint32_t)dst))
    {
        CPU.REG.a0 = (uint32_t)memmove(dst, src, CPU.REG.a2);
        return RV_OK;
    }
    return E_NULL;
}

int api_memchr(void) // void *	memchr(const void * ptr, int c, size_t len);
{
    PRINT_API("0x%X, %d, %d", CPU.REG.a0, CPU.REG.a1, CPU.REG.a2);
    uint8_t *ptr = V2R(CPU.REG.a0, CPU.REG.a2);
    if (ptr)
    {
        CPU.REG.a0 = (uint32_t)memchr(ptr, CPU.REG.a1, CPU.REG.a2);
        return RV_OK;
    }
    return E_NULL;
}

int api_memcmp(void) // int memcmp(const void *p1, const void *p2, size_t len);
{
    PRINT_API("0x%X, 0x%X, %d", CPU.REG.a0, CPU.REG.a1, CPU.REG.a2);
    uint8_t *a = V2R(CPU.REG.a0, CPU.REG.a2);
    uint8_t *b = V2R(CPU.REG.a1, CPU.REG.a2);
    if (a && b)
    {
        CPU.REG.a0 = memcmp(a, b, CPU.REG.a2);
        return RV_OK;
    }
    return E_NULL;
}

int api_strlen(void) // size_t strlen(const char * str);
{
    PRINT_API("\"%s\"", V2R(CPU.REG.a0, 0));
    uint8_t *s = V2R(CPU.REG.a0, 0);
    if (s)
    {
        CPU.REG.a0 = strlen((const char *)s);
        return RV_OK;
    }
    return E_NULL;
}
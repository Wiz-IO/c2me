#ifndef _PIC_H_
#define _PIC_H_
#include <stdint.h>

#define HEADER_MAGIC 0xFECAFECA
#define API_VERSION 0x10000000

#define ROM_BASE 0                 // винаги ще бъде 0
#define MAX_ROM_SIZE 32760         // ще се коригират от builder
#define ROM_START sizeof(header_t) // skip header

#define RAM_BASE 0x10000000 // винаги ще бъде 0x10000000
#define MAX_RAM_SIZE 32768  // ще се коригират от builder

#define MAX_HEAP_SIZE 4096  // ще се коригират от builder
#define MAX_STACK_SIZE 4096 // ще се коригират от builder

typedef struct
{
    uint32_t magic;       /* 00 MAGIC (0xFECAFECA) */
    uint32_t version;     /* 01 VERSION */
    uint32_t entry;       /* 02 ENTRY (address of app_entry) */
    uint32_t heap_size;   /* 03 HEAP SIZE */
    uint32_t stack_size;  /* 04 STACK SIZE */
    uint32_t ram_end;     /* 05 STACK ADDRESS */
    uint32_t data_load;   /* 06 ROM адреса на .data */
    uint32_t data_start;  /* 07 RAM Start of .data */
    uint32_t data_end;    /* 08 RAM End of .data */
    uint32_t bss_start;   /* 09 RAM Start of .bss */
    uint32_t bss_end;     /* 10 RAM End of .bss */
    uint32_t reserved[5]; /* Reserved for alignment (total 64 bytes) */
} header_t;

#define MAX_BIN_SIZE (sizeof(header_t) + MAX_ROM_SIZE + MAX_RAM_SIZE)

typedef int (*api_fun)(void); // call API
typedef struct
{
    uint32_t hash;
    api_fun function;
} api_t;
extern api_t API_TABLE[];
#define API_TABLE_SIZE (sizeof(API_TABLE) / sizeof(API_TABLE[0]))

typedef struct
{
    uint32_t a, b, c, hash;
} api_rel_t;

int app_init(void);

#if 1
#define PRINT_PIC(FORMAT, ...)                \
    do                                        \
    {                                         \
        printf("[P] " FORMAT, ##__VA_ARGS__); \
        fflush(stdout);                       \
    } while (0)
#else
#define PRINT_PIC(FORMAT, ...) ((void)0)
#endif

#endif // _PIC_H_
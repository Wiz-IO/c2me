#ifndef _RV_H_
#define _RV_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "PIC.h"
#include "API.h"

typedef union RV_u
{
    uint32_t x[32];
    struct
    {
        uint32_t x0;  // Нулев регистър (винаги 0)
        uint32_t x1;  // Връщащ адрес (ra)
        uint32_t x2;  // Указател на стека (sp)
        uint32_t x3;  // Глобален указател (gp)
        uint32_t x4;  // Указател на нишка (tp)
        uint32_t x5;  // Временен/алтернативна връзка (t0)
        uint32_t x6;  // Временен (t1)
        uint32_t x7;  // Временен (t2)
        uint32_t x8;  // Запазен/фрейм указател (s0/fp)
        uint32_t x9;  // Запазен (s1)
        uint32_t x10; // Аргумент/резултат (a0)
        uint32_t x11; // Аргумент/резултат (a1)
        uint32_t x12; // Аргумент (a2)
        uint32_t x13; // Аргумент (a3)
        uint32_t x14; // Аргумент (a4)
        uint32_t x15; // Аргумент (a5)
        uint32_t x16; // Временен (a6)
        uint32_t x17; // Временен (a7)
        uint32_t x18; // Запазен (s2)
        uint32_t x19; // Запазен (s3)
        uint32_t x20; // Запазен (s4)
        uint32_t x21; // Запазен (s5)
        uint32_t x22; // Запазен (s6)
        uint32_t x23; // Запазен (s7)
        uint32_t x24; // Запазен (s8)
        uint32_t x25; // Запазен (s9)
        uint32_t x26; // Запазен (s10)
        uint32_t x27; // Запазен (s11)
        uint32_t x28; // Временен (t3)
        uint32_t x29; // Временен (t4)
        uint32_t x30; // Временен (t5)
        uint32_t x31; // Временен (t6)
    };
    struct
    {
        uint32_t zero;
        uint32_t ra;
        uint32_t sp;
        uint32_t gp;
        uint32_t tp;
        uint32_t t0;
        uint32_t t1;
        uint32_t t2;
        uint32_t s0;
        uint32_t s1;
        uint32_t a0;
        uint32_t a1;
        uint32_t a2;
        uint32_t a3;
        uint32_t a4;
        uint32_t a5;
        uint32_t a6;
        uint32_t a7;
        uint32_t s2;
        uint32_t s3;
        uint32_t s4;
        uint32_t s5;
        uint32_t s6;
        uint32_t s7;
        uint32_t s8;
        uint32_t s9;
        uint32_t s10;
        uint32_t s11;
        uint32_t t3;
        uint32_t t4;
        uint32_t t5;
        uint32_t t6;
    };

} RV_t;

typedef union __attribute__((packed)) // валидно за GCC little-endian
{
    uint32_t raw;
    struct
    {
        uint32_t opcode : 7;
    } any;
    struct
    {
        uint32_t opcode : 7, rd : 5, f3 : 3, rs1 : 5, rs2 : 5, f7 : 7;
    } rtype;
    struct
    {
        uint32_t opcode : 7, rd : 5, f3 : 3, rs1 : 5, imm11_0 : 12;
    } itype;
    struct
    {
        uint32_t opcode : 7, rd : 5, imm19_12 : 8, imm11 : 1, imm10_1 : 10, imm20 : 1;
    } jtype;
    struct
    {
        uint32_t opcode : 7, imm4_0 : 5, f3 : 3, rs1 : 5, rs2 : 5, imm11_5 : 7;
    } stype;
    struct
    {
        uint32_t opcode : 7, imm11 : 1, imm4_1 : 4, f3 : 3, rs1 : 5, rs2 : 5, imm10_5 : 6, imm12 : 1;
    } btype;
    struct
    {
        uint32_t opcode : 7, rd : 5, imm31_12 : 20;
    } utype;
    struct
    {
        uint32_t opcode : 7, rd : 5, f3 : 3, rs1 : 5, f12 : 12;
    } special;
} instruction_t;

typedef struct
{
    int ERROR;

    RV_t REG;

    instruction_t instr; // инструкция
    uint32_t pc;

    uint8_t *ROM;
    uint32_t ROM_SIZE;
    uint8_t *RAM;
    uint32_t RAM_SIZE;
    uint32_t SP_BEGIN;
    uint32_t SP_END; // end of ram
} riscv;

extern riscv CPU;

typedef enum
{
    OPCODE_LOAD = 0x03,     // Зареждане от памет
    OPCODE_MISC_MEM = 0x0F, // FENCE инструкции
    OPCODE_OP_IMM = 0x13,   // Операции с непосредствена стойност
    OPCODE_AUIPC = 0x17,    // Добавяне на U-immediate към PC
    OPCODE_STORE = 0x23,    // Запис в памет
    OPCODE_AMO = 0x2F,      // Атомарни операции
    OPCODE_OP = 0x33,       // Аритметични/логически операции
    OPCODE_LUI = 0x37,      // Зареждане на U-immediate
    OPCODE_BRANCH = 0x63,   // Условни скокове
    OPCODE_JALR = 0x67,     // Скок и връщане (индиректен)
    OPCODE_JAL = 0x6F,      // Скок и връщане (директен)
    OPCODE_SYSTEM = 0x73,   // Системни инструкции
} riscv_opcode_t;

typedef enum
{
    RV_DONE = 2,
    RV_JUMP = 1,
    RV_OK = 0,
    E_ERROR = -1,
    E_INVALID_HEADER = -2,
    E_INVALID_RELOCATION = -3,
    E_INVALID_INSTRUCTION = -4,
    E_INVALID_ADDRESS = -5,
    E_STACK_OVERFLOW = -6,
    E_ALIGNMENT = -7,
    E_TRAP = -8,
    E_DIVIDE = -9,
    E_NULL = -10,
    E_API = -11,
} RV_ERROR;

///////////////////////////////////////////////////////////

#define TRUE (1)
#define FALSE (0)

#define SIGN_EXTEND_8(V) ((int32_t)((int8_t)V))
#define SIGN_EXTEND_16(V) ((int32_t)((int16_t)V))
#define SIGN_EXTEND(V, S) ((int32_t)(V << (32 - S)) >> (32 - S))

#define SUBMASK(x) ((1L << ((x) + 1)) - 1)
#define BIT(obj, st) (((obj) >> (st)) & 1)
#define BITS(obj, st, fn) (((obj) >> (st)) & SUBMASK((fn) - (st)))

int bit_count(uint32_t x);

///////////////////////////////////////////////////////////

int API_HOOK(uint32_t address);
bool IS_API_ADDRESS(uint32_t address);

bool IS_ADDRESS(uint32_t addr);
bool IS_TRAP(uint32_t addr);
bool IS_RAM(uint32_t addr);
uint8_t *V2R(uint32_t address, uint32_t size);
uint32_t R2V(uint8_t *ptr);

int READ_MEM_32(uint32_t address, uint32_t *data);
int READ_MEM_16(uint32_t address, uint32_t *data);
int READ_MEM_8(uint32_t address, uint32_t *data);

int WRITE_MEM_32(uint32_t address, uint32_t data);
int WRITE_MEM_16(uint32_t address, uint16_t data);
int WRITE_MEM_8(uint32_t address, uint8_t data);

int SET_PC(uint32_t address);

///////////////////////////////////////////////////////////

int app_run(void);
int rv_execute(void);

// DEBUG //////////////////////////////////////////////////

extern const char *RN[32];

void PRINT_REG(void);
void PRINT_REG_16(void);

#if 1
#define FLUSH() fflush(stdout)
#define PRINT printf
#else
#define PRINT
#define FLUSH()
#endif

#if 1
#define PRINT_ERROR(FORMAT, ...)                   \
    do                                             \
    {                                              \
        PRINT("\n[ERROR] " FORMAT, ##__VA_ARGS__); \
        FLUSH();                                   \
    } while (0)
#else
#define PRINT_ERROR(FORMAT, ...) ((void)0)
#endif

#if 1
#define PRINT_ASM(FORMAT, ...)        \
    do                                \
    {                                 \
        PRINT("    %08X: ", CPU.pc);    \
        PRINT(FORMAT, ##__VA_ARGS__); \
        FLUSH();                      \
    } while (0)
#else
#define PRINT_ASM(FORMAT, ...) ((void)0)
#endif

#define FUNC() PRINT("[I] %s()\n", __func__)

#endif // _RV_H_
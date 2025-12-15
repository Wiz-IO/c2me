#include "RV.h"

riscv CPU;

const char *RN[32] = {
    "ZERO",
    "RA",
    "SP",
    "GP",
    "TP",
    "T0",
    "T1",
    "T2",
    "S0",
    "S1",
    "A0",
    "A1",
    "A2",
    "A3",
    "A4",
    "A5",
    "A6",
    "A7",
    "S2",
    "S3",
    "S4",
    "S5",
    "S6",
    "S7",
    "S8",
    "S9",
    "S10",
    "S11",
    "T3",
    "T4",
    "T5",
    "T6",
};

void PRINT_REG(void)
{
    PRINT("\n[R] ");
    for (int i = 0; i < 32; i++)
    {
        PRINT("x%02d=0x%08X ", i, CPU.REG.x[i]);
        if (((i + 1) % 8) == 0)
        {
            PRINT("\n    ");
        }
    }
    // PRINT("\n");
}

void PRINT_REG_16(void)
{
    PRINT("\n[R] ");
    for (int i = 0; i < 16; i++)
    {
        PRINT("%s=0x%X  ", RN[i], CPU.REG.x[i]);
        if (((i + 1) % 8) == 0)
        {
            PRINT("\n    ");
        }
    }
    PRINT("\n");
}

bool IS_TRAP(uint32_t address) // is in header
{
    return address < sizeof(header_t);
}

bool IS_ADDRESS(uint32_t address) // RAM or ROM
{
    return ((address >= ROM_START) && (address < ROM_BASE + CPU.ROM_SIZE)) ||
           ((address >= RAM_BASE) && (address < CPU.SP_END));
}

bool IS_RAM(uint32_t address)
{
    return (address >= RAM_BASE) && (address < CPU.SP_END);
}

uint8_t *V2R(uint32_t address, uint32_t size) // Virtual To Real Address
{
    if (address)
    {
        if (address >= ROM_BASE && size <= CPU.ROM_SIZE && (address - ROM_BASE) <= CPU.ROM_SIZE - size)
            return &CPU.ROM[address - ROM_BASE];

        if (address >= RAM_BASE && size <= CPU.RAM_SIZE && (address - RAM_BASE) <= CPU.RAM_SIZE - size)
            return &CPU.RAM[address - RAM_BASE];
    }
    return NULL;
}

uint32_t R2V(uint8_t *ptr) // Real To Virtual Address
{
    if (ptr >= CPU.ROM && ptr < CPU.ROM + CPU.ROM_SIZE)
        return ROM_BASE + (uint32_t)(ptr - CPU.ROM);

    if (ptr >= CPU.RAM && ptr < CPU.RAM + CPU.RAM_SIZE)
        return RAM_BASE + (uint32_t)(ptr - CPU.RAM);

    return 0; // невалиден указател
}

int SET_PC(uint32_t address)
{
    // PRINT("SET PC: 0x%08X\n", address);
    int err = API_HOOK(address);
    if (RV_OK == err)
    {
        return RV_JUMP;
    }
    else if (err < 0)
    {
        return err;
    }
    if (IS_ADDRESS(CPU.pc))
    {
        CPU.pc = address;
        return RV_JUMP;
    }
    return E_TRAP;
}

int READ_MEM_32(uint32_t address, uint32_t *data)
{
    if (!data)
        return E_NULL;
    if (address & 0x3)
    {
        PRINT_ERROR("READ_MEM_32: Unaligned access: 0x%X\n", address);
        return E_ALIGNMENT;
    }
    if (address >= ROM_BASE && address < ROM_BASE + CPU.ROM_SIZE - 3)
    {
        *data = *((uint32_t *)&CPU.ROM[address - ROM_BASE]);
        return RV_OK;
    }
    else if (address >= RAM_BASE && address < CPU.SP_END - 3)
    {
        *data = *((uint32_t *)&CPU.RAM[address - RAM_BASE]);
        return RV_OK;
    }
    PRINT_ERROR("READ_MEM_32: Invalid Address: 0x%X\n", address);
    return E_INVALID_ADDRESS;
}

int READ_MEM_16(uint32_t address, uint32_t *data)
{
    if (!data)
        return E_NULL;
    if (address & 0x1)
    {
        PRINT_ERROR("READ_MEM_16: Unaligned access: 0x%X\n", address);
        return E_ALIGNMENT;
    }
    if (address >= ROM_BASE && address < ROM_BASE + CPU.ROM_SIZE - 1)
    {
        *data = *((uint16_t *)&CPU.ROM[address - ROM_BASE]);
        return RV_OK;
    }
    else if (address >= RAM_BASE && address < CPU.SP_END - 1)
    {
        *data = *((uint16_t *)&CPU.RAM[address - RAM_BASE]);
        return RV_OK;
    }
    PRINT_ERROR("READ_MEM_16: Invalid Address: 0x%08X\n", address);
    return E_INVALID_ADDRESS;
}

int READ_MEM_8(uint32_t address, uint32_t *data)
{
    if (!data)
        return E_NULL;
    if (address >= ROM_BASE && address < ROM_BASE + CPU.ROM_SIZE)
    {
        *data = CPU.ROM[address - ROM_BASE];
        return RV_OK;
    }
    else if (address >= RAM_BASE && address < CPU.SP_END)
    {
        *data = CPU.RAM[address - RAM_BASE];
        return RV_OK;
    }
    PRINT_ERROR("READ_MEM_8: Invalid Address: 0x%08X\n", address);
    return E_INVALID_ADDRESS;
}

int WRITE_MEM_32(uint32_t address, uint32_t data)
{
    if (address & 0x3)
    {
        PRINT_ERROR("WRITE_MEM_32: Unaligned access: 0x%X\n", address);
        return E_ALIGNMENT;
    }
    if (address >= RAM_BASE && address < CPU.SP_END - 3)
    {
        *((uint32_t *)&CPU.RAM[address - RAM_BASE]) = data;
        return RV_OK;
    }
    PRINT_ERROR("WRITE_MEM_32: Invalid Address: 0x%08X\n", address);
    return E_INVALID_ADDRESS;
}

int WRITE_MEM_16(uint32_t address, uint16_t data)
{
    if (address & 0x1)
    {
        PRINT_ERROR("WRITE_MEM_16: Unaligned access: 0x%X\n", address);
        return E_ALIGNMENT;
    }
    if (address >= RAM_BASE && address < CPU.SP_END - 1)
    {
        *((uint16_t *)&CPU.RAM[address - RAM_BASE]) = data;
        return RV_OK;
    }
    PRINT_ERROR("WRITE_MEM_16: Invalid Address: 0x%08X\n", address);
    return E_INVALID_ADDRESS;
}

int WRITE_MEM_8(uint32_t address, uint8_t data)
{
    if (address >= RAM_BASE && address < CPU.SP_END)
    {
        CPU.RAM[address - RAM_BASE] = data;
        return RV_OK;
    }
    PRINT_ERROR("WRITE_MEM_8: Invalid Address: 0x%08X\n", address);
    return E_INVALID_ADDRESS;
}

static int rv_setup(void)
{
    header_t *app_header = (header_t *)CPU.ROM;

    CPU.pc = app_header->entry;
    CPU.REG.ra = -1; // exit()

    CPU.REG.sp = CPU.SP_END = app_header->ram_end;
    CPU.SP_BEGIN = CPU.SP_END - app_header->stack_size;

    PRINT("STACK : 0x%X - 0x%X\n", CPU.SP_BEGIN, CPU.SP_END);
    PRINT("START : PC = 0x%X, SP = %X\n\n", CPU.pc, CPU.REG.sp);

    return RV_OK;
}

int app_run(void)
{
    PRINT("APPLICATION BEGIN\n");
    int err = rv_setup();
    int count = 64;
    while (count)
    {
        err = rv_execute();
        if (RV_DONE == err)
        {
            PRINT("\nAPPLICATION END: %d\n", CPU.REG.a0);
            return 0;
        }
        else if (err)
        {
            PRINT_ERROR("BLUE SCREEN OF DEAD: %d\n", err);
            PRINT_REG();
            break;
        }

        // fflush(stdout);
        // int ch = getc(stdin);
        // count--;
    }
    return err;
}
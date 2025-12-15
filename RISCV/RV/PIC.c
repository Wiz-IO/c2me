#include "RV.h"

api_t API_TABLE[] = {
#include "API_TABLE.h" // allowed functions
};

bool IS_API_ADDRESS(uint32_t address)
{
    return false; // TODO
}

int API_HOOK(uint32_t address)
{
    int err = 1; // mean: not a hook, Continue with the instruction
    for (uint32_t i = 0; i < API_TABLE_SIZE; i++)
    {
        if ((uint32_t)API_TABLE[i].hash == address)
        {
            err = API_TABLE[i].function(); // call API_FUNCTION()
            CPU.pc = CPU.REG.ra;           // return from API
            break;
        }
    }
    return err;
}

static uint32_t get_function_by_hash(uint32_t hash)
{
    for (uint32_t i = 0; i < (sizeof(API_TABLE) / sizeof(API_TABLE[0])); i++)
    {
        if (API_TABLE[i].hash == hash)
            return (uint32_t)API_TABLE[i].function;
    }
    return 0;
}

int app_init(void)
{
    // Инициализация на CPU
    memset(&CPU, 0, sizeof(riscv));

    // Отваряне на файла
    const char *filename = "../APPLICATION/firmware.bin";
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        PRINT_ERROR("File open: %s\n", filename);
        return -1;
    }

    // Проверка на размера на файла
    fseek(file, 0, SEEK_END);
    size_t size, file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (file_size > MAX_BIN_SIZE || file_size < sizeof(header_t))
    {
        PRINT_ERROR("Invalid file size: %zu\n", file_size);
        fclose(file);
        return -1;
    }

    // Четене на хедъра
    header_t app_header;
    if (fread(&app_header, 1, sizeof(header_t), file) != sizeof(header_t))
    {
        PRINT_ERROR("Failed to read header\n");
        fclose(file);
        return -1;
    }
    // DUMP("HEADER", &app_header, sizeof(header_t), 0);

    // Валидация на хедъра
    if (app_header.magic != HEADER_MAGIC)
    {
        PRINT_ERROR("Invalid Header Magic: 0x%X\n", app_header.magic);
        fclose(file);
        return -1;
    }
    if (app_header.version != API_VERSION)
    {
        PRINT_ERROR("Invalid API version: 0x%X\n", app_header.version);
        fclose(file);
        return -1;
    }

    if (app_header.entry < sizeof(header_t) || app_header.entry >= file_size)
    {
        PRINT_ERROR("Invalid Entry Point: 0x%X\n", app_header.entry);
        fclose(file);
        return -1;
    }

    // Проверка на подравняване
    if (app_header.data_load % 4 != 0 ||
        app_header.data_start % 4 != 0 ||
        app_header.bss_start % 4 != 0)
    {
        PRINT_ERROR("Misaligned Section Addresses\n");
        fclose(file);
        return -1;
    }

    // Проверка на секциите
    if (app_header.data_start > app_header.data_end ||
        app_header.bss_start > app_header.bss_end ||
        app_header.data_end > app_header.bss_start ||
        app_header.heap_size > MAX_HEAP_SIZE ||
        app_header.stack_size > MAX_STACK_SIZE)
    {
        PRINT_ERROR("Invalid section addresses or sizes\n");
        fclose(file);
        return -1;
    }

    // Извеждане на информация
    PRINT_PIC("APPLICATION\n");
    PRINT_PIC("\tMAGIC      : 0x%08X\n", app_header.magic);
    PRINT_PIC("\tVERSION    : 0x%08X\n", app_header.version);
    PRINT_PIC("\tENTRY      : 0x%08X\n", app_header.entry);
    PRINT_PIC("\tHEAP SIZE  : %d\n", app_header.heap_size);
    PRINT_PIC("\tSTACK SIZE : %d\n", app_header.stack_size);
    PRINT_PIC("\tRAM END    : %08X\n", app_header.ram_end);
    PRINT_PIC("\tDATA       : 0x%08X - 0x%08X, %d\n", app_header.data_start, app_header.data_end, app_header.data_end - app_header.data_start);
    PRINT_PIC("\tBSS        : 0x%08X - 0x%08X, %d\n", app_header.bss_start, app_header.bss_end, app_header.bss_end - app_header.bss_start);

    // Създаване на ROM, нормално е FLASH
    CPU.ROM_SIZE = file_size;
    CPU.ROM = malloc(CPU.ROM_SIZE);
    if (!CPU.ROM)
    {
        PRINT_ERROR("Failed to allocate ROM memory\n");
        fclose(file);
        return -1;
    }
    PRINT_PIC("ROM  SIZE: %d\n", CPU.ROM_SIZE);

    // Четене на ROM
    fseek(file, 0, SEEK_SET);
    if (fread(CPU.ROM, 1, CPU.ROM_SIZE, file) != CPU.ROM_SIZE)
    {
        PRINT_ERROR("Failed to read ROM\n");
        free(CPU.ROM);
        fclose(file);
        return -1;
    }

    if (fclose(file) != 0)
    {
        PRINT_ERROR("Failed to close file\n");
        free(CPU.ROM);
        return -1;
    }

    // Create RAM
    CPU.RAM_SIZE = app_header.ram_end - RAM_BASE;
    if (CPU.RAM_SIZE > MAX_RAM_SIZE)
    {
        PRINT_ERROR("RAM size too large: %zu\n", CPU.RAM_SIZE);
        free(CPU.ROM);
        return -1;
    }

    PRINT_PIC("RAM  SIZE: %d\n", CPU.RAM_SIZE);
    CPU.RAM = calloc(1, CPU.RAM_SIZE); // clear ALL RAM
    if (!CPU.RAM)
    {
        PRINT_ERROR("Failed to allocate RAM memory\n");
        free(CPU.ROM);
        return -1;
    }

    // Копиране на .data
    size = app_header.data_end - app_header.data_start;
    PRINT_PIC("DATA SIZE: %d\n", size);
    PRINT_PIC("BSS  SIZE: %d\n", app_header.bss_end - app_header.bss_start);
    memcpy(CPU.RAM, CPU.ROM + app_header.data_load, size);

    PRINT_PIC("DONE\n\n");
    return 0;
}

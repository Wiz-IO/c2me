
ROM_BASE = 0; /* CONST */
ROM_SIZE = DEFINED(ROM_SIZE) ? ROM_SIZE : 32768; /* LIMIT */

RAM_BASE = 0x10000000; /* CONST */
RAM_SIZE = DEFINED(RAM_SIZE) ? RAM_SIZE : 32768; /* LIMIT */

/* APP HEAP SIZE */
APP_HEAP_SIZE = DEFINED(APP_HEAP_SIZE) ? APP_HEAP_SIZE : 1024;  /* MINIMAL */
APP_HEAP_SIZE = APP_HEAP_SIZE > 2048 ? 2048 : APP_HEAP_SIZE;    /* LIMIT */

/* APP STACK SIZE */
APP_STACK_SIZE = DEFINED(APP_STACK_SIZE) ? APP_STACK_SIZE : 4096; /* MINIMAL */
APP_STACK_SIZE = (APP_STACK_SIZE > 2048) ? 4096 : APP_STACK_SIZE; /* LIMIT */

MEMORY
{
    ROM (rx)  : ORIGIN = ROM_BASE, LENGTH = ROM_SIZE
    RAM (rwx) : ORIGIN = RAM_BASE, LENGTH = RAM_SIZE
}

ENTRY(main)

SECTIONS
{

/*** ROM ***/

    . = ROM_BASE; /* 0 */

/*** HEADER ***/

    .initdata :
    {
        LONG(0xFECAFECA);               /* 00 APP MAGIC */
        LONG(0x10000000);               /* 01 API VERSION - RISC-V */
        LONG(ABSOLUTE(main));           /* 02 APP ENTRY */

        LONG(APP_HEAP_SIZE);            /* 03 APP HEAP size */
        LONG(APP_STACK_SIZE);           /* 04 APP STACK size */
        LONG(ABSOLUTE(RAM_END));        /* 05 STACK ADDRESS */

        LONG(ABSOLUTE(_data_load));     /* 06 ROM адреса на .data */
        LONG(ABSOLUTE(_data_start));    /* 07 copy(.data) */
        LONG(ABSOLUTE(_data_end));      /* 08 */

        LONG(ABSOLUTE(_bss_start));     /* 09 clear(.bss) */
        LONG(ABSOLUTE(_bss_end));       /* 10 */

        LONG(0);                        /* 11 */
        LONG(0);                        /* 12 */
        LONG(0);                        /* 13 */
        LONG(0);                        /* 14 */
        LONG(0);                        /* 15 */
    } > ROM                             /* HEADER: 64 bytes */

    .text : ALIGN(4)
    {
        *(.text .text.*)        /* Всички .text секции */
        *(.rodata .rodata.*)    /* Константи (read-only данни) */
        KEEP (*(.init))         /* Инициализационен код */
        KEEP (*(.fini))         /* Финализационен код */

        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(SORT(.preinit_array.*)))
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        PROVIDE_HIDDEN (__fini_array_start = .);
        *(SORT(.fini_array.*))
        *(.fini_array)
        PROVIDE_HIDDEN (__fini_array_end = .);
    } > ROM

/*** RAM ***/
    .data : ALIGN(4)
    {
        _data_start = .;
        *(.data .data.*)
        _data_end = .;
    } > RAM AT > ROM
    _data_load = LOADADDR(.data); /* copy ROM адреса на data */

    .bss (NOLOAD) : ALIGN(4)
    {
        _bss_start = .;
        *(.bss .bss.* COMMON)
        _bss_end = .;
    } > RAM

    .heap (NOLOAD) : ALIGN(4)
    {
        . = ALIGN(4);
        _heap_start = .;
        . += APP_HEAP_SIZE;
        . = ALIGN(4);
        _heap_end = .;
    } > RAM

    .stack (NOLOAD) : ALIGN(4)
    {
        . = ALIGN(4);
        _stack_start = .;
        . += APP_STACK_SIZE;
        . = ALIGN(4);
        _stack_end = .;
        RAM_END = .;
    } > RAM

    /* Премахване на ненужни секции */
    /DISCARD/ :
    {
        *(.got)
        *(.dynamic)
        *(.eh_frame)            /* Exception handling frame */
        *(.debug*)              /* Всички дебъг секции */
        *(.gnu*)                /* GNU-специфични метаданни */
        *(.comment)             /* Коментари от компилатора */
        *(.note*)               /* Бележки, напр. .note.GNU-stack */
        *(.group)               /* Групови секции */
        *(.symtab)              /* Таблица със символи (не е нужна за изпълнение) */
        *(.strtab)              /* Таблица с низове за символи */
        *(.shstrtab)            /* Таблица с имена на секции */
        *(.rela.debug*)         /* Релокации за дебъг секции */
        *(.line)                /* Информация за редове */
        *(.stab*)               /* Stabs дебъг формат */
        *(.interp*)             /* Интерпретатор (напр. /usr/lib/ld.so.1) */
    }
}

api = [
    "syscall",
    "putchar",

    "seconds",
    "millis",
    "micros",

    "delay",
    "delayMicroseconds",

    "digitalRead",
    "digitalWrite",
    "digitalToggle",

    "analogRead",
    "analogWrite",

# LIBC
    "memset",
    "memcpy",
    "memchr",
    "memcmp",
    "memmove",
    "strlen",
]

def strhash(p: str) -> int:
    h = 0
    if p:
        for c in p:
            h = 5527 * h + 7 * ord(c)
            v = h & 0x0000ffff
            h ^= v * v
    hex = (h & 0xFFFFFFFF)| 0xF0000000
    return f"{hex & 0xFFFFFFFF:08X}"

with open("RV/API_TABLE.h", "w") as f:
    for i in range(len(api)):
        f.write('\t{ 0x'+strhash(api[i]) + ', api_'+api[i] + '},\n')


with open("LIB/api.S", "w") as f:
    #f.write('.section .api_code, "ax", @progbits\n\n')
    for name in api:
        label = f"{name}"
        h = strhash(name)
        f.write(f'.globl {label}\n')
        #f.write(f'.section .text.{label}, "ax", @progbits\n')
        #f.write(f'.type {label}, %function\n')
        f.write(f'.func {label}\n')
        f.write(f'{label}:\n')
        f.write('    auipc t0, 0\n')
        f.write('    lw    t0, 12(t0)\n')
        f.write('    jalr zero, 0(t0)\n')
        f.write(f'   .word 0x{h}\t# {name}\n')
        f.write('.endfunc\n\n')

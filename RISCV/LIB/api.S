.globl syscall
.func syscall
syscall:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xF5400F94	# syscall
.endfunc

.globl putchar
.func putchar
putchar:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xF411B592	# putchar
.endfunc

.globl seconds
.func seconds
seconds:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xFB80DF68	# seconds
.endfunc

.globl millis
.func millis
millis:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xFAC4FB6A	# millis
.endfunc

.globl micros
.func micros
micros:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xFC76F7B6	# micros
.endfunc

.globl delay
.func delay
delay:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xFF55D5A2	# delay
.endfunc

.globl delayMicroseconds
.func delayMicroseconds
delayMicroseconds:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xFA50FC40	# delayMicroseconds
.endfunc

.globl digitalRead
.func digitalRead
digitalRead:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xF3501778	# digitalRead
.endfunc

.globl digitalWrite
.func digitalWrite
digitalWrite:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xF9AE709C	# digitalWrite
.endfunc

.globl digitalToggle
.func digitalToggle
digitalToggle:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xFD0D1078	# digitalToggle
.endfunc

.globl analogRead
.func analogRead
analogRead:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xF0A9DD60	# analogRead
.endfunc

.globl analogWrite
.func analogWrite
analogWrite:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xF0F099D4	# analogWrite
.endfunc

.globl memset
.func memset
memset:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xFAB4AAC4	# memset
.endfunc

.globl memcpy
.func memcpy
memcpy:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xFBF08EB4	# memcpy
.endfunc

.globl memchr
.func memchr
memchr:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xF6EAFE34	# memchr
.endfunc

.globl memcmp
.func memcmp
memcmp:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xFE85FD20	# memcmp
.endfunc

.globl memmove
.func memmove
memmove:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xF430C80C	# memmove
.endfunc

.globl strlen
.func strlen
strlen:
    auipc t0, 0
    lw    t0, 12(t0)
    jalr zero, 0(t0)
   .word 0xF8252192	# strlen
.endfunc


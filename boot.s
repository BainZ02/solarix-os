.set MULTIBOOT_MAGIC, 0x1BADB002
.set MULTIBOOT_FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

.section .multiboot
.align 4
.long MULTIBOOT_MAGIC
.long MULTIBOOT_FLAGS
.long CHECKSUM

.section .text
.global _start
.type _start, @function
_start:
    call kernel_main
    cli
.hang: hlt
    jmp .hang

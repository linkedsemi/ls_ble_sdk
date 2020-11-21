
SECTIONS
{
    .boot_ram :
    {
        KEEP(*(.text.boot_ram_start))
        *(.xip_banned .text* .rodata* .sdata* .data* .sbss* .bss* )
    }
    .got :
    {
        *(.got)
    }
    .got.plt :
    {
        *(.got.plt)
    }
}
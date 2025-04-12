MEMORY
{
    RAM : ORIGIN = 0x80000000, LENGTH = 4k
}

SECTIONS
{
    .text : {
        *(.text*)
    } > RAM
}
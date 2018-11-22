ENTRY(_start);

SECTIONS
{
    _start = 0x0F000000;
    . = 0x0F000000;
    .data : {
        *(.data)
    }
}

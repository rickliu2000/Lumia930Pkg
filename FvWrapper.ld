ENTRY(_start);

SECTIONS
{
    _start = 0x7CC00000;
    . = 0x7CC00000;
    .data : {
        *(.data)
    }
}

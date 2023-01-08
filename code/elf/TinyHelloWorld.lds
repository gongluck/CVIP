ENTRY(nomain)/*指定程序入口*/

SECTIONS
{
. = 0x08048000 + SIZEOF_HEADERS;/*指定程序入口指明程序开始的地址*/
tinytext : { *(.text) *(.data) *(.rodata) }/*将后面三段，合成当前一段*/
/DISCARD/ : { *(.comment) }
}
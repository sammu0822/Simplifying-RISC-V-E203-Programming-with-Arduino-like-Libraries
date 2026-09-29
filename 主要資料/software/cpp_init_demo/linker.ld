ENTRY(_start)

MEMORY {
	FLASH (rx) : ORIGIN = 0x20000000, LENGTH = 512K
	RAM   (rw) : ORIGIN = 0x80000000, LENGTH = 64K
}

SECTIONS {
	.text : {
		*(.text*)
		KEEP(*(.init_array))
	} > FLASH
	
	.data : {
		*(.data*)
	} > RAM AT > FLASH
	
	.bss : {
		*(.bss*)
	} > RAM
	
	. = ALIGN(4);
	PROVIDE(_stack_top = ORIGIN(RAM) + LENGTH(RAM));

}

.section .text
.global _start
.type _start, @function
_start:
	call __libc_init_array #呼叫 c++ global constructors
	call main	       #進入主程式
1:	j 1b		       #無限迴圈

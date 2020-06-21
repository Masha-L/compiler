	.text
	.globl main
main:
	 la $a0 _newline_ 
	 syscall 
	li $v0, 10
	syscall

	.data
	_newline_: .asciiz "\n"
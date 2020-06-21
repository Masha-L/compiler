.data 
_newline_: 
.asciiz "\n" 
.text 
.globl main 
main: 
addiu $sp, $sp, 0
addiu $sp, $sp, -40
sw $ra, 36($sp)
sw $fp, 32($sp)
sw $s0, 28($sp)
sw $s1, 24($sp)
sw $s2, 20($sp)
sw $s3, 16($sp)
sw $s4, 12($sp)
sw $s5, 8($sp)
sw $s6, 4($sp)
sw $s7, 0($sp)
addiu $fp, $sp, 40
addiu $sp, $sp, -4
addiu $sp, $sp, -4
li $t0, 0
move $s0, $t0
sw $s0, -44($fp)
li $t0, 0
move $s0, $t0
sw $s0, -48($fp)
addiu $sp, $sp, -4
li $t0, 1
move $s0, $t0
sw $s0, -52($fp)
addiu $sp, $sp, -4
li $t0, 2
move $s0, $t0
sw $s0, -56($fp)
lw $t0, -56($fp)
li $v0, 1
la $a0, 0($t0)
syscall 
addiu $sp, $sp, 0
lw $t1, -52($fp)
li $v0, 1
la $a0, 0($t1)
syscall 
addiu $sp, $sp, 0
li $v0, 4
la $a0, _newline_
syscall 
addiu $sp, $sp, 0
addiu $sp, $sp, 0
addiu $sp, $sp, 4
addiu $sp, $sp, -4
li $t2, 3
move $s0, $t2
sw $s0, -56($fp)
lw $t2, -44($fp)
li $v0, 1
la $a0, 0($t2)
syscall 
addiu $sp, $sp, 0
lw $t3, -56($fp)
li $v0, 1
la $a0, 0($t3)
syscall 
addiu $sp, $sp, 0
li $v0, 4
la $a0, _newline_
syscall 
addiu $sp, $sp, 0
addiu $sp, $sp, 0
addiu $sp, $sp, 4
lw $t4, -44($fp)
li $v0, 1
la $a0, 0($t4)
syscall 
addiu $sp, $sp, 0
lw $t5, -52($fp)
li $v0, 1
la $a0, 0($t5)
syscall 
addiu $sp, $sp, 0
li $v0, 4
la $a0, _newline_
syscall 
addiu $sp, $sp, 0
addiu $sp, $sp, 0
addiu $sp, $sp, 4
lw $t6, -44($fp)
li $v0, 1
la $a0, 0($t6)
syscall 
addiu $sp, $sp, 0
lw $t7, -48($fp)
li $v0, 1
la $a0, 0($t7)
syscall 
addiu $sp, $sp, 0
li $v0, 4
la $a0, _newline_
syscall 
addiu $sp, $sp, 0
addiu $sp, $sp, 0
addiu $sp, $sp, 8
.L0: 
lw $s7, 0($sp)
lw $s6, 4($sp)
lw $s5, 8($sp)
lw $s4, 12($sp)
lw $s3, 16($sp)
lw $s2, 20($sp)
lw $s1, 24($sp)
lw $s0, 28($sp)
lw $fp, 32($sp)
lw $ra, 36($sp)
addiu $sp, $sp, 0
addiu $sp, $sp, 40
jr $ra

	.text
	.align 2
	
main:
	subu $sp, $sp, 8
	sw $fp, 4($sp)
	sw $ra, 0($sp)
	addiu $fp, $sp, 4
	subu $sp, $sp, 4
	subu $gp, $gp, 8
	lw $ra, -4($fp)
	lw $fp, 0($fp)
	addu $sp, $sp, 4
	addu $sp, $sp, 8
	jr $ra
	
	.data
str:
	.asciiz "Output: "
newline:
	.asciiz "\n"

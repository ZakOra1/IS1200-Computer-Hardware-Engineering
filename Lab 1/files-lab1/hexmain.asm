  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0,17		# change this to test different values

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	

	move	$a0,$v0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window
	
stop:	j	stop		# stop after one run
	nop			# delay slot filler (just in case)

  # You can write your own code for hexasc here
  #
hexasc:
	
	ble $a0, 9, number	#if $a0 <= 9. go to number
	ble $a0, 15, char	#if $a0 <= 15 go to char
	j end
number:	
	addi $v0, $a0, 48	#adds 48 to $a0
	j end	
char:
	addi $v0, $a0, 55	#adds 55 to $a0
	
end: jr $ra
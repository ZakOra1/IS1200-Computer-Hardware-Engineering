  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0, 13		# change this to test different values

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
  	andi	$v0, $a0, 15	# "and" on the first byte to ignore rest of bytes if bigger then 15
  	slti	$t0, $v0, 10	# Check if its 0-9(=1) or 10-15(=0)
  	addi 	$v0, $v0, 0x30	# Write out correct ASCII code
  	beqz	$t0, stor 	# If $v0 10-15 branch
  	nop
  	j 	end
  	nop
  stor:
 	addi 	$v0, $v0, 7	# Compensate ASCII code, 7 other symbols between 9 and A in ASCII
  end:
  	jr	$ra		# Returns the value
  	nop			# t0 is only value not returned to same state as before subroutine

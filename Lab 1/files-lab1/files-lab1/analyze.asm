  # analyze.asm
  # This file written 2015 by F Lundevall
  # Copyright abandoned - this file is in the public domain.

	.text
main:
	li	$s0,0x30	#laddar register s0 med "0011 0000"
loop:
	move	$a0,$s0		# copy from s0 to a0
	
	li	$v0,11		# syscall with v0 = 11(=1011) will print out 
	syscall			# one byte(=8bits) from a0 to the Run I/O window

	addi	$s0,$s0,3	# what happens if the constant is changed? (s0 +1) Ändrar till s0 + 3
	
	li	$t0,0x5d	# Ändrar 5b till 5d, då den hoppar 3 steg varje gång (0101 1101)
	bne	$s0,$t0,loop	# if s0 inte är t0, branch (kör från loop)
	nop	
stop:	j	stop		# loop forever here
	nop			# delay slot filler (just in case)


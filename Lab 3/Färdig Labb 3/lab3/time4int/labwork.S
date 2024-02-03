  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
  
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm	

  #
  # Please add your own code below this line
  #
  
  # Beh�ver ej .text, d� jag ej har en .data tag och den �r d� automatiskt .text
  # (instruktioner och programmerings logik)
  # Anv�nder .data f�r att n� minneslagrad data.
  
.global hexasc	# S� subrutinerna kan n�s utanf�r programmet (accesible utanf�r)
  
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

.global delay

delay:
	PUSH $s0
	blez  	$a0, stop	# if a0 <= 0, stop (while (ms > 0)) (Safet prevention)
	nop
	li	$s0, 6569	# millisekunder (konstant)
	li	$t0, 0		# int i;
  loop:
  	slt	$t1, $t0, $s0	# for i < 4711
  	addi	$t0, $t0, 1	# i++;
  	bnez	$t1, loop	# loop if rad 96 st�mmer	
  	nop
  	
  	li	$t0, 0		# reset int i;
	addiu	$a0, $a0, -1	# ms = ms-1
	bnez  	$a0, loop
	nop
  stop:
  	POP $s0
 	jr $ra
	nop
.global time2string	

time2string:
	PUSH $ra		# Sparar return v�rdet 
	PUSH $s0
	PUSH $s1		# Sparar s0 s� att man kan �terst�lla den
	move	$s0, $a0	# ge s0 v�rdet av a0
	move	$s1, $a1
			
	sra	$a0, $a1, 12	# shift 12 steps to make the 4th LSB the new LSB	
	jal	hexasc
	nop
	sb	$v0, 0($s0)	# sparar x f�r (x*:** NULL)
			
	sra	$a0, $a1, 8	# shift 8 steps to make the 3th LSB the new LSB	
	jal	hexasc
	nop
	sb	$v0, 1($s0)	# sparar x f�r (*x:** NULL)
			
	addi 	$t0, $zero, 0x3A # Ger en adress v�rdet f�r ":" i ASCII
	sb	$t0, 2($s0)	# Sparar ":" f�r (xx:yy NULL)

	sra	$a0, $a1, 4	# shift 4 steps to make the 2th LSB the new LSB	
	jal	hexasc
	nop
	sb	$v0, 3($s0)	# sparar y f�r (**:y* NULL)
	
	sra	$a0, $a1, 0	# Skriver om v som a f�r att sedan f� LSB
	jal	hexasc
	nop
	sb	$v0, 4($s0)	# Sparar **:*y f�r (xx:yy NULL)
	
	addi 	$t0, $zero, 0x00 # Ger en adress v�rdet f�r "null" i ASCII
	sb	$t0, 5($s0)	# Sparar "null" f�r (xx:yy NULL)

 	POP $s1	
	POP $s0			# �terst�ller s0
	POP $ra			# �terst�ller ra s� man kommer till main
	jr	$ra
	nop

enable_interrupts:
	ei
	jr $ra
	nop

		  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0, 1000
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result  # Kan bara ha igång timern i ca 16^4 - 1 timmar (65 536-1 varv kan den göra). Räknar 65 536 - 1 timmar + 59:59 - 59:57 (För första varvet är bara 3 sek) (65 535 timmar och 2 sekunder)
	jr	$ra		# return
	nop

  # you can write your code for subroutine "hexasc" below this line
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

delay:
	PUSH ($s0)
	blez  	$a0, stop	# if a0 <= 0, stop (while (ms > 0)) (Safet prevention)
	nop
	li	$s0, 12	# millisekunder (konstant)
	li	$t0, 0		# int i;
  loop:
  	slt	$t1, $t0, $s0	# for i < 4711
  	addi	$t0, $t0, 1	# i++;
  	bnez	$t1, loop	# loop if rad 96 stämmer	
  	nop
  	li	$t0, 0		# reset int i;
	addiu	$a0, $a0, -1	# ms = ms-1
	bnez  	$a0, loop
	nop
  stop:
  	POP ($s0)
 	jr $ra
	nop
	

time2string:
	PUSH ($ra)		# Sparar return värdet 
	PUSH ($s0)		# Sparar s0 så att man kan återställa den
	move	$s0, $a0	# ge s0 värdet av a0
	
			
	sra	$a0, $a1, 12	# shift 12 steps to make the 4th LSB the new LSB	
	jal	hexasc
	nop
	sb	$v0, 0($s0)	# sparar x för (x*:** NULL)
			
	sra	$a0, $a1, 8	# shift 8 steps to make the 3th LSB the new LSB	
	jal	hexasc
	nop
	sb	$v0, 1($s0)	# sparar x för (*x:** NULL)
			
	addi 	$t0, $zero, 0x3A # Ger en adress värdet för ":" i ASCII
	sb	$t0, 2($s0)	# Sparar ":" för (xx:yy NULL)

	sra	$a0, $a1, 4	# shift 4 steps to make the 2th LSB the new LSB	
	jal	hexasc
	nop
	sb	$v0, 3($s0)	# sparar y för (**:y* NULL)
	
	sra	$a0, $a1, 0	# Skriver om v som a för att sedan få LSB
	jal	hexasc
	nop
	sb	$v0, 4($s0)	# Sparar **:*y för (xx:yy NULL)
	
	addi 	$t0, $zero, 0x00 # Ger en adress värdet för "null" i ASCII
	sb	$t0, 5($s0)	# Sparar "null" för (xx:yy NULL)
	
	
	POP ($s0)		# Återställer s0
	POP ($ra)		# Återställer ra så man kommer till main
	jr	$ra
	nop
	
	#a0 är addressen dä jag sparar
	#a1 är det som jag får in från klockan
	#storebyte efter hexasc till rätt plats i addresen

	

	
	

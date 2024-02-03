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
	li	$a0, 00
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
tiend:	sw	$t0,0($a0)	# save updated result  # Kan bara ha ig�ng timern i ca 16^4 - 1 timmar (65 536-1 varv kan den g�ra). R�knar 65 536 - 1 timmar + 59:59 - 59:57 (F�r f�rsta varvet �r bara 3 sek) (65 535 timmar och 2 sekunder)
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
	li	$s0, 1		# millisekunder (konstant)
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
  	POP ($s0)
 	jr $ra
	nop
	

time2string:
	PUSH ($ra)		# Sparar return v�rdet 
	PUSH ($s0)		# Sparar s0 s� att man kan �terst�lla den
	PUSH ($s1)
	move	$s0, $a0	# ge s0 v�rdet av a0
	move	$s1, $a1
		
############################Surprise assignment
	andi	$t0, $s1, 0xffff #Mask the 2 LSB's of the time
	bnez	$t0, notHour	# branchar d� det inte �r 00:00
	nop
	
	.data
  hour:	.ascii "HOUR"
  	.text
  	
	la	$t0, hour	# load:ar addressen till "HOUR"
	lw	$t1, ($t0)	# loadar $t1 med v�rdet p� addressen till "HOUR" (v�rdet: "HOUR"
	sw	$t1, ($s0)	# lagrar sedan v�rdet i addressen som skrivs ut, d� "HOUR" i ASCII best�r bara av 4 bytes.
	sb	$zero, 4($s0)	# l�gger till null efter alla bytes som formar "HOUR" f�r att inte skriva ut mer saker.
  	j	endTime
  	nop
############################
	
 notHour:		
	sra	$a0, $s1, 12	# shift 12 steps to make the 4th LSB the new LSB	
	jal	hexasc
	nop
	sb	$v0, 0($s0)	# sparar x f�r (x*:** NULL)
			
	sra	$a0, $s1, 8	# shift 8 steps to make the 3th LSB the new LSB	
	jal	hexasc
	nop
	sb	$v0, 1($s0)	# sparar x f�r (*x:** NULL)
			
	addi 	$t0, $zero, 0x3A # Ger en adress v�rdet f�r ":" i ASCII
	sb	$t0, 2($s0)	# Sparar ":" f�r (xx:yy NULL)

	sra	$a0, $s1, 4	# shift 4 steps to make the 2th LSB the new LSB	
	jal	hexasc
	nop
	sb	$v0, 3($s0)	# sparar y f�r (**:y* NULL)
	
	sra	$a0, $s1, 0	# Skriver om v som a f�r att sedan f� LSB
	jal	hexasc
	nop
	sb	$v0, 4($s0)	# Sparar **:*y f�r (xx:yy NULL)
	
	addi 	$t0, $zero, 0x00 # Ger en adress v�rdet f�r "null" i ASCII
	sb	$t0, 5($s0)	# Sparar "null" f�r (xx:yy NULL)
	
 endTime:
 	POP ($s1)	
	POP ($s0)		# �terst�ller s0
	POP ($ra)		# �terst�ller ra s� man kommer till main
	jr	$ra
	nop
	
	#a0 �r addressen d� jag sparar
	#a1 �r det som jag f�r in fr�n klockan
	#storebyte efter hexasc till r�tt plats i addresen

	

	
	

.text
	addi $a0,$0,5       
	add  $v0,$a0,$0  	
	beq  $a0,$0,guard
	
loop:
	addi $a0,$a0,-1
	beq  $a0,$0, done
	mul $v0, $v0, $a0
	beq  $0,$0 loop
	
guard:
	addi $v0,$0,1	  #Incase of factorial is 0
	
done:	
	add  $0,$0,$0	  # NOP	

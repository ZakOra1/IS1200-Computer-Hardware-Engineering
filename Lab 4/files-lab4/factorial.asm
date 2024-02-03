.text
	addi $a0,$0,7     # set $a0 to n   (Used as reference for counting down)
	add  $a1,$a0,$0   # set $a1 to $a0 (Used to iterate through inner loop
	add  $a2,$a0,$0   # set $a2 to $a0 (Used to add up) 
		#$a3 is also too be used (used as temporary register)
	
	beq  $a0,$0,guard   # Guard for 0
	beq  $a0,1,guard    # Guard for 1
	
loop: #(inner loop)
	addi $a1,$a1,-1   # Loops the function n-1 times, to get value n*(n-1).
	beq  $a1,$0, loop2
	add  $a3,$a3,$a2  # $a3 = r*(r-1) before exiting loop
	beq  $0,$0 loop
	
loop2: #(outer loop)
	addi $a0,$a0,-1	  # Keeps count of total outer loops, n= n-1
	addi $a1,$a0,0	  # Resets $a1 to $a0 again. 
	add  $v0,$0,$a3   # Adds the result from the innerloop
	add  $a3,$0,$0    # Resets $a3
	add  $a2,$v0,$0	  # Makes n = n*(n-1). Thereof the inner loops second result becomes n*(n-1) = (n*(n-1))*(n-2)
	beq  $a0,1, done  
	beq  $0,$0, loop
	
guard:
	addi $v0,$0,1	  #Incase of factorial is 0 or 1.
	
done:	
	add  $0,$0,$0	  # NOP	

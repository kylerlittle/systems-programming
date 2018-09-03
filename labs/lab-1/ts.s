# ts.s file:
	.global getebp
# points at stack frame of current executing function
getebp:	
	movl %ebp, %eax
	ret

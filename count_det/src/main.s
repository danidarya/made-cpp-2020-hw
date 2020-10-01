.text

.global count_det

count_det:
        ldr r1, [r0,#0]
	ldr r2, [r0,#16]
	ldr r3, [r0,#32]
	mul r2, r3, r2
	mul r1, r2, r1
        ldr r2, [r0,#4]
        ldr r3, [r0,#20]
	mul r2, r3, r2
	ldr r3, [r0,#24]
	mul r2, r3, r2
	add r1, r2, r1
	ldr r2, [r0,#8]
	ldr r3, [r0,#12]
	mul r2, r3, r2
	ldr r3, [r0,#28]
	mul r2, r3, r2
	add r1, r2, r1
	ldr r2, [r0,#8]
	ldr r3, [r0,#16]
	mul r2, r3, r2
	ldr r3, [r0,#24]
	mul r2, r3, r2
	sub r1, r1, r2
	ldr r2, [r0,#4]
	ldr r3, [r0,#12]
	mul r2, r3, r2
	ldr r3, [r0,#32]
	mul r2, r3, r2
	sub r1, r1, r2
	ldr r2, [r0,#0]
	ldr r3, [r0,#20]
	mul r2, r3, r2
	ldr r3, [r0,#28]
	mul r2, r3, r2
	sub r1, r1, r2
        mov r0, r1
	bx lr

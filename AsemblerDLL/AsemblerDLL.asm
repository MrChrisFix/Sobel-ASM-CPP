
.data
grayArrayStart QWORD 0		; pointer to the read array
readyArray QWORD 0			; pointer for the modified array
imageHeight DWORD 0			; the height of the image
imageWidth DWORD 0			; the width of the image

; Arguments (ints) passed from C++:
; 1 argument: RCX
; 2 argument: RDX
; 3 argument: R8
; 4 argument: R9
; all next arguments are on the stack

.code
Sobel proc
; save nonvolatile registers
push r12
push r13
push r14
push r15
push rdi
push rsi

;Load arguments
mov grayArrayStart, RCX
mov readyArray, RDX
mov imageHeight, R8d
mov imageWidth, R9d

mov edx, DWORD PTR[rbp + 48]
mov r15d, edx
mov edx, DWORD PTR[rbp + 56]
mov r14d, edx

; R14 always has whereToStart
; R15 always has bytesToCalculate

call Matrixes

TheEnd:
; get back nonvolatile registers
pop rsi
pop rdi
pop r15
pop r14
pop r13
pop r12

ret

Sobel endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Matrixes proc

	; Initialisation

	mov rsi, grayArrayStart
	mov rdi, readyArray

	mov ecx, r14d						; Counter for loop

	;Vertical mask
	mov r8, 0000FFFF0000FFFFh
	movq xmm10, r8						; first byte is on the right side
	mov r8, 0FFFF0000FFFF0000h
	pinsrq xmm10, r8, 1					; second byte is on the left side

	;Vertical negatin mask
	mov r8, 0000FFFF00000000h
	movq xmm11, r8						; first byte is on the right side
	mov r8, 0FFFF000000000000h
	pinsrq xmm11, r8, 1					; second byte is on the left side

	;Corection for vertical negation
	mov r8, 0000000000000003h
	movq xmm12, r8
	mov r8, 0000000000030000h
	pinsrq xmm12, r8, 1

	;Horizontal Mask
	mov r8, 0000FFFFFFFFFFFFh
	movq xmm13, r8						; first byte is on the right side
	mov r8, 0FFFFFFFFFFFF0000h
	pinsrq xmm13, r8, 1					; second byte is on the left side

	;Correction for horizontal negation
	mov r8, 0000000100010001h
	movq xmm14, r8
	mov r8, 0001000100010000h
	pinsrq xmm14, r8, 1

	;Horizontal *2 mask
	mov r8, 00000000FFFF0000h
	movq xmm15, r8						; first byte is on the right side
	mov r8, 0000FFFF00000000h
	pinsrq xmm15, r8, 1					; second byte is on the left side

	mov r11d, imageHeight
	dec r11d				; r11d = imageHeight-1

	mov r12d, imageWidth
	dec r12d;				; r12d = imageWidth-1
	mov r13d, r12d
	dec r13d				; r13d = imagewidth-2

	mov r9d, ecx						; use r9 for end of loop
	add r9d, r15d						; ^

	;Calculate the current row and column
	mov eax, ecx	; save the conter into eax
	xor edx,edx		; clean edx for division
	div imageWidth	; div to get modulo in edx.
	mov eax, ecx	; save the counter again
	sub eax, edx
	mov r10d, edx	; save the modulo for later
	xor edx,edx		; clean edx for division
	div imageWidth	; finally row is saved in eax

	; eax - row counter
	; r10d - column counter
	cmp eax, 0					;if(row ==0)
	jnz Initial1
	mov ebx, imageWidth
	sub ebx, r10d
	inc ebx
	add ecx, ebx				; ecx += imageWidth-column+1
	mov r10d, 1					; column = 1
	inc eax						; row++

Initial1:
	cmp r10d, 0
	jnz forloop
	inc r10d					;if(col==0) col=1



forloop:

	cmp r10d, r12d		
	jnz Test1					; col != imageWidth-1 -> jump
	add ecx, 2
	mov r10d, 1
	inc eax						;ecx+=2, col = 1, row++

Test1:

	cmp eax, r11d
	jge FunctionEnd				; row >= imageHeight-1

	cmp ecx, r9d			
	jge FunctionEnd				;i >= bytesToCalculate

	cmp r10d, r13d		
	jnz CalculationStart		; col != imageWidth-2 -> jump
	dec ecx
	dec r10d



CalculationStart:

	;Idea:
	;read the dword to xmm. then:
	;pmovzxbw -> vpbroadcastq -> copy to other xmm -> mask -> negation(pandn or pxor) 

	; xmm0 - read memory
	; xmm1 - xmm3 - rows 1-3 vertical
	; xmm4 - xmm6 - rows 1-3 horizontal
	; xmm7, xmm8 - helpers

	
	;Load data to xmm's
		mov r8d, ecx
		dec r8d
		movd xmm0, DWORD PTR[rsi+r8]		; r11d = i-1
		pmovzxbw xmm0, xmm0					; set bytes as words
		vpbroadcastq xmm0, xmm0				; copy first half into the second half
		vmovdqu	xmm2, xmm0					; copy info to xmm2 for vertical
		;Xmm5 is going to be empty


		sub r8d, imageWidth					; r11d = i-1-imagewidth
		movd xmm0, DWORD PTR[rsi+r8]		; save dword in xmm
		pmovzxbw xmm0, xmm0					; set bytes as words
		vpbroadcastq xmm0, xmm0				; copy first half into the second half
		vmovdqu	xmm1, xmm0					; copy info to xmm1 for vertical
		vmovdqu xmm4, xmm0					; copy info to xmm4 for horizontal

		add r8d, imageWidth
		add r8d, imageWidth					; r11d = i+imagewidth-1
		movd xmm0, DWORD PTR[rsi+r8]
		pmovzxbw xmm0, xmm0					; set bytes as words
		vpbroadcastq xmm0, xmm0				; copy first half into the second half
		vmovdqu	xmm3, xmm0					; copy info to xmm3 for vertical
		vmovdqu	xmm6, xmm0					; copy info to xmm6 for horizontal


;;;;;;;;;vertical
		paddw xmm2, xmm2					; multiply by 2
		;use the mask
		pand xmm1, xmm10					; use mask on 1st row
		pand xmm2, xmm10					; use mask on 2nd row
		pand xmm3, xmm10					; use mask on 3rd row

		;use the neg mask via xor
		;1st row
		pxor xmm1, xmm11					;Use negation mask
		;2nd row
		pxor xmm2, xmm11					;Use negation mask
		;3rd row
		pxor xmm3, xmm11					;Use negation mask

;;;;;;;;;horizontal
		; info : 2nd horizontal row is all zeros

		;1st row
		pand xmm4, xmm13					; use mask (1st row)
		movdqu xmm8, xmm4					; create copy for *2
		;3rd row
		pandn xmm6, xmm13					; use mask nad negate(3rd row)
		paddw xmm6, xmm14					; Correction
		movdqu xmm9, xmm6					; create copy for *2

		;1st row
		pand xmm8, xmm15					; use *2 mask on copy
		paddw xmm4, xmm8					; add copy to original

		;3rd row
		pand xmm9, xmm15					; use *2 mask on copy
		paddw xmm6, xmm9					; add copy to original

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


	; adding the regiesters
	;vertical
	paddw xmm1, xmm2
	paddw xmm1, xmm3
	paddw xmm1, xmm12							; Correction

	phaddw xmm1, xmm1							; horizontal add words
	pmovsxwd xmm1, xmm1							; set lower words as all dwords respecting the sign
	phaddd xmm1, xmm1							; horizontal add dwords


	;horizontal
	paddw xmm4, xmm6							; xmm5 is empty

	phaddw xmm4, xmm4							; horizontal add words
	pmovsxwd xmm4, xmm4							; set lower words as all dwords respecting the sign
	phaddd xmm4, xmm4							; horizontal add dwords

	;power of 2
	
	pmulld xmm1, xmm1

	pmulld xmm4, xmm4

	;squareroot
	paddd xmm1, xmm4				; sum

	cvtdq2ps xmm1, xmm1				; convert dwords to single precision floats

	sqrtps xmm1, xmm1				; square root of xmm1

	cvtps2dq xmm1, xmm1				; convert back to dwords

	movq r8, xmm1
	mov [rdi+rcx*4], r8				; move result to memory


	add ecx, 2						; 2 Bytes a time are calculated
	add r10d, 2
	jmp forloop
FunctionEnd:

ret
Matrixes endp	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Normalize proc
; save nonvolatile registers
push r12
push r13
push r14
push r15
push rdi
push rsi

; RCX -> pointer to array
; RDX -> normalized output array
; R8  -> minimum
; R9  -> maximum
; R14 -> bytesToCalculate
; R10 -> whereToStart -> counter for loop

mov r14d, DWORD PTR[rbp + 48]
mov r10d, DWORD PTR[rbp + 56]
mov r15, rdx					; move output array pointer to r15

add r14, r10					; r14 = bytesToCalculate + whereToStart

mov r11d, r9d
sub r11d, r8d					; r11d = diffenerce = max-min
jnz normalLoop		
inc r11d						; if (r11d == 0) r11d = 1 - preventiong division by 0

normalLoop:
	xor rdx,rdx
	mov r13d, DWORD PTR[rcx+r10*4]
	sub r13d, r8d				; r13d = Array[i]-minimum
	mov eax, r13d
	shl eax, 8					; eax*2^8 = eax*256
	sub eax, r13d				; eax = r13d*255 = r13d*(256-1) = r13d*256-r13d
	div r11d					; eax = r13d*255/r11d -> (Array[i]-minimum)*255/diffenerce

	mov BYTE PTR[r15+r10], al
	inc r10d
	cmp r10d, r14d				; i =?= bytesTocalculate
jnz normalLoop


; get back nonvolatile registers
pop rsi
pop rdi
pop r15
pop r14
pop r13
pop r12
ret
Normalize endp

end
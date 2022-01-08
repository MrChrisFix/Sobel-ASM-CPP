;ifndef X64
;.686p
;.xmm
;.model	flat, C
;endif

.data
grayArrayStart QWORD 0		; pointer to the read array
readyArray QWORD 0			; pointer for the modified array
helperArray QWORD 0			; pointer for a helper array
imageHeight DWORD 0			; the height of the image
imageWidth DWORD 0			; the width of the image

;TODO: thoose are non-global, delete them and put somewere else(stack/register)
bytesToCalculate DWORD 0	; amount of bytes the thread has to calculate
whereToStart DWORD 0		; offset of bytes where the thread should start in the grayArray

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
mov helperArray, R8
mov imageHeight, R9D

mov edx, DWORD PTR[rbp + 48]
mov imageWidth, edx

mov edx, DWORD PTR[rbp + 56]
mov bytesToCalculate, edx
mov r15d, edx				;TODO: delete
mov edx, DWORD PTR[rbp + 64]
mov whereToStart, edx		;TODO: delete
mov r14d, edx

; R14 always has whereToStart
; R15 always has bytesToCalculate


xor eax, eax ; clearing register
xor ebx, ebx ; clearing register
xor ecx, ecx ; clearing register

call Vertical
call Horizontal


mov rdi, readyArray			;GX Matrix
mov rsi, helperArray		;GY Matrix


mov ecx, 0					; counter for loop
mov eax, r15d				;calculate number of loops
mov r11, 4					; divide by 4 becouse dword is 4 bytes
div r11						; eax = nuber of loops; edx = rest

;mov r10d, ecx
mov r10d, r14d
imul r10d, 4
squareloop:

	;movdqa xmm0, OWORD PTR[rdi+r10]	; possible alterante instruction
	vmovdqa xmm0, OWORD PTR[rdi+r10]	;The error is possobly bc i try to read e.g 1/4 of the first int nad 3/4 of the second
	vmovdqa xmm1, OWORD PTR[rsi+r10]

	paddd xmm0, xmm1		; sum

	cvtdq2ps xmm0, xmm0		; convert dword to single precision float

	sqrtps xmm0, xmm0		; square root of xmm0

	cvtps2dq xmm0, xmm0		; convert back to dword

	movdqa OWORD PTR[rdi+r10], xmm0

	inc ecx
	add r10d, 16				
	cmp ecx, eax
	jnz squareloop

	;TODO: not finished, need to calculate the rest
	;pxor xmm0, xmm0
	;pxor xmm1, xmm1
;restLoop:
	;cmp rdx, 0
	;jz TheEnd

	;mov r8, DWORD PTR[rdi+r10]
	;mov r9, DWORD PTR[rsi+r10]

	;pinsrq xmm0, r8, 0
	;pinsrq xmm1, r9, 0


	;paddd xmm0, xmm1		; sum
	;cvtdq2ps xmm0, xmm0		; convert dword to single precision float
	;sqrtps xmm0, xmm0		; square root of xmm0
	;cvtps2dq xmm0, xmm0		; convert back to dword

	;movdqa OWORD PTR[rdi+r10], xmm0


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

Vertical proc ;use readyArray

;Registers:
; r14 - whereToStart
; r15 - bytesToCalculate
; rsi - grayArrayStart
; rdi - readyArray
; rax - current row
; rcx - loop couner
; r8 - temporary value
; r11 - register used in cmp
; r12d - ecx +/- 1
; r10d - ecx%imagewidth
; r14d - r12d +/- imageWidth
; r13b - byte holder for xmm insertion
; r9d - where to end in grayArray

; GXMATRIX:
;  1  0 -1
;  2  0 -2
;  1  0 -1

;Assingning the matrix to xmm1 in clokcwise order starting with up left corner

mov r8, 0FFFEFFFF00000001h			; heximal values for first 4 numbers 
pinsrq xmm1, r8, 0
mov r8, 000200010000FFFFh			; heximal values for last 4 numbers
pinsrq xmm1, r8, 1


mov rsi, grayArrayStart
mov rdi, readyArray


mov ecx, r14d						; Counter for loop

mov r9d, ecx						; use r9 for end of loop
add r9d, r15d						; ^

forloop:
	
	xorpd xmm0, xmm0 ; Clean xmm0 register

	;Calculate the current row
	mov eax, ecx	; save the conter into eax
	xor edx,edx		; clean edx for division
	div imageWidth	; div to get modulo in edx. <-SLOW?
	mov eax, ecx	; save the counter again
	sub eax, edx
	mov r10d, edx	; save the modulo for later
	xor edx,edx		; clean edx for division
	div imageWidth	; finally row is saved in eax

	
	cmp r10d, 0
	jnz ifelse
	; IF statement - left wall:
		mov r13b, BYTE PTR[rsi+rcx]		; Correction
		pinsrw xmm0, r13d, 7

		mov r12d, ecx					; r12d = i
		inc r12d						; i++

		mov r13b, BYTE PTR[rsi+r12]
		pinsrw xmm0, r13d, 3
		
		;if(row!=0)
		cmp eax, 0
		jz row1

		mov r13b, BYTE PTR[rsi+rcx]		; Correction
		pinsrw xmm0, r13d, 0

		mov r8d, r12d					;r8d = i+1
		sub r8d, imageWidth				;r8d = (i+1)-imageWidth
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d, 2

		row1:
		;if(row != imageHeight-1)
		mov r11d, imageHeight;
		dec r11d;
		cmp eax, r11d
		jz ifend

		mov r13b, BYTE PTR[rsi+rcx]		; Correction
		pinsrw xmm0, r13d, 6

		mov r8d, imageWidth				; r8d = imageWidth
		add r8d, ecx					; r8d = imageWidth+i
		inc r8d						; r8d = imageWidth+i+1
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d, 4

		jmp ifend

	ifelse:
		mov r11d, imageWidth
		dec r11d
		cmp r10d, r11d					;i%imagewidth =?= imageWidth-1
		jnz ifelse2

		; ELSEIF statement - right wall:
		mov r13b, BYTE PTR[rsi+rcx]		; Correction
		pinsrw xmm0, r13d, 3

		mov r12d, ecx					; r12d = i
		dec r12d						; r12d = i-1

		mov r13b, BYTE PTR[rsi+r12]
		pinsrw xmm0, r13d, 7

		;if(row!=0)
		cmp eax, 0
		jz row2

		mov r13b, BYTE PTR[rsi+rcx]		; Correction
		pinsrw xmm0, r13d, 2

		mov r8d, r12d					;r8d = i-1
		sub r8d, imageWidth				;r8d = (i-1)-imageWidth
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d, 0

		row2:
		;if(row != imageHeight-1)
		mov r11d, imageHeight;
		dec r11d;
		cmp eax, r11d
		jz ifend

		mov r13b, BYTE PTR[rsi+rcx]		; Correction
		pinsrw xmm0, r13d, 4

		mov r8d, imageWidth				;r8d = imageWidth
		add r8d, ecx					;r8d = imageWidth+i
		dec r8d							;r8d = imageWidth+i-1
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d ,6

		jmp ifend

	ifelse2:
		; ELSE statement - center:
		mov r12d, ecx					; r12d = i
		inc r12d						; i+1

		mov r13b, BYTE PTR[rsi+r12]
		pinsrw xmm0, r13d, 3
		
		sub r12d, 2						; r12d = i-1

		mov r13b, BYTE PTR[rsi+r12]
		pinsrw xmm0, r13d, 7

		;if(row!=0)
		cmp eax, 0
		jz row3

		mov r8d, r12d					;r8d = i-1
		sub r8d, imageWidth				;r8d = (i-1)-imageWidth
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d ,0

		add r8d, 2						;r8d = i+1-imageWidth
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d ,2

		row3:
		;if(row != imageHeight-1)
		mov r11d, imageHeight;
		dec r11d;
		cmp eax, r11d
		jz ifend

		mov r8d, imageWidth				;r8d = imageWidth
		add r8d, ecx					;r8d = imageWidth +i
		inc r8d							;r8d = i+1+imageWidth
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d ,4

		sub r8d,2						;r8d = i-1+imageWidth
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d ,6

ifend:
	
	pmaddwd	xmm0, xmm1	; multiply and add words into dwords
	phaddd xmm0, xmm0	; add 2 neighbouring dwords into one dword
	phaddd xmm0, xmm0	; ^ and now the sum of all words(bytes) is in the first dword of xmm0

	pextrd eax, xmm0, 0	; the sum is now in eax

	imul eax, eax		; calculate the power of 2 of the number for later

	mov [rdi+rcx*4], eax


	inc ecx				;increment counter
	cmp ecx, r9d
jnz forloop

ret
Vertical endp	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Horizontal proc		;use helperArray
;Registers:
; r14 - whereToStart
; r15 - bytesToCalculate
; rsi - grayArrayStart
; rdi - readyArray
; rax - current row
; rcx - loop couner
; r8 - temporary value
; r11 - register used in cmp
; r12d - ecx +/- 1
; r10d - ecx%imagewidth
; r14d - r12d +/- imageWidth
; r13b - byte holder for xmm insertion
; r9d - where to end in grayArray

;GYMATRIX:	
; 1  2  1
; 0  0  0
;-1 -2 -1
;Assingning the matrix to xmm1 in clokcwise order starting with up left corner
mov r8, 0000000100020001h			; heximal values for first 4 numbers 
pinsrq xmm1, r8, 0
mov r8, 0000FFFFFFFEFFFFh			; heximal values for last 4 numbers
pinsrq xmm1, r8, 1


mov rsi, grayArrayStart
mov rdi, helperArray


mov ecx, r14d						; Counter for loop

mov r9d, ecx						; use r9 for end of loop
add r9d, r15d						; ^

forloop:
	xorpd xmm0, xmm0 ; Clean xmm0 register

	;Calculate the current row
	mov eax, ecx	; save the conter into eax
	xor edx,edx		; clean edx for division
	div imageWidth	; div to get modulo in edx. <-SLOW?
	mov eax, ecx	; save the counter again
	sub eax, edx
	mov r10d, edx	; save the  i%imageWidth for later
	xor edx,edx		; clean edx for division
	div imageWidth	; finally row is saved in eax


	cmp eax, 0
	jnz ifelse
	; IF statement - top wall:
		mov r13b, BYTE PTR[rsi+rcx]		;Correction
		pinsrw xmm0, r13d, 1

		mov r8d, ecx					; r8d = i
		add r8d, imageWidth				; r8d = i+imageWidth
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d, 5
		
		;if(i%imageWidth!=0)
		cmp r10d, 0
		jz Hrow1

		dec r8d							; r8d = i-1+imageWidth
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d, 6

		mov r13b, BYTE PTR[rsi+rcx]		;Correction
		pinsrw xmm0, r13d, 0

		Hrow1:
		;if(i%imageWidth != imageWidth-1)
		mov r11d, imageWidth;
		dec r11d;
		cmp r10d, r11d
		jz ifend

		mov r8d, ecx					; r8d = i
		add r8d, imageWidth				; r8d = i+imageWidth
		inc r8d							; r8d = i+1+imageWidth
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d, 4

		mov r13b, BYTE PTR[rsi+rcx]		;Correction
		pinsrw xmm0, r13d, 2
		jmp ifend

	ifelse:
		mov r11d, imageHeight
		dec r11d
		cmp eax, r11d					;row =?= imageHeight-1
		jnz ifelse2

		; ELSEIF statement - bottom wall:
		mov r13b, BYTE PTR[rsi+rcx]		;Correction
		pinsrw xmm0, r13d, 5

		mov r8d, ecx					; r8d = i
		sub r8d, imageWidth				; r8d = i-imageWidth
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d, 1

		;if(i%imageWidth!=0)
		cmp r10d, 0
		jz Hrow2

		mov r13b, BYTE PTR[rsi+rcx]		;Correction
		pinsrw xmm0, r13d, 6

		dec r8d							; r8d = i-1-imageWidth
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d, 0

		Hrow2:
		;if(i%imageWidth != imageWidth-1)
		mov r11d, imageWidth;
		dec r11d;
		cmp r10d, r11d
		jz ifend

		mov r13b, BYTE PTR[rsi+rcx]		;Correction
		pinsrw xmm0, r13d, 4

		mov r8d, ecx					; r8d = i
		sub r8d, imageWidth				; r8d = i-imageWidth
		inc r8d							; r8d = i+1-imageWidth
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d, 2
		jmp ifend

	ifelse2:
		; ELSE statement - center:
		mov r8d, ecx
		add r8d, imageWidth				; r8d = i+imageWidth
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d, 5

		mov r12d, ecx
		sub r12d, imageWidth			; r12d = i-imageWidth
		mov r13b, BYTE PTR[rsi+r12]
		pinsrw xmm0, r13d, 1

		;if(i%imageWidth!=0)
		cmp r10d, 0
		jz Hrow3

		dec r12d						; r12d = i-1-imageWidth
		mov r13b, BYTE PTR[rsi+r12]
		pinsrw xmm0, r13d, 0

		dec r8d							; r8d = i-1+imageWidth
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d, 6

		Hrow3:
		;if(i%imageWidth != imageWidth-1)
		mov r11d, imageWidth;
		dec r11d;
		cmp r10d, r11d
		jz ifend

		mov r8d, ecx
		inc r8d
		add r8d, imageWidth				; r8d = i+1+imageWidth
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d, 4

		mov r8d, ecx
		inc r8d
		sub r8d, imageWidth				; r8d = i+1-imageWidth
		mov r13b, BYTE PTR[rsi+r8]
		pinsrw xmm0, r13d, 2


ifend:
	
	pmaddwd	xmm0, xmm1	; multiply and add words into dwords
	phaddd xmm0, xmm0	; add 2 neighbouring dwords into one dword
	phaddd xmm0, xmm0	; ^ and now the sum of all words(bytes) is in the first dword of xmm0

	pextrd eax, xmm0, 0	; the sum is now in eax

	imul eax, eax		; calculate the power of 2 of the number for later
	mov [rdi+rcx*4], eax

	inc ecx
	cmp ecx, r9d
	jnz forloop

ret
Horizontal endp

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
	cmp r10d, r14d
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
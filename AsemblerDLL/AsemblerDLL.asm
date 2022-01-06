;ifndef X64
;.686p
;.xmm
;.model	flat, C
;endif


; 1 argument: RCX
; 2 argument: RDX
; 3 argument: R8
; 4 argument: R9
; all next arguments are on the stack
.data
grayArrayStart QWORD 0		; pointer to the read array
readyArray QWORD 0			; pointer for the modified array
helperArray QWORD 0			; pointer for a helper array
imageHeight DWORD 0			; the height of the image
imageWidth DWORD 0			; the width of the image
bytesToCalculate DWORD 0	; amount of bytes the thread has to calculate
whereToStart DWORD 0		; offset of bytes where the thread should start in the grayArray


.code
Sobel proc

;Load arguments
mov grayArrayStart, RCX
mov readyArray, RDX
mov helperArray, R8
mov imageHeight, R9D

mov edx, DWORD PTR[rbp + 48]
mov imageWidth, edx

mov edx, DWORD PTR[rbp + 56]
mov bytesToCalculate, edx 
mov edx, DWORD PTR[rbp + 64]
mov whereToStart, edx

xor eax, eax ; clearing register
xor ebx, ebx ; clearing register
xor ecx, ecx ; clearing register

call Vertical
call Horizontal

;All below in this function is testing garbage

mov rax, grayArrayStart

mov bl, [rax]
mov cx, WORD PTR[grayArrayStart]

pxor xmm0,xmm0
pxor xmm1,xmm1

pmovzxbw xmm0, QWORD PTR[rax] ;put bytes as words in xmm

mov r8, 2
mov r9, 0

pinsrw xmm1, r8 ,0
pinsrw xmm1, r9 ,1
pinsrw xmm1, r9 ,2
pinsrw xmm1, r8 ,3
pinsrw xmm1, r8 ,4
pinsrw xmm1, r9 ,5
pinsrw xmm1, r8 ,6
pinsrw xmm1, r9 ,7

PMULLW xmm0, xmm1 ;multiply words and store lower 16-bits
xor rax, rax

; Quick info for myself:
; SobelGX & SobelGY have the max value of 1020 in one cell
; The calculated root can have 1400-1500 max in one cell
; but exponentiation and rooting can be a problem.
; For exponentation result I need DWORD, so it should be ok with readyArray, but
; just in case i would change the pointer in Sobel_ASM to __int32


ret

Sobel endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Vertical proc ;use readyArray

; save nonvolatile registers
push r12
push r13
push r14
push r15
push rdi

xor	r12,r12
xor r13,r13
xor r14,r14
xor r15,r15
;Registers:
; r8 - grayArrayStart
; r9 - readyArray
; ecx - loop couner
; r12d - ecx +/- 1
; r10d - ecx%imagewidth
; r14d - r12d +/- imageWidth
; r13b - byte holder for xmm insertion

; GXMATRIX:
;  1  0 -1
;  2  0 -2
;  1  0 -1

;Assingning the matrix to xmm1 in clokcwise order starting with up left corner

mov r8, 0FFFEFFFF00000001h			; heximal values for first 4 numbers 
pinsrq xmm1, r8, 0
mov r8, 000200010000FFFFh			; heximal values for last 4 numbers
pinsrq xmm1, r8, 1


mov r8, grayArrayStart
mov r9, readyArray



;pmovzxbw xmm0, QWORD PTR[r8] ;put bytes as words in xmm TEMPLATE

mov ecx, whereToStart				; Counter for loop

;TODO: Is this needed?
mov r15d, ecx						; use r15 for end of numbers
add r15d, bytesToCalculate			; needed to calculate in array

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
	div imageWidth	; final row saved in eax

	
	cmp r10d, 0
	jnz ifelse
	; IF statement - left wall:
		mov r12d, ecx					; r12d = i
		inc r12d						; i++

		mov r13b, BYTE PTR[r8+r12]
		pinsrw xmm0, r13d, 3
		
		;if(row!=0)
		cmp edx, 0
		jz row1
		mov r14d, r12d					;r14d = i+1
		sub r14d, imageWidth			;r14d = (i+1)-imageWidth
		mov r13b, BYTE PTR[r8+r14]
		pinsrw xmm0, r13d ,2
		row1:
		;if(row != imageHeight-1)
		mov r11d, imageHeight;
		dec r11d;
		cmp edx, r11d
		jz ifend
		mov r14d, imageWidth			;r14d = imageWidth
		add r14d, r12d					;r14d = imageWidth+(i+1)
		mov r13b, BYTE PTR[r8+r14]
		pinsrw xmm0, r13d ,4

		jmp ifend

	ifelse:
		mov r11d, imageWidth
		dec r11d
		cmp r10d, r11d					;i%imagewidth =?= imageHeight-1
		jnz ifelse2

		; ELSEIF statement - right wall:
		mov r12d, ecx
		dec r12d ;i-1

		mov r13b, BYTE PTR[r8+r12]
		pinsrw xmm0, r13d, 7

		;if(row!=0)
		cmp edx, 0
		jz row2
		mov r14d, r12d					;r14d = i-1
		sub r14d, imageWidth			;r14d = (i-1)-imageWidth
		mov r13b, BYTE PTR[r8+r14]
		pinsrw xmm0, r13d ,0
		row2:
		;if(row != imageHeight-1)
		mov r11d, imageHeight;
		dec r11d;
		cmp edx, r11d
		jz ifend
		mov r14d, imageWidth			;r14d = imageWidth
		add r14d, r12d					;r14d = imageWidth+(i-1)
		mov r13b, BYTE PTR[r8+r14]
		pinsrw xmm0, r13d ,6

		jmp ifend
	ifelse2:
		; ELSE statement - center:
		mov r12d, ecx					; r12d = i
		inc r12d						; i+1

		mov r13b, BYTE PTR[r8+r12]
		pinsrw xmm0, r13d, 3
		
		sub r12d, 2						; r12d = i-1

		mov r13b, BYTE PTR[r8+r12]
		pinsrw xmm0, r13d, 7

		;if(row!=0)
		cmp edx, 0
		jz row3
		mov r14d, r12d					;r14d = i-1
		sub r14d, imageWidth			;r14d = (i-1)-imageWidth
		mov r13b, BYTE PTR[r8+r14]
		pinsrw xmm0, r13d ,0

		add r12d, 2						;r12d +=2 -> r12d = i+1

		mov r14d, r12d					;r14d = i+1
		sub r14d, imageWidth			;r14d = (i+1)-imageWidth
		mov r13b, BYTE PTR[r8+r14]
		pinsrw xmm0, r13d ,2
		row3:
		;if(row != imageHeight-1)
		mov r11d, imageHeight;
		dec r11d;
		cmp edx, r11d
		jz ifend
		mov r14d, imageWidth			;r14d = imageWidth
		add r14d, r12d					;r14d = imageWidth+(i+1)
		mov r13b, BYTE PTR[r8+r14]
		pinsrw xmm0, r13d ,4

		sub r12d,2						;r12d = i-1

		mov r14d, imageWidth			;r14d = imageWidth
		add r14d, r12d					;r14d = imageWidth+(i-1)
		mov r13b, BYTE PTR[r8+r14]
		pinsrw xmm0, r13d ,6

ifend:
	
	pmullw xmm1, xmm0	;multiply
	;mov [r8], sum


	inc ecx ;increment counter
	cmp ecx, r15d
jnz forloop

; get back nonvolatile registers
pop r12
pop r13
pop r14
pop r15
pop rdi

ret
Vertical endp	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Horizontal proc

;use helperArray

ret
Horizontal endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Normalize proc



Normalize endp

end
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
grayArrayStart QWORD 0	; pointer
readyArray QWORD 0		; pointer
helperArray QWORD 0		; pointer
imageHeight DWORD 0
imageWidth DWORD 0
bytesToCalculate DWORD 0
whereToStart DWORD 0


.code
Sobel proc

;Ladowanie argumentow

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



xor rax, rax

ret

Sobel endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Vertical proc

;use readyArray


Vertical endp	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Horizontal proc

;use helperArray


Horizontal endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Normalize proc



Normalize endp

end
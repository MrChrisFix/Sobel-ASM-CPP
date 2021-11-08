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
imageHeight WORD 0
imageWidth WORD 0
bytesToCalculate WORD 0
whereToStart WORD 0
readyArray QWORD 0		; pointer



.code
FunkcjaASM proc

mov grayArrayStart, RCX
mov imageHeight, DX
mov imageWidth, R8W
mov bytesToCalculate, R9W
mov dx, WORD PTR[rbp + 48]
mov whereToStart, dx
;mov rdx, QWORD PTR[rbp + 64] 
;mov readyArray, rdx

xor rax, rax

ret

FunkcjaASM endp

end
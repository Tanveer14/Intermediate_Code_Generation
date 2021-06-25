.MODEL SMALL
.STACK 100H;initialize model and stack size
.DATA
a1_1 DB  ?; line no: 2
b1_1 DB  ?; line no: 2
c1_1 DB  ?; line no: 2
i1_1 DB  ?; line no: 2
t0 DB  ?

.CODE

PRINTLN PROC;procedure for printing
MOV AH, 2
ADD DL,30H 
INT 21H
RET
PRINTLN ENDP

main PROC
MOV AX,@DATA
MOV DS,AX


MOV AL,0
MOV b1_1,AL



MOV AL,1
MOV c1_1,AL



MOV AL,0
MOV i1_1,AL
startForL3:


MOV AL,i1_1
MOV BL,4		
CMP AL,BL
MOV t0,0
JNL L0
MOV t0,1
L0:
MOV AL,t0
CMP AL,0
JE endForL4


MOV AL,3
MOV a1_1,AL
startWhileL1:

MOV AL,a1_1
SUB AL,1
MOV a1_1, AL

MOV AL,a1_1
CMP AL,0
JE endWhileL2

MOV AL,b1_1
ADD AL,1
MOV b1_1, AL

JMP startWhileL1
endWhileL2:



MOV AL,i1_1
ADD AL,1
MOV i1_1, AL

JMP startForL3
endForL4:



MOV DL,a1_1
CALL PRINTLN



MOV DL,b1_1
CALL PRINTLN



MOV DL,c1_1
CALL PRINTLN


MOV AH,4CH
INT 21H
main ENDP

END MAIN

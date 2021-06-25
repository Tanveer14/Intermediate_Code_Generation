.MODEL SMALL
.STACK 100H;initialize model and stack size
.DATA
a1_1 DB  ?
b1_1 DB  ?
c1_1 DB  3 DUP(?)
t0 DB  ?
t1 DB  ?
t2 DB  ?
t3 DB  ?
t4 DB  ?

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




MOV AL,2
MOV BL,3
ADD AL,BL
MOV t0, AL

MOV AL,1
MOV BL,t0
MUL BL

MOV t1, AL


MOV AH,0
MOV AL,t1
MOV BL,3
DIV BL
MOV t2, AH

MOV AL,t2
MOV a1_1,AL




MOV AL,1
MOV BL,5		
CMP AL,BL
MOV t3,0
JNG L0
MOV t3,1
L0:

MOV AL,t3
MOV b1_1,AL


MOV BL,0
MOV BH,0;accessing array in based mode

MOV AL,8
MOV c1_1[BX],AL



MOV AL, a1_1
 MOV BL,b1_1
MOV CL,0
MOV t4,CL
CMP AL,0
JE L1
CMP BL,0 
JE L1
MOV CL,1
MOV t4,CL
L1:
MOV AL,t4
CMP AL,0
JE L2

MOV BL,0
MOV BH,0;accessing array in based mode
MOV AL,c1_1[BX]
ADD AL,1
MOV c1_1[BX], AL

L2:;line no 8




MOV BL,0
MOV BH,0;accessing array in based mode
MOV AL,c1_1[BX]
MOV a1_1,AL


MOV DL,a1_1
CALL PRINTLN



MOV DL,b1_1
CALL PRINTLN


MOV AH,4CH
INT 21H
main ENDP

END MAIN

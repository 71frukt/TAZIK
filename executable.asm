
main:

POP [BX] 
PUSH BX   
PUSH 1    
ADD      
POP BX   

PUSH AX 
PUSH 6

JMP fact: 
PUSH AX   
POP BX   
POP AX   
RET

fact:

POP [BX] 
PUSH BX   
PUSH 1    
ADD      
POP BX   

PUSH [AX + 0]
PUSH 0

JNE if_mark_0:

PUSH 0

PUSH AX   
POP BX   
POP AX   
RET
if_mark_0:

PUSH [AX + 0]
PUSH AX 
PUSH [AX + 0]
PUSH 1

SUB 
JMP fact: 
MUL 
PUSH AX   
POP BX   
POP AX   
RET

PUSH 0 
POP AX     
PUSH 0 
POP BX 
PUSH AX    
PUSH 0 
CALL main: 
SPU_OUT    
HLT    

main:

POP [BX] 
PUSH BX   
PUSH 1    
ADD      
POP BX   

PUSH AX   
PUSH 10

PUSH BX   
POP AX   
CALL fact: 
POP CX  
PUSH AX  
POP BX  
POP AX  
PUSH CX  
RET

fact:

POP [BX] 
PUSH BX   
PUSH 1    
ADD      
POP BX   

PUSH [AX + 0]
PUSH 1

JNE if_mark_0:

PUSH 1

RET
if_mark_0:

PUSH 0


POP [BX] 
PUSH BX   
PUSH 1    
ADD      
POP BX   

PUSH AX   
PUSH [AX + 0]
PUSH 1

SUB 
PUSH BX   
POP AX   
CALL fact: 
POP CX  
PUSH AX  
POP BX  
POP AX  
PUSH CX  
POP [AX + 1]
var 'next'   num = 1

PUSH [AX + 1]
PUSH [AX + 0]
MUL 
RET

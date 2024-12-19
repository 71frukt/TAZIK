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
PUSH 3

PUSH 6

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


POP [BX] 
PUSH BX   
PUSH 1    
ADD      
POP BX   

PUSH [AX + 1]
PUSH 3

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
PUSH 6

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
POP [AX + 2]

PUSH [AX + 2]
PUSH [AX + 0]
MUL 
RET

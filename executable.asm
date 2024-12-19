
main:
POP [BX] 
PUSH BX   
PUSH 1    
ADD      
POP BX   

PUSH 9
PUSH 2
ADD 
POP [AX + 1]
PUSH BX   
PUSH 1    
ADD      
POP BX   

PUSH 5
PUSH [AX + 1]
ADD 
POP [AX + 2]
PUSH BX   
PUSH 1    
ADD      
POP BX   

PUSH [AX + 2]
PUSH 9
ADD 
POP [AX + 3]
PUSH BX   
PUSH 1    
ADD      
POP BX   

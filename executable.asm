PUSH 0                     
POP AX                   

PUSH 0                     
POP BX                   

PUSH AX                    
PUSH 0                   

CALL GOIDA:   
SPU_OUT                    
HLT                        

	; инициализация функции 'GOIDA'   
GOIDA:

	; инициализация переменной 'a'   

POP [BX] 
PUSH BX   
PUSH 1    
ADD      
POP BX   

	; вызов функции 'fact'   
PUSH AX   
	; передача аргументов в функцию 
SPU_IN
PUSH BX   
POP AX   
CALL fact: 
POP CX  
PUSH AX  
POP BX  
POP AX  
	PUSH CX  
	; конец вызова функции 'fact'   
RET
	; конец инициализации функции 'GOIDA'   

	; инициализация функции 'fact'   
fact:

	; инициализация переменной 'n'   

POP [BX] 
PUSH BX   
PUSH 1    
ADD      
POP BX   

	; начало цикла if
PUSH [AX + 0]
PUSH 1

JA if_mark_0:

PUSH 1

RET
if_mark_0:
	; конец цикла if

PUSH [AX + 0]
	; вызов функции 'fact'   
PUSH AX   
	; передача аргументов в функцию 
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
	; конец вызова функции 'fact'   
MUL 
RET
	; конец инициализации функции 'fact'   

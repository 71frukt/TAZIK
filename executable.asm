PUSH 0                     
POP AX                   

PUSH 0                     
POP BX                   

PUSH AX                    
PUSH 0                   

CALL main:   
SPU_OUT                    
HLT                        

	; инициализация функции 'main'   
main:

	; инициализация переменной 'arg'   

POP [BX] 
PUSH BX   
PUSH 1    
ADD      
POP BX   

	; вызов функции 'fact'   
PUSH AX   
	; передача аргументов в функцию 
PUSH 6

PUSH 2

MUL 
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
	; конец инициализации функции 'main'   

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

JNE if_mark_0:

PUSH 1

RET
if_mark_0:
	; конец цикла if

	; инициализация переменной 'next'   
PUSH 0


POP [BX] 
PUSH BX   
PUSH 1    
ADD      
POP BX   

	; присваивание значения переменной 'next'
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
POP [AX + 1]

PUSH [AX + 1]
PUSH [AX + 0]
MUL 
RET
	; конец инициализации функции 'fact'   

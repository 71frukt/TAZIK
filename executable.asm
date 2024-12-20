PUSH 0                     
POP AX                   

PUSH 0                     
POP BX                   

PUSH AX                    
PUSH 0                   

CALL ГОЙДА:   
SPU_OUT                    
HLT                        

	; инициализация функции 'ГОЙДА'   
ГОЙДА:

	; инициализация переменной 'Н'   

POP [BX] 
PUSH BX   
PUSH 1    
ADD      
POP BX   

	; вызов функции 'Бум'   
PUSH AX   
	; передача аргументов в функцию 
SPU_IN
PUSH BX   
POP AX   
CALL Бум: 
POP CX  
PUSH AX  
POP BX  
POP AX  
	PUSH CX  
	; конец вызова функции 'Бум'   
RET
	; конец инициализации функции 'ГОЙДА'   

	; инициализация функции 'Бум'   
Бум:

	; инициализация переменной 'Пункт'   

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
RET

PUSH [AX + 0]
PUSH 1

SUB 
	; конец инициализации функции 'Бум'   

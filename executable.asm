PUSH 0                     
POP AX                   

PUSH 0                     
POP BX                   

PUSH AX                    
PUSH 0                   

CALL GOIDA:   
SPU_OUT                    
HLT                        

	; ������������� ������� 'GOIDA'   
GOIDA:

	; ������������� ���������� 'a'   

POP [BX] 
PUSH BX   
PUSH 1    
ADD      
POP BX   

	; ����� ������� 'fact'   
PUSH AX   
	; �������� ���������� � ������� 
SPU_IN
PUSH BX   
POP AX   
CALL fact: 
POP CX  
PUSH AX  
POP BX  
POP AX  
	PUSH CX  
	; ����� ������ ������� 'fact'   
RET
	; ����� ������������� ������� 'GOIDA'   

	; ������������� ������� 'fact'   
fact:

	; ������������� ���������� 'n'   

POP [BX] 
PUSH BX   
PUSH 1    
ADD      
POP BX   

	; ������ ����� if
PUSH [AX + 0]
PUSH 1

JA if_mark_0:

PUSH 1

RET
if_mark_0:
	; ����� ����� if

PUSH [AX + 0]
	; ����� ������� 'fact'   
PUSH AX   
	; �������� ���������� � ������� 
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
	; ����� ������ ������� 'fact'   
MUL 
RET
	; ����� ������������� ������� 'fact'   


PUSH 5
PUSH 6
ADD 

PUSH 8
PUSH 9
SUB 

PUSH 5
POP [0]

while_start_mark_0:
PUSH 5
PUSH 6
ADD 
PUSH 8
JE while_end_mark_0:

PUSH [0]
PUSH 56
JNE if_mark_0:

PUSH 5
PUSH 602
ADD 
if_mark_0:

PUSH 9
PUSH 8
SUB 
JMP while_start_mark_0:  
while_end_mark_0:       

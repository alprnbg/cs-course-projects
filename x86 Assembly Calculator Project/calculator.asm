code segment
  mov bx,0h
  mov ax,0h
  mov cx,0h
  mov dx,0h ; all registers are 0
  mov si,0h ; si register will be used to check whether the last input is number or operation
            ; if si = 0, last input was an operation. Otherwise last input was a number.
  jmp read ; jumps to read

letter_input: ; converts letter input to the its integer value
  sub al,"A"
  add al, 10d
  jmp read_cont

pushvalue:
 cmp si, 0h ; if the last input was an operation, go back to read
 je read
 push bx ; push the last given input to the stack
 mov bx,0h ; make bx 0, because it is used in taking input
 jmp read

addition:
  mov si,0h
  pop ax ; take the top of stack
  pop bx ; take the top of stack
  add ax,bx ; add them
  push ax ; push it to stack
  mov bx, 0h
  jmp read

multiplication:
  mov si,0h
  mov dx,0h ; make dx zero, just in case
  pop ax ; take the top of stack
  pop bx ; take the top of stack
  mul bx ; multiply them
  push ax ; push back to stack
  mov bx, 0h
  jmp read

division:
  mov si,0h
  mov dx,0h ; make dx zero, just in case
  pop bx ; take the top of stack
  pop ax ; take the top of stack
  div bx ; divide ax by bx
  push ax ; push the result to the stack
  mov bx, 0h
  jmp read

read:
  mov ah,01h  ; to take input
  int 21h     ; takes input
  cmp al,2Bh  ; compare with +
  je addition
  cmp al,20h  ; compare with ' '
  je pushvalue
  cmp al,0Dh  ; compare with enter
  je print
  cmp al,2Ah  ; compare with *
  je multiplication
  cmp al,2Fh  ; compare with /
  je division
  cmp al,5Eh  ; compare with ^
  je myxor
  cmp al,26h  ; compare with &
  je myand
  cmp al,7Ch  ; compare with |
  je myor
  mov si,01h  ; input is a number 
  cmp al,40h  ; compare with 40h to understand of the input is a letter or number
  jg letter_input
  sub al,'0'  ; if it is not a letter, subtract ascii value of 0 to convert its value
  jmp read_cont

read_cont:
  mov cx,0h
  mov cl,al  ; put the input in cl
  mov ax,10h
  mul bx     ; if the given input is digit of a large number, multiply the first digits with 10h, if it is not the result will be 0
  add cx,ax  ; then add the input to the result of multiplication
  mov bx,cx  ; move the input to bx
  jmp read   ; back to read

myxor:
  mov si,0h
  pop ax ; take the top of stack
  pop bx ; take the top of stack
  xor ax,bx ; xor operation
  push ax ; push the result in stack
  mov bx, 0h
  jmp read

myand:
  mov si,0h
  pop ax ; take the top of stack
  pop bx ; take the top of stack
  and ax,bx ; and operation
  push ax ; push the result in stack
  mov bx, 0h
  jmp read

myor:
  mov si,0h
  pop ax ; take the top of stack
  pop bx ; take the top of stack
  or ax,bx ; or operation
  push ax ; push the result in stack
  mov bx, 0h
  jmp read

print:
  mov ah,02h ; for printing
  mov dl, 10d ; new line
  int 21h ; print new line
  pop ax ; pop top of the stack
  mov bx,10h ; for dividing
  mov cx,0h
  mov dx,0h
  div bx ; divide the number to 10d in order to take the last digit of it seperately
  push dx ; push the remainder -last digit of num- to stack
  inc cx ; c++, to know number of digits that will be in stack
  cmp ax,0h ; if the quotient is 0, there is nothing more to divide, jump to finish
  je print_finish
  mov dx,0h ; make it 0, just in case
  div bx ; same process again, if the quotient is not 0
  push dx
  inc cx
  cmp ax,0h
  je print_finish
  mov dx,0h
  div bx ; same process again, if the quotient is not 0
  push dx
  inc cx
  cmp ax,0h
  je print_finish
  push ax ; if there is a last digit, push it to stack
  inc cx
  jmp print_finish

print_finish:
  mov ah,02h ; to print
  pop dx ; pop the top of the stack, first digit of the result
  cmp dl,9h ; if it is a letter
  jg letter_output
  add dl, '0'

print_cont:
  int 21h ; print it
  dec cx ; c-- , c is the number of digits in stack
  jnz print_finish ; if c is not zero, same process again
  int 20h ; quit

letter_output: ; if the output is a letter convert it to ascii
  sub dl, 10d
  add dl, "A"
  jmp print_cont

code ends

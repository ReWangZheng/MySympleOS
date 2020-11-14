extern CInterrupt_0
global Interrupt_0
Interrupt_0 :
    call CInterrupt_0
    mov al,20h
    mov dx,20h
    out dx,al
    iret
;global start
;section .text
use32
    mov edx, esp
    push 0x00
    push 0x33
    push DWORD sixtyfour_bit - eip_val ; distance from call 
    mov ebx, esp
    call get_eip
eip_val:
    add DWORD [ebx], eax
enter_64:
    jmp far ptr ebx
exit:
    jmp _end
sixtyfour_bit:
use64
    
    push rdx ; secondary saved stack pointer
    and rdi, 0xFF
    and rsi, 0xFF
    add edi, ecx ; at this point rdi = xored character, rsi = input byte, ecx = index
    mov ecx, 0x180001
    xor dil, [ecx]
    cmp dil, sil
    mov rdx, 0x1
    cmove rdi, rdx
    mov rdx, 0x0
    cmovne rdi, rdx

    pop rdx
    sub WORD [ebx + 0x4], 0x10
    sub DWORD [ebx], sixtyfour_bit - exit ; increment past the jmp to 64
    jmp far ptr ebx
use32
get_eip:
    mov eax, DWORD [esp]
    ret
_end:
    mov esp, edx
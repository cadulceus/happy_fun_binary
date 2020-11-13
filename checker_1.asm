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
    
    push rdx
    mov rax, 0xFFFFFFFF ; ensure args from c to
    and rdi, rax
    push rdi ; offset into happy_fun_binary
    and rsi, 0xFF
    push rsi ; input byte

    mov rax, 0x00007972 ; prep for syscalls by loading "./happy_fun_binary" onto stack
    pushq rax
    mov rax, 0x616e69625f6e7566
    pushq rax
    mov rax, 0x5f79707061682f2e
    pushq rax

    mov     rax, 2                   ; open syscall number for 64 bit
    mov     rdi, rsp  
    mov     rsi, 0x0                 ; O_RDONLY
    mov     rdx, 0x0
    syscall

    mov     r8, rax
    mov     rax, 9                   ; mmap syscall number for 64 bit
    mov     rdi, 0x180000
    mov     rsi, 0x2000              ; size of mapping
    mov     rdx, 0x5                 ; rwp
    mov     r10, 0x2                 ; MAP_PRIVATE
    mov     r9, 0x0
    syscall
;    mov     rax, 1                   ; basically print
;    mov     rdi, 1
;    mov     rsi, 0x180000             
;    mov     rdx, 0x20
;    syscall

    add esp, 0x18 ; no need for the happy_fun_binary string anymore

    pop rsi
    pop rdi
    cmp sil, [rdi + 0x180000]
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
    bits 64
    extern malloc, puts, printf, fflush, abort, free
    global main

    section   .data
empty_str: db 0x0
int_format: db "%ld ", 0x0
data: dq 4, 8, 15, 16, 23, 42, 1
data_length: equ ($-data) / 8

    section   .text
    
;;; print_int proc
print_int:
    push rbp
    mov rbp, rsp
    sub rsp, 16

    mov rsi, rdi
    mov rdi, int_format
    xor rax, rax
    call printf

    xor rdi, rdi
    call fflush

    mov rsp, rbp
    pop rbp
    ret

;;; p proc
p:
    mov rax, rdi
    and rax, 1
    ret

;;; add_element proc
add_element:
    push rbp
    push rbx
    push r14
    mov rbp, rsp
    sub rsp, 16

    mov r14, rdi
    mov rbx, rsi

    mov rdi, 16
    call malloc
    test rax, rax
    jz abort

    mov [rax], r14
    mov [rax + 8], rbx

    mov rsp, rbp
    pop r14
    pop rbx
    pop rbp
    ret

;;; m proc
m:
    push rbp
    mov rbp, rsp
    sub rsp, 16

    test rdi, rdi
    jz outm

    push rbp
    push rbx

    mov rbx, rdi
    mov rbp, rsi

    mov rdi, [rdi]
    call rsi

    mov rdi, [rbx + 8]
    mov rsi, rbp
    call m

    pop rbx
    pop rbp

outm:
    mov rsp, rbp
    pop rbp
    ret

;;; f proc
f:
    mov rax, rsi

    test rdi, rdi
    jz outf

    push rbx
    push r12
    push r13

    mov rbx, rdi
    mov r12, rsi
    mov r13, rdx

    mov rdi, [rdi]
    call rdx
    test rax, rax
    jz z

    mov rdi, [rbx]
    mov rsi, r12
    call add_element
    mov rsi, rax
    jmp ff

z:
    mov rsi, r12

ff:
    mov rdi, [rbx + 8]
    mov rdx, r13
    call f

    pop r13
    pop r12
    pop rbx

outf:
    ret








;;; m2 proc 
; Работает аналогично m proc
; но реализован не через рекурсивный вызов, а итерационно
m2:
    push rbp
    mov rbp, rsp
    sub rsp, 16

    push rbp
    mov rbp, rsi
    
    
m2_loop:
    test rdi, rdi
    jz outm2

    push rbx
    mov rbx, rdi

    mov rdi, [rdi]
    call rbp

    mov rdi, [rbx + 8]
    pop rbx

    jmp m2_loop

outm2:

    pop rbp
    
    mov rsp, rbp
    pop rbp
    ret









;;; f2 proc 
; Работает аналогично f proc
; но реализован не через рекурсивный вызов, а итерационно
f2:
    mov rax, rsi
    
    push rbx
    push r12
    push r13

f2_loop:
    test rdi, rdi
    jz outf2

    mov rbx, rdi
    mov r12, rsi
    mov r13, rdx

    mov rdi, [rdi]
    call rdx
    test rax, rax
    jz z2

    mov rdi, [rbx]
    mov rsi, r12
    call add_element
    mov rsi, rax
    jmp ff2

z2:
    mov rsi, r12
    mov rax, rsi
    
ff2:
    mov rdi, [rbx + 8]
        
    mov rdx, r13
     
    jmp f2_loop

outf2:

    pop r13
    pop r12
    pop rbx    
    
    ret






;;; delete_list proc
; Освобождает память, занимаемую элементами списка
; 1 аргумент - rdi - указатель на первый элемент удаляемого списка
delete_list:
     
    push rbx

    mov rbx, rdi ; rbx <== first

delete_list_loop:
    test rbx, rbx 		; while (first != NULL) {
    jz delete_list_outf

    mov rdi, rbx 		; struct list_item * del = first;	
    mov rbx, [rbx + 8] 		; first = first-> next;
    call free			; 	free(del);
     
    jmp delete_list_loop	; }

delete_list_outf:
    
    pop rbx    
    
    ret








;;; main proc
main:


    push rbx
    push r12
    push r13

    xor rax, rax ; rax <== указатель на голову списка, куда добавляются элементы. Изначально = NULL 
    mov rbx, data_length
adding_loop:
    mov rdi, [data - 8 + rbx * 8]
    mov rsi, rax
    call add_element
    dec rbx
    jnz adding_loop

    mov rbx, rax ; rbx <== Указатель на голову исходного списка


;  печать исходного списка
    mov rdi, rax
    mov rsi, print_int
    call m2

    mov rdi, empty_str
    call puts



; фильтрация исходного списка
; Создается список, содержащий только нечетные значения
    mov rdx, p
    xor rsi, rsi
    mov rdi, rbx
    call f2
    
    
    
    mov r12, rax ; r12 <== Указатель на голову отфильтрованного списка 
    
;  печать отфильтрованного списка
    mov rdi, rax
    mov rsi, print_int
    call m2

    mov rdi, empty_str
    call puts


;  повторная печать исходного списка
    mov rdi, rbx
    mov rsi, print_int
    call m2

    mov rdi, empty_str
    call puts

; повторная печать отфильтрованного списка
    mov rdi, r12
    mov rsi, print_int
    call m2

    mov rdi, empty_str
    call puts



    mov rdi, rbx 	   	; delete_list (исходный список)
    call delete_list	    
    xor rbx, rbx 		; исходный список = NULL



    mov rdi, r12	   	; delete_list (отфильтрованный список)
    call delete_list	    
    xor r12, r12 		; отфильтрованный список = NULL



;  повторная печать исходного списка
    mov rdi, rbx
    mov rsi, print_int
    call m2

    mov rdi, empty_str
    call puts

; повторная печать отфильтрованного списка
    mov rdi, r12
    mov rsi, print_int
    call m2

    mov rdi, empty_str
    call puts
    
    
    pop r13
    pop r12
    pop rbx

    xor rax, rax
    ret
    
    

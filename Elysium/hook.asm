EXTERN ktrue_o: QWORD ; ktrue_o is a pointer (8 bytes on x64), not a PROC

.code

ktrue_hook PROC
    ; Store return address
    pop rax

    ; Store original return address in a non-volatile register (r12 here)
    mov r12, rax

    ; Call original function
    push rax
    call QWORD PTR [ktrue_o] ; Dereference ktrue_o when calling

    ; Set return value to true
    mov rax, 1

    ; Restore return address
    push r12
    ret
ktrue_hook ENDP

END

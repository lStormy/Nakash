.global switch_point
.global callable_context_start
.global switch_to_callable_context
.global switch_to_context

switch_to_callable_context:
    pushq %rbp
    movq %rsp, %rbp
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    movq %rsp, (%rdi) 
    movq %rbp, (%rdx)
    jmp switch_point

callable_context_start:
    movq %r13, %rdi 
    callq *%r12 
    movq (%rbx), %rsi 
    jmp switch_point

switch_to_context:
    pushq %rbp
    movq %rsp, %rbp
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    movq %rsp, (%rdi) 

switch_point:
    movq %rsi, %rsp
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbx
    popq %rbp
    retq

#include "stack_context.h"
#include <stdlib.h>
static void * ensure_alignment(void * stack, size_t stack_size)
{
    static const size_t CONTEXT_STACK_ALIGNMENT = 16;
    unsigned char * stack_top = static_cast<unsigned char *>(stack) + stack_size;
    // if the user gave me a non-aligned stack, just cut a couple bytes off from the top
    return stack_top - reinterpret_cast<size_t>(stack_top) % CONTEXT_STACK_ALIGNMENT;
}
 
stack_context::stack_context(void * stack, size_t stack_size, void (* function)(void *), void * arg)
    : caller_stack_top(nullptr), my_stack_top(nullptr)
    , rbp_on_stack(nullptr)
{
    unsigned char * math_stack = static_cast<unsigned char *>(ensure_alignment(stack, stack_size));
    my_stack_top = math_stack - sizeof(void *) * 9;
    void ** initial_stack = static_cast<void **>(my_stack_top);
    // store the return address here to make the debuggers life easier
    asm("movq $switch_point, %0\n\t" : : "m"(initial_stack[8]));
    initial_stack[7] = nullptr; // will store rbp here to make the debuggers life easier
    asm("movq $callable_context_start, %0\n\t" : : "m"(initial_stack[6]));
    rbp_on_stack = initial_stack[5] = &initial_stack[7]; // initial rbp
    initial_stack[4] = &caller_stack_top; // initial rbx
    initial_stack[3] = reinterpret_cast<void *>(function); // initial r12
    initial_stack[2] = arg; // initial r13
    initial_stack[1] = nullptr; // initial r14
    initial_stack[0] = nullptr; // initial r15
}

void stack_context::switch_into()
{
    asm("call switch_to_callable_context"
        : : "D"(&caller_stack_top), "S"(my_stack_top), "d"(rbp_on_stack));
}
void stack_context::switch_out_of()
{
    asm("call switch_to_context"
        : : "D"(&my_stack_top), "S"(caller_stack_top));
}
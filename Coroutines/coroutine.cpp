#include "coroutine.h"
#include <iostream>

void coro::detail::coroutine::yield() {
    this->stack_ctx->switch_out_of();
}

void coro::detail::coroutine::resume() {
    if (returned) return; 
    this->stack_ctx->switch_into();
}

void coro::detail::coroutine::YieldToCoroutine(coroutine & another) {
    this->yield();
    another.resume(); 
}



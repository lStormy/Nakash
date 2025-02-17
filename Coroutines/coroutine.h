#include "stack_context.h"
#include <cstdint>
#include <functional>
#include <memory>
#include <signal.h>


#ifndef CORO
#define CORO

namespace coro {
    namespace detail {
        struct coroutine {
            coroutine (std::function<void (coroutine &)> func, size_t stack_size = SIGSTKSZ): stack {new unsigned char[stack_size]}, func {func}  {
                stack = std::make_unique<unsigned char []> (stack_size); 
                stack_ctx = new stack_context(stack.get(), stack_size, reinterpret_cast<void (*)(void*)>(&coroutine_call), this);
            }
            
            void yield(); //Return to the context of the caller
            void resume(); // Return to the context of the callee 
            void YieldToCoroutine(coroutine & another);//Yield the execution to another coroutine 
            operator bool() const {
                return !returned; 
            }


            private: 
                stack_context * stack_ctx;
                std::unique_ptr<unsigned char []> stack; 
                std::function<void(coroutine&)> func;
                bool returned = false; 
                static void coroutine_call(void * arg) {
                    coroutine & this_ = * reinterpret_cast<coroutine * > (arg);
                    this_.func(this_); 
                    this_.returned = true;
                }


        };

    }
}

#endif

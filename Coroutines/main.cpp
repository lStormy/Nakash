#include "coroutine.h"
#include <iostream>

int main () {
    coro::detail::coroutine test{[](coro::detail::coroutine & self) {
        for (int i = 0; i < 5; i++) {
            std::cout << "coroutine " << i << std::endl; 
            self.yield(); 
        }
    }};
    while (test) {
        std::cout << "main" << std::endl;
        test.resume();
    }

    return 0;
}
#include "Scopedlock.h"
#include "Spinlock.h"
#include <thread>
#include <iostream>
SpinLock buffer_lock;
std::vector<int> buffer; // Buffer compartido

void Producer() {
    
    for (int i = 1; i <= 5; ++i) {
        buffer_lock.Acquire();  // Bloquea el acceso al buffer
        buffer.push_back(i);
        std::cout << "Productor: Insertó " << i << std::endl;
        buffer_lock.Release();  // Libera el lock
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simula trabajo
    }
}

void Consumer() {
    for (int i = 0; i < 5; ++i) {
        while (true) {  // Espera activa hasta que haya elementos en el buffer
            buffer_lock.Acquire();
            if (!buffer.empty()) {
                int value = buffer.front();
                buffer.erase(buffer.begin());
                std::cout << "Consumidor: Procesó " << value << std::endl;
                buffer_lock.Release();
                break;
            }
            buffer_lock.Release();
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Espera antes de reintentar
        }
    }
}

int main () {

    std::thread prod_thread(Producer);
    std::thread cons_thread(Consumer);

    prod_thread.join();
    cons_thread.join();  
    return 0; 
}
//El sistema siempre va a dar el mismo resultado con o sin locks. 

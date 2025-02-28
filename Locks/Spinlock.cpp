#include "Spinlock.h"

/*
Test_and_set (bool * plock) {
    const bool * old = plock
    *plock = true
    return old
}
Con el memory_order_acquire:
Garantiza que una escritura en esta dirección de memoria hecha por otros threads sea visible por
este thread. 
*/

bool SpinLock::TryAcquire() {
    bool alreadyLocked = m_atomic.test_and_set(std::memory_order_acquire); 
    return !alreadyLocked;
}   
//El lock gira (spin) hasta que pueda devolverse
void SpinLock::Acquire()  {
    while (!TryAcquire()) {
        std::this_thread::yield(); 
    }  
  }
//Lo hecho en el release va a poder leerse por otros threads leyendo desde esta dirección de memoria
void SpinLock::Release() {
    m_atomic.clear (std::memory_order_release); 
}
#include "Reentrantlock.h"

void ReentrantLock32::Acquire()  {
    std::hash<std::thread::id> hasher; 
    std::size_t t_id = hasher (std::this_thread::get_id()); 
    //si no sostiene el lock
    if (m_atomic.load(std::memory_order_relaxed) != t_id) {
        //girar hasta sostenerlo
        std::size_t unlockvalue = 0;
        while (!m_atomic.compare_exchange_weak(unlockvalue, 
            t_id, 
            std::memory_order_relaxed, 
            std::memory_order_relaxed)) {
                unlockvalue = 0;
                std::this_thread::yield(); 
            }
    } 
    //Incrementar el contador de referencias para verificar que
    //Aquire y Release se llamen de a pares
    ++m_refCount;
    //Usar una fence de adquirir para asegurarnos de que todos los reads
    //Subsiguientes leídos por este thread sean validos
    std::atomic_thread_fence(std::memory_order_acquire);  
}

void ReentrantLock32::Release()  {
    //Asegurarnos de que todas las escrituras fueron hechas antes de desbloquear
    std::atomic_thread_fence(std::memory_order_release); 
    
    std::hash<std::thread::id> hasher; 
    std::size_t t_id = hasher (std::this_thread::get_id());
    std::size_t actual = m_atomic.load (std::memory_order_relaxed);
    assert (actual == t_id);

    --m_refCount; 
    if (m_refCount == 0 ) {
        //Liberar lock, lo cual es seguro porque pertenece a este thread
        m_atomic.store (0, std::memory_order_relaxed);
    }
}

bool ReentrantLock32::TryAcquire()  {
    std::hash<std::thread::id> hasher; 
    std::size_t t_id = hasher(std::this_thread::get_id());
    
    bool acquired = false; 

    if (m_atomic.load (std::memory_order_relaxed) == t_id) {
        acquired = true;
    } else {
        std::size_t unlockValue = 0; 
        acquired = m_atomic.compare_exchange_strong (
            unlockValue, 
            t_id,
            std::memory_order_relaxed,
            std::memory_order_relaxed
        );
    }
    if (acquired) {
        ++m_refCount; 
        std::atomic_thread_fence (std::memory_order_acquire); 
    }
    return acquired; 

}
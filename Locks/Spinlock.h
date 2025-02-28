#include <thread>
#include <atomic>
#include <memory_resource>



#ifndef SPINLOCK_H
#define SPINLOCK_H
class SpinLock {
    std::atomic_flag m_atomic;     
public: 
    SpinLock (): m_atomic(false) {}
    bool TryAcquire ();
    void Acquire ();
    void Release ();
}; 

#endif
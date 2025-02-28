#include <atomic>
#include <cstdint>
#include <hashtable.h>
#include <thread>
#include <cassert>

#ifndef REENTRANTLOCK_H
#define REENTRANTLOCK_H
class ReentrantLock32 {
    std::atomic<std::size_t> m_atomic;
    std::int32_t m_refCount;  
public: 
    ReentrantLock32() :m_atomic(0), m_refCount(0) {}
    void Acquire();

    void Release();

    bool TryAcquire ();

    
}; 

#endif
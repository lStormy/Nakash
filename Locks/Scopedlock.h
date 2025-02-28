
#ifndef SCOPEDLOCK_H
#define SCOPEDLOCK_H
template <class LOCK>
class ScopedLock {
    typedef LOCK lock_t; 
    lock_t * m_pLock;
public: 
    explicit ScopedLock (lock_t & lock) :m_pLock (&lock) {
        m_pLock->Acquire(); 
    }

    ~ScopedLock() {
        m_pLock->Release(); 
    }
};
#endif
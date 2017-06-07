#ifndef __DSSYNC_H__
#define __DSSYNC_H__

#ifdef _WIN32
#include "windows.h"
class LMutex { 
    CRITICAL_SECTION cs;
    bool             initialized;
  public:
    LMutex() {
        InitializeCriticalSection(&cs);
        initialized = true;
    }
    ~LMutex() {
        DeleteCriticalSection(&cs);
        initialized = false;
    }
    bool isInitialized() { 
        return initialized;
    }
    void lock() {
        if (initialized) { 
            EnterCriticalSection(&cs);
        }
    }
    void unlock() {
        if (initialized) { 
            LeaveCriticalSection(&cs);
        }
    }
};
#else 

#include <pthread.h>

class LMutex { 
    pthread_mutex_t cs;
    bool            initialized;
  public:
    LMutex() {
        pthread_mutex_init(&cs, NULL);
        initialized = true;
    }
    ~LMutex() {
        pthread_mutex_destroy(&cs);
        initialized = false;
    }
    bool isInitialized() { 
        return initialized;
    }
    void lock() {
        if (initialized) { 
            pthread_mutex_lock(&cs);
        }
    }
    void unlock() {
        if (initialized) { 
            pthread_mutex_unlock(&cs);
        }
    }
};
#endif

class LMutexLocker
{
public:
    LMutexLocker( LMutex * );
    ~LMutexLocker();

    LMutex *mutex() const;

private:
    LMutex *mtx;
};

inline LMutexLocker::LMutexLocker( LMutex *m )
    : mtx( m )
{
    if ( mtx ) mtx->lock();
}

inline LMutexLocker::~LMutexLocker()
{
    if ( mtx ) mtx->unlock();
}

inline LMutex *LMutexLocker::mutex() const
{
    return mtx;
}

#endif // __DSSYNC_H__

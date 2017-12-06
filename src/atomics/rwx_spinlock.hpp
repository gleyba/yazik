//
// Created by Gleb Kolobkov on 04.12.17.
//

#ifndef YAZIK_RWX_SPINLOCK_H
#define YAZIK_RWX_SPINLOCK_H

#include <assert.h>
#include <atomic>
#include <cstddef>

namespace yazik {
namespace atomics {

class rwx_spinlock {
    std::atomic<long> _flag{0};

public:

    bool try_r_lock();
    void r_lock();
    bool try_w_lock();
    void w_lock();
    bool try_x_lock();
    void x_lock();

    void upgrade_w_to_x_lock();

    void r_unlock();
    void w_unlock();
    void x_unlock();

};

enum class rwx_lock_type : int {
    no,
    read,
    write,
    exclusive
};

class rwx_locker {

    rwx_spinlock* _spinlock;
    rwx_lock_type _lockType;

public:

    rwx_locker();
    rwx_locker(rwx_spinlock& spinlock,rwx_lock_type lockType);
    rwx_locker(const rwx_locker&) = delete;
    rwx_locker operator= (const rwx_locker&) = delete;

    void upgradeToExclusive();
    ~rwx_locker();
};

} //end of atomics namespace
} //end of yazik namespace

#endif //YAZIK_RWX_SPINLOCK_H

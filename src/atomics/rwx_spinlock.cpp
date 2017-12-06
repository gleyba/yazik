//
// Created by Gleb Kolobkov on 04.12.17.
//

#include "rwx_spinlock.hpp"

#include <limits>
#include <assert.h>
#include "baton.hpp"

namespace {

    constexpr long sMinLong = std::numeric_limits<long>::min();

} // namespace <anonymous>

namespace yazik {
namespace atomics {

bool rwx_spinlock::try_r_lock() {
    long v = _flag.load();
    if (v >= 0)
        return _flag.compare_exchange_weak(v, v + 2);
    return false;
}

void rwx_spinlock::r_lock() {
    long v = _flag.load();
    if (!try_r_lock()) for (;;) {
        baton b;
        if (v < 0) {
            v = _flag.load();
        } else if (_flag.compare_exchange_weak(v, v + 2)) {
            return;
        }
        b.incrementAttemptsAndCheck();
    }
}

bool rwx_spinlock::try_w_lock() {
    long v = _flag.load();
    if (v >= 0 && (v % 2) == 0)
        return _flag.compare_exchange_weak(v, v + 1);
    return false;
}


void rwx_spinlock::w_lock() {
    long v = _flag.load();
    if (!try_w_lock()) for (;;) {
        baton b;
        if (v < 0 || (v % 2) != 0) {
            v = _flag.load();
        } else if (_flag.compare_exchange_weak(v, v + 1)) {
            return;
        }
        b.incrementAttemptsAndCheck();
    }
}

bool rwx_spinlock::try_x_lock() {
    long v = _flag.load();
    if (v == 0)
        return _flag.compare_exchange_weak(v,sMinLong);
    return false;
}


void rwx_spinlock::x_lock() {
    long v = _flag.load();
    if (!try_x_lock()) for(;;) {
        baton b;
        if (v != 0) {
            v = _flag.load();
        } else if (_flag.compare_exchange_weak(v,sMinLong)) {
            return;
        }
        b.incrementAttemptsAndCheck();
    }
}

void rwx_spinlock::upgrade_w_to_x_lock() {
    long v = _flag.load();
    assert(v > 0 && (v % 2) == 1);
    for(;;) {
        baton b;
        if (v != 1) {
            v = _flag.load();
        } else if (_flag.compare_exchange_weak(v,sMinLong)) {
            return;
        }
        b.incrementAttemptsAndCheck();
    }
}

void rwx_spinlock::r_unlock() {
    _flag.fetch_sub(2);
}

void rwx_spinlock::w_unlock() {
    _flag.fetch_sub(1);
}

void rwx_spinlock::x_unlock() {
    _flag.store(0);
}

rwx_locker::rwx_locker()
: _spinlock{nullptr}
, _lockType{rwx_lock_type::no}
{}

rwx_locker::rwx_locker(rwx_spinlock& spinlock,rwx_lock_type lockType)
: _spinlock{&spinlock}
, _lockType{lockType}
{
    switch(_lockType)
    {
        case rwx_lock_type::read:
            _spinlock->r_lock();
            break;
        case rwx_lock_type::write:
            _spinlock->w_lock();
            break;
        case rwx_lock_type::exclusive:
            _spinlock->x_lock();
            break;
        default:
            break;
    }
}

rwx_locker::~rwx_locker() {
    switch(_lockType)
    {
        case rwx_lock_type::read:
            _spinlock->r_unlock();
            break;
        case rwx_lock_type::write:
            _spinlock->w_unlock();
            break;
        case rwx_lock_type::exclusive:
            _spinlock->x_unlock();
            break;
        default:
            break;
    }
}

void rwx_locker::upgradeToExclusive() {
    switch(_lockType)
    {
        case rwx_lock_type::write:
            _spinlock->upgrade_w_to_x_lock();
            _lockType = rwx_lock_type::exclusive;
            break;
        default:
            assert(false);
            break;
    }
}

} //end of atomics namespace
} //end of yazik namespace
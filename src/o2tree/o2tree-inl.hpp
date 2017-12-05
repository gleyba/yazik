//
// Created by Gleb Kolobkov on 05.12.17.
//

#include "o2tree.hpp"

namespace yazik {
namespace o2tree {

    template <typename T, size_t M>
    atomics::rwx_locker node_t::readLock() {
        return {_lock,atomics::rwx_lock_type::read};
    }

    template <typename T, size_t M>
    atomics::rwx_locker node_t::writeLock() {
        return {_lock,atomics::rwx_lock_type::write};
    }

    template <typename T, size_t M>
    atomics::rwx_locker node_t::exclusiveLock() {
        return {_lock,atomics::rwx_lock_type::exclusive};
    }

    template <typename T, size_t M>
    t::t(uint64_t m)
    : _m{m}
    {}


    template <typename T, size_t M>
    bool t::put(uint64_t key,T&& val) {
        node_t* node = &_root;
        atomics::rwx_locker nodeLocker = node->readLock();
        node_t* parent = nullptr;
        atomics::rwx_locker parentLocker;

        while(!node->isLeaf()) {
            if (node->isGreater(key)) {
                nodeLocker =
            } else {

            }
        }


        return true;
    }

    template <typename T, size_t M>
    bool t::put(uint64_t key,const T& val) {
        return put(key,val);
    }

} //end of o2tree namespace
} //end of yazik namespace

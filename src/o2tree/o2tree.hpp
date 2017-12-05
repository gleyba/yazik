//
// Created by Gleb Kolobkov on 05.12.17.
//

#ifndef YAZIK_O2TREE_HPP
#define YAZIK_O2TREE_HPP

#include "atomics/rwx_spinlock.hpp"

namespace yazik {
namespace o2tree {


    enum class node_type_t : int {
        red,
        black,
        leaf
    };

    template <typename T, size_t M>
    class node_t {
        atomics::rwx_spinlock _lock;
        node_type_t _type;
        uint64_t _key;

        node_t* _left = nullptr;
        node_t* _right = nullptr;

    public:

        inline bool isLeaf() { return _type == node_type_t::leaf; }
        inline bool isGreater(uint64_t key) {
            assert(!isLeaf());
            return key < _key;
        }

        atomics::rwx_locker readLock();
        atomics::rwx_locker writeLock();
        atomics::rwx_locker exclusiveLock();
    };

    template <typename T, size_t M = 1024>
    class t {

        using node_t = node_t<T>;
        node_t _root{node_type_t::leaf};

    public:

        t();

        bool put(uint64_t key,T&& val);
        bool put(uint64_t key,const T& val);

    };

} //end of o2tree namespace
} //end of yazik namespace

#include "o2tree-inl.hpp"

#endif //YAZIK_O2TREE_HPP

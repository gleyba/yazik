#pragma once

#include <deque>
#include "atomics/rwx_spinlock.hpp"


namespace yazik {
namespace o2tree {

    enum class node_type_t : int {
        none,
        red,
        black,
        leaf
    };

    template <typename T, size_t M>
    class node {
        atomics::rwx_spinlock _lock;
        node_type_t _type;
        uint64_t _key;

        node* _left = nullptr;
        node* _right = nullptr;

    public:

        inline bool isLeaf() { return _type == node_type_t::leaf; }
        inline bool isGreater(uint64_t key) {
            assert(!isLeaf());
            return key < _key;
        }

        static node none();

        atomics::rwx_locker readLock();
        atomics::rwx_locker writeLock();
    };

    template <typename T, size_t M>
    class node_bank {
        using node_t = node<T,M>;
        std::deque<node_t> _nodes;

    public:

    };

    template <typename T, size_t M = 96>
    class t {

        using node_t = node<T,M>;
        using node_bank_t = node_bank<T,M>;
        node_t _root{node_type_t::leaf};

    public:

        t(uint64_t size = 1024);

        bool put(uint64_t key,T&& val);
        bool put(uint64_t key,const T& val);

    };

} //end of o2tree namespace
} //end of yazik namespace

#include "o2tree-inl.hpp"

#pragma once

#include <deque>
#include <yazik/atomics/rwx_spinlock.hpp>


namespace yazik {
namespace o2tree {

    enum class node_type_t : int {
        none,
        red,
        black,
        leaf
    };

    template <typename T, size_t M>
    class node;

    template <typename T, size_t M = 24>
    class t {

        using node_t = node<T,M>;
        node_t* _root;

        node_t* new_leaf_node();
        void split_insert(uint64_t key,T&& val, node_t* node);

    public:

        t(uint64_t size = 1024);

        bool put(uint64_t key,T&& val);
        bool put(uint64_t key,const T& val);

    };

} //end of o2tree namespace
} //end of yazik namespace

#include "o2tree-inl.hpp"

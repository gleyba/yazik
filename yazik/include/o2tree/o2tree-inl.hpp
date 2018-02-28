#pragma once

#include "o2tree.hpp"
#include <yazik/utils/compute.hpp>

#define __TM_SIG(x) \
    template <typename T, size_t M> \
    x<T,M>

namespace yazik {
namespace o2tree {

    template <typename T, size_t M>
    class leaf {
        T _data[M];
        T* _last = _data;

    public:

        inline bool is_full() {
            return _last != (_data + M);
        }

        inline void split_half_to(leaf* other) {

        }

    };

    template <typename T, size_t M>
    class node_lock_pair;

    template <typename T, size_t M>
    class node {
        using leaf_t = leaf<T,M>;
        using node_lock_pair_t = node_lock_pair<T,M>;

        atomics::rwx_spinlock _lock;
        node_type_t _type;
        uint64_t _key;

        node* _left = nullptr;
        node* _right = nullptr;
        leaf_t* _leaf = nullptr;

    public:

        inline bool is_leaf() {
            return _type == node_type_t::leaf;
        }

        inline leaf_t* leaf() {
            assert(_leaf);
            return _leaf;
        }

        inline bool is_full() {
            assert(is_leaf());
            return _leaf->is_full();
        }

        inline bool is_greater(uint64_t key) {
            assert(!is_leaf());
            return key < _key;
        }

        node_lock_pair_t left_with_w_lock();
        node_lock_pair_t right_with_w_lock();

        inline atomics::rwx_locker read_lock() {
            return {_lock,atomics::rwx_lock_type::read};
        }

        inline atomics::rwx_locker write_lock() {
            return {_lock,atomics::rwx_lock_type::write};
        }

    };

    template <typename T, size_t M>
    class node_lock_pair {
        using node_t = node<T,M>;
        node_t* _node;
        atomics::rwx_locker _locker;

    public:

        node_lock_pair(node_t* node,atomics::rwx_locker&& locker)
        : _node{node}, _locker{std::move(locker)} {}

        inline bool is_leaf() { return _node->is_leaf(); }
        inline bool is_full() { return _node->is_full(); }
        inline void upgrade_to_x_lock() { _locker.upgrade_to_x(); }
        inline node_t* node() { return _node; }
    };

    __TM_SIG(auto node)::left_with_w_lock() -> node_lock_pair_t {
        return {_left,_left->writeLock()};
    }

    __TM_SIG(auto node)::right_with_w_lock() -> node_lock_pair_t {
        return {_right,_right->writeLock()};
    }


    __TM_SIG(t)::t(uint64_t size) {

    }

    __TM_SIG(auto t)::new_leaf_node() -> node_t* {
        return nullptr;
    }

    __TM_SIG(void t)::split_insert(uint64_t key,T&& val, node_t* node) {
        auto* newLeafNode = new_leaf_node();
        node->leaf()->split_half_to(newLeafNode->leaf());
    }

    __TM_SIG(bool t)::put(uint64_t key,T&& val) {
        using node_lock_pair_t = node_lock_pair<T,M>;

        node_lock_pair_t parent{nullptr,{}};
        node_lock_pair_t node{_root,_root->write_lock()};

        while(!node.is_leaf()) {
            parent = std::move(node);
            if (parent->is_greater(key)) {
                node = std::move(parent.right_with_w_lock());
            } else {
                node = std::move(parent.left_with_w_lock());
            }
        }

        parent.upgrade_to_x_lock();
        node.upgrade_to_x_lock();

        if (node.is_full()) {

        } else {

        }

        return true;
    }

    __TM_SIG(bool t)::put(uint64_t key,const T& val) {
        return put(key,val);
    }

} //end of o2tree namespace
} //end of yazik namespace

#undef __TM_SIG

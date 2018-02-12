#pragma once

#include <boost/config/detail/suffix.hpp>
#include <ltalloc/ltalloc.h>


namespace yazik {
namespace utility {

//≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈

struct lt_inline_allocator {
    static BOOST_FORCEINLINE void *allocate(size_t size) {
        return ltmalloc(size);
    }
    static BOOST_FORCEINLINE void *reallocate(void *pointer,size_t size) {
        return ltrealloc(pointer,size);
    }

    static BOOST_FORCEINLINE void deallocate(void *pointer,size_t size) {
        ltfree(pointer);
    }

    static BOOST_FORCEINLINE void deallocate(void *pointer) {
        ltfree(pointer);
    }
};

//≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈

struct std_inline_allocator {
    static BOOST_FORCEINLINE void *allocate(size_t size) {
        return malloc(size);
    }
    static BOOST_FORCEINLINE void deallocate(void *pointer,size_t size) {
        free(pointer);
    }
    static BOOST_FORCEINLINE void deallocate(void *pointer) {
        free(pointer);
    }
    static BOOST_FORCEINLINE void *reallocate(void *pointer,size_t size) {
        return realloc(pointer,size);
    }
};

//≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈

using default_inline_allocator = std_inline_allocator;// RXD_IF_DEBUG_OR(std_inline_allocator,lt_inline_allocator);

} //end of utility namespace
} //end of yazik namespace

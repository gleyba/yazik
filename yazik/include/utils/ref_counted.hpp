#pragma once

#include <mutex>
#include <yazik/utils/misc.hpp>
#include <yazik/utils/inline_allocator.hpp>

namespace yazik {
namespace utility {

namespace detail {

//≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈

using rc_controll_object = std::atomic<std::size_t>;

} //end of detail namespace

using rc_controll_object = detail::rc_controll_object;

//≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈

template<bool T_Weakable>
class ref_counted_templ {
public:

    using allocator_t = default_inline_allocator;
    using has_weak = std::integral_constant<bool, T_Weakable> ;

private:

    typename std::conditional<T_Weakable,rc_controll_object*,rc_controll_object>::type _rc;

    //----------------------------------------------------------------------

    static BOOST_FORCEINLINE
    void* allocImpl(std::size_t sz) {
        void* res = allocator_t::allocate(sz);
        return res;
    }

    //----------------------------------------------------------------------

    static BOOST_FORCEINLINE void deallocImpl(void* p) {
        allocator_t::deallocate(p);
    }

    //----------------------------------------------------------------------

public:

    //----------------------------------------------------------------------

    template<bool T_W = T_Weakable>
    BOOST_FORCEINLINE
    std::enable_if_t<T_W> initRc() noexcept {
        _rc = allocator_t::allocate(sizeof(rc_controll_object) * 2);
        // _rc = new rc_controll_object[2];
        new (_rc) rc_controll_object{0};
        new (_rc + 1) rc_controll_object{1};
    }

    //----------------------------------------------------------------------

    template<bool T_W = T_Weakable>
    BOOST_FORCEINLINE
    std::enable_if_t<!T_W> initRc() noexcept {}

    //----------------------------------------------------------------------

    ref_counted_templ() {
        initRc();
    }
    virtual ~ref_counted_templ() {}

    ref_counted_templ(const ref_counted_templ&) = delete;
    ref_counted_templ& operator=(const ref_counted_templ&) = delete;

    //----------------------------------------------------------------------

    template<bool T_W = T_Weakable>
    BOOST_FORCEINLINE
    std::enable_if_t<T_W> ref() const noexcept {
        _rc->fetch_add(1,std::memory_order_release);
    }
    //----------------------------------------------------------------------

    template<bool T_W = T_Weakable>
    BOOST_FORCEINLINE
    std::enable_if_t<!T_W> ref() const noexcept {
        _rc.fetch_add(1,std::memory_order_release);
    }

    //----------------------------------------------------------------------

    template<bool T_W = T_Weakable>
    BOOST_FORCEINLINE
    std::enable_if_t<T_W, rc_controll_object*> counters() const noexcept {
        return _rc;
    }

    //----------------------------------------------------------------------

    template<bool T_W = T_Weakable>
    BOOST_FORCEINLINE
    std::enable_if_t<T_W> deref() const noexcept {
        size_t strongPrev = _rc->fetch_sub(1,std::memory_order_acq_rel);

        YAssert(strongPrev != 0);
        if (strongPrev == 1) {
            delete this;

            size_t weakPrev = (_rc + 1)->fetch_sub(1,std::memory_order_acq_rel);
            YAssert(weakPrev != 0);
            if (weakPrev == 1) {
                allocator_t::deallocate(_rc);
            }
        }

    }

    //----------------------------------------------------------------------

    template<bool T_W = T_Weakable>
    BOOST_FORCEINLINE
    std::enable_if_t<!T_W>
    deref() const noexcept {
        size_t strongPrev = _rc.fetch_sub(1,std::memory_order_acq_rel);
        YAssert(strongPrev != 0);
        if (strongPrev == 1) {
            delete this;
        }
    }

    //----------------------------------------------------------------------

    static void operator delete (void * mem) {
        deallocImpl(mem);
    }

    //----------------------------------------------------------------------

    static void* operator new(std::size_t sz) {
        return allocImpl(sz);
    }

    //----------------------------------------------------------------------

    //placement new
    template<typename... T_Args>
    static void* operator new (std::size_t sz, void *p) {
        return p;
    }

    //----------------------------------------------------------------------
};

//≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈

using ref_counted = ref_counted_templ<true>;
using ref_counted_no_weak = ref_counted_templ<false>;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template<bool T_Weakable>
BOOST_FORCEINLINE
void intrusive_ptr_add_ref(const ref_counted_templ<T_Weakable>* p) {
    if (YCheckWithAssert(p != nullptr)) {
        p->ref();
    }
}

//----------------------------------------------------------------------

template<bool T_Weakable>
BOOST_FORCEINLINE
void intrusive_ptr_release(const ref_counted_templ<T_Weakable>* p) {
    if (YCheckWithAssert(p!=nullptr)) {
        p->deref();
    }
}

//----------------------------------------------------------------------

BOOST_FORCEINLINE
rc_controll_object* intrusive_ptr_get_counters(const ref_counted_templ<true>* p) {
    if (p != nullptr) {
        auto* counters = p->counters();
        if (YCheckWithAssert(counters)) {
            return counters;
        }
    }
    return nullptr;
}

//----------------------------------------------------------------------

} //end of utility namespace
} //end of yazik namespace

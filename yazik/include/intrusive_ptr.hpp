#pragma once

#include <boost/config/detail/suffix.hpp>
#include <boost/operators.hpp>

#include <yazik/utils/ref_counted.hpp>

namespace yazik {

template <class T>
class intrusive_ptr;

//≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈


#pragma mark intrusive_weak_ptr

template <class T>
struct intrusive_weak_ptr
: boost::equality_comparable<intrusive_weak_ptr<T>>
, boost::equality_comparable<intrusive_weak_ptr<T>, intrusive_ptr<T>>
, boost::equality_comparable<intrusive_weak_ptr<T>, const T*>
, boost::equality_comparable<intrusive_weak_ptr<T>, std::nullptr_t> {
    friend class intrusive_ptr<T>;
    using pointer = T*;
    using strong_type   = intrusive_ptr<T>;
    using reference = T&;
    using const_pointer = const T*;

    using rc_controll_object = utility::rc_controll_object;

private:
    pointer _ptr                            = nullptr;
    rc_controll_object* _counters           = nullptr;

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    void refWeak() const {
        weakRc()->fetch_add(1,std::memory_order_release);
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    void derefWeak() const {
        if (weakRc()->fetch_sub(1,std::memory_order_acq_rel) == 1) {
            T::allocator_t::deallocate((void*)_counters);
        }
    }

public:

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    BOOST_FORCEINLINE
    rc_controll_object* strongRc() const {
        if (YCheckWithAssert(_ptr != nullptr && _counters != nullptr)) {
            return _counters;
        }
        return nullptr;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    rc_controll_object* weakRc() const {
        if (YCheckWithAssert(_ptr != nullptr && _counters != nullptr)) {
            return _counters + 1;
        }
        return nullptr;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    intrusive_weak_ptr(pointer p)
     : _ptr(p)
     , _counters(intrusive_ptr_get_counters(p)) {
        if (p != nullptr && YCheckWithAssert(p != nullptr)) {
            refWeak();
        }
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    constexpr intrusive_weak_ptr() {/* nop */}

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    intrusive_weak_ptr(std::nullptr_t) {/* nop */}

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    void swap(intrusive_weak_ptr& other) {
        std::swap(_ptr, other._ptr);
        std::swap(_counters, other._counters);
    }


    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    intrusive_weak_ptr(intrusive_weak_ptr&& other) {
        std::tie(_ptr,_counters) = other.release();
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    intrusive_weak_ptr(const intrusive_weak_ptr& other)
    : _ptr(other._ptr)
    , _counters(other._counters) {
        if (other._ptr) {
            refWeak();
        }
    }

    //----------------------------------------------------------------------

    template <class Y>
    BOOST_FORCEINLINE
    explicit intrusive_weak_ptr(const intrusive_weak_ptr<Y>& other) {
        static_assert(std::is_convertible<Y*, T*>::value,
                      "Y* is not assignable to T*");

        std::tie(_ptr,_counters) = intrusive_weak_ptr<Y>{other}.release();
    }

    //----------------------------------------------------------------------

    template <class Y>
    BOOST_FORCEINLINE
    explicit intrusive_weak_ptr(intrusive_weak_ptr<Y>&& other) {
        static_assert(std::is_convertible<Y*, T*>::value,"Y* is not assignable to T*");
        std::tie(_ptr,_counters) = other.release();
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    explicit intrusive_weak_ptr(const strong_type& strong_ref) {
        reset(strong_ref.get());
    }

    //----------------------------------------------------------------------

    template <class Y>
    BOOST_FORCEINLINE
    explicit intrusive_weak_ptr(const intrusive_ptr<Y>& strong_ref) {
        static_assert(std::is_convertible<Y*, T*>::value, "Y* is not assignable to T*");

        reset(strong_ref.get());
    }

    //----------------------------------------------------------------------

    ~intrusive_weak_ptr() {
        if (_ptr != nullptr && YCheckWithAssert(_counters != nullptr)) {
            derefWeak();
        }
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    void reset(pointer newValue = nullptr) {
        if (_ptr != nullptr && YCheckWithAssert(_counters != nullptr)) {
            derefWeak();
        }
        _ptr = newValue;
        if (newValue != nullptr) {
            _counters = intrusive_ptr_get_counters(newValue);
            if (YCheckWithAssert(_counters != nullptr)) {
                refWeak();
            } else {
                _ptr = nullptr;
            }
        } else {
            _counters = nullptr;
        }
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    std::pair<pointer,rc_controll_object*> release() {
        auto* ptr = _ptr;
        auto* counters = _counters;
        _ptr = nullptr;
        _counters = nullptr;
        return {ptr,counters};
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    intrusive_weak_ptr& operator=(pointer p) = delete;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    BOOST_FORCEINLINE
    intrusive_weak_ptr& operator=(std::nullptr_t) {
        reset(nullptr);
        return *this;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    intrusive_weak_ptr& operator=(intrusive_weak_ptr&& other) {
        swap(other);
        return *this;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    intrusive_weak_ptr& operator=(const intrusive_weak_ptr& other) {
        if (_ptr != nullptr && YCheckWithAssert(_counters)) {
            derefWeak();
        }
        _ptr = other._ptr;
        _counters = other._counters;
        if (_ptr != nullptr && YCheckWithAssert(_counters)) {
            refWeak();
        }
        return *this;
    }

    //----------------------------------------------------------------------

    template <class Y>
    BOOST_FORCEINLINE
    intrusive_weak_ptr& operator=(const intrusive_weak_ptr<Y>& other) {
        static_assert(std::is_convertible<Y*, T*>::value,"Y* is not assignable to T*");
        if (_ptr != nullptr && YCheckWithAssert(_counters)) {
            derefWeak();
        }
        std::tie(_ptr,_counters) = intrusive_weak_ptr{other}.release();
        return *this;
    }

    //----------------------------------------------------------------------

    template <class Y>
    BOOST_FORCEINLINE
    intrusive_weak_ptr& operator=(intrusive_weak_ptr<Y>&& other) {
        static_assert(std::is_convertible<Y*, T*>::value,"Y* is not assignable to T*");

        std::tie(_ptr,_counters) = other.release();
        return *this;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    intrusive_weak_ptr& operator=(const strong_type& other);
    template <class Y>
    intrusive_weak_ptr& operator=(const intrusive_ptr<Y>& strong_ref);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    BOOST_FORCEINLINE
    bool valid() const {
        return _ptr != nullptr
            && YCheckWithAssert(_counters != nullptr)
            && strongRc()->load() > 0;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    pointer get() const {
        if (valid()) {
            return _ptr;
        }
        return nullptr;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    pointer get_unsafe(bool IKnowWhatIDo = false) const {
        if (IKnowWhatIDo || YCheckWithAssert(valid())) {
            return _ptr;
        }
        return nullptr;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    strong_type lock() const {
        strong_type result{nullptr};
        if (_ptr != nullptr && YCheckWithAssert(_counters != nullptr)) {
            size_t curStrongCount = _counters->load(std::memory_order_acquire);
            do {
                if (curStrongCount == 0) {
                    return result;
                }
            } while(!_counters->compare_exchange_strong(
                curStrongCount,
                curStrongCount + 1,
                std::memory_order_release,
                std::memory_order_consume
            ));
            result.reset(_ptr,false);
        }

        return result;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    pointer operator->() const {
        return get_unsafe();
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    reference operator*() const {
        return *get();
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    bool operator!() const {
        return get() == nullptr;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    explicit operator bool() const {
        return valid();
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    ptrdiff_t compare(const_pointer ptr) const {
        return static_cast<ptrdiff_t>(get() - ptr);
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    ptrdiff_t compare(const strong_type& other) const;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    BOOST_FORCEINLINE
    ptrdiff_t compare(const intrusive_weak_ptr& other) const {
        return compare(other.get());
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    ptrdiff_t compare(std::nullptr_t) const {
        return reinterpret_cast<ptrdiff_t>(get());
    }

    //----------------------------------------------------------------------

    template <class C>
    BOOST_FORCEINLINE
    intrusive_weak_ptr<C> downcast() const {
        auto* ptr = get();
        return (ptr) ? dynamic_cast<C*>(ptr) : nullptr;
    }

    //----------------------------------------------------------------------

    template <class C>
    BOOST_FORCEINLINE
    intrusive_weak_ptr<C> upcast() const {
        auto* ptr = get();
        return (ptr) ? static_cast<C*>(ptr) : nullptr;
    }

    //----------------------------------------------------------------------

};

//≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈


#pragma mark intrusive_ptr

template <class T>
struct intrusive_ptr
: boost::equality_comparable<intrusive_ptr<T> >
, boost::equality_comparable<intrusive_weak_ptr<T>>
, boost::equality_comparable<intrusive_ptr<T>, const T*>
, boost::equality_comparable<intrusive_ptr<T>, std::nullptr_t> {

    using pointer = T*;
    using weak_type = intrusive_weak_ptr<T>;
    using const_pointer = const T*;
    using element_type = T;
    using reference = T&;
    using const_reference = const T&;

private:
    pointer _ptr = nullptr;

public:

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    BOOST_FORCEINLINE
    constexpr intrusive_ptr() noexcept {/* nop */}

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    intrusive_ptr(pointer raw_ptr, bool add_ref = true) noexcept {
        set_ptr(raw_ptr, add_ref);
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    intrusive_ptr(intrusive_ptr&& other) noexcept
    : _ptr(other.release()) { /* nop */}

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    intrusive_ptr(const intrusive_ptr& other) noexcept {
        set_ptr(other.get(), true);
    }

    //----------------------------------------------------------------------

    template <class Y>
    BOOST_FORCEINLINE
    intrusive_ptr(intrusive_ptr<Y> other) noexcept
    : _ptr(other.release()) {
        static_assert(std::is_convertible<Y*, T*>::value,
                      "Y* is not assignable to T*");
    }

    //----------------------------------------------------------------------

    ~intrusive_ptr() {
        if (_ptr) {
            intrusive_ptr_release(_ptr);
        }
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    void swap(intrusive_ptr& other) noexcept {
        std::swap(_ptr, other._ptr);
    }

    //----------------------------------------------------------------------

    /**
     * Returns the raw pointer without modifying reference
     * count and sets this to `nullptr`.
     */
    BOOST_FORCEINLINE
    pointer release() noexcept {
        auto result = _ptr;
        _ptr = nullptr;
        return result;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    void reset(pointer new_value = nullptr, bool add_ref = true) noexcept {
        auto old = _ptr;
        set_ptr(new_value, add_ref);
        if (old) {
            intrusive_ptr_release(old);
        }
    }

    //----------------------------------------------------------------------

    template <class... Ts>
    BOOST_FORCEINLINE
    void emplace(Ts&&... xs) {
        reset(new T(std::forward<Ts>(xs)...));
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    intrusive_ptr& operator=(pointer ptr) noexcept {
        reset(ptr);
        return *this;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    intrusive_ptr& operator=(intrusive_ptr&& other) noexcept {
        swap(other);
        return *this;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    intrusive_ptr& operator=(const intrusive_ptr& other) noexcept {
        intrusive_ptr tmp{ other };
        swap(tmp);
        return *this;
    }

    //----------------------------------------------------------------------

    template <class Y>
    BOOST_FORCEINLINE
    intrusive_ptr& operator=(intrusive_ptr<Y> other) noexcept {
        intrusive_ptr tmp{ std::move(other) };
        swap(tmp);
        return *this;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    pointer get() const noexcept {
        return _ptr;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    intrusive_ptr& operator=(const weak_type& other) noexcept {
        static_assert(T::has_weak::value,"");
        intrusive_ptr tmp{ std::move(other.lock()) };
        swap(tmp);
        return *this;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    weak_type weak() const noexcept {
        static_assert(T::has_weak::value,"");
        return weak_type{get()};
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    pointer operator->() const {
        return _ptr;
    }
    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    reference operator*() const {
        return *_ptr;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    bool operator!() const {
        return _ptr == nullptr;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    bool valid() const {
        return _ptr != nullptr;
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    explicit operator bool() const {
        return valid();
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    ptrdiff_t compare(const_pointer ptr) const {
        return static_cast<ptrdiff_t>(get() - ptr);
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    ptrdiff_t compare(const intrusive_ptr& other) const {
        return compare(other.get());
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    ptrdiff_t compare(const weak_type& other) const {
        return compare(other.get());
    }

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    ptrdiff_t compare(std::nullptr_t) const {
        return reinterpret_cast<ptrdiff_t>(get());
    }

    //----------------------------------------------------------------------

    template <class C>
    BOOST_FORCEINLINE
    intrusive_ptr<C> downcast() const {
        return (_ptr) ? dynamic_cast<C*>(get()) : nullptr;
    }

    //----------------------------------------------------------------------

    template <class C>
    BOOST_FORCEINLINE
    intrusive_ptr<C> upcast() const {
        return (_ptr) ? static_cast<C*>(get()) : nullptr;
    }

private:

    //----------------------------------------------------------------------

    BOOST_FORCEINLINE
    void set_ptr(pointer raw_ptr, bool add_ref) noexcept {
        _ptr = raw_ptr;
        if (raw_ptr && add_ref) {
            intrusive_ptr_add_ref(raw_ptr);
        }
    }

    //----------------------------------------------------------------------
};

//≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈

//----------------------------------------------------------------------

template<typename T>
intrusive_weak_ptr<T>& intrusive_weak_ptr<T>::operator=(const strong_type& strong_ref) {
    reset(strong_ref.get());
    return *this;
}

//----------------------------------------------------------------------

template <typename T>
template <class Y>
intrusive_weak_ptr<T>& intrusive_weak_ptr<T>::operator=(const intrusive_ptr<Y>& strong_ref) {
    static_assert(std::is_convertible<Y*, T*>::value,
                      "Y* is not assignable to T*");

    reset(strong_ref.get());
    return *this;
}

//----------------------------------------------------------------------

template<typename T>
ptrdiff_t intrusive_weak_ptr<T>::compare(const strong_type& other) const {
    return compare(other.get());
}

//----------------------------------------------------------------------

/**
 * @relates intrusive_ptr
 */
template <class X, typename Y>
bool operator==(const intrusive_ptr<X>& lhs, const intrusive_ptr<Y>& rhs) {
    return lhs.get() == rhs.get();
}

//----------------------------------------------------------------------

/**
 * @relates intrusive_ptr
 */
template <class X, typename Y>
bool operator!=(const intrusive_ptr<X>& lhs, const intrusive_ptr<Y>& rhs) {
    return !(lhs == rhs);
}

//----------------------------------------------------------------------

/**
 * @relates intrusive_ptr
 */
template <class X, typename Y>
bool operator==(const intrusive_weak_ptr<X>& lhs, const intrusive_weak_ptr<Y>& rhs) {
    return lhs.get() == rhs.get();
}

//----------------------------------------------------------------------

/**
 * @relates intrusive_ptr
 */
template <class X, typename Y>
bool operator!=(const intrusive_weak_ptr<X>& lhs, const intrusive_weak_ptr<Y>& rhs) {
    return !(lhs == rhs);
}

//----------------------------------------------------------------------

/**
 * @relates intrusive_ptr
 */
template <class X, typename Y>
bool operator==(const intrusive_ptr<X>& lhs, const intrusive_weak_ptr<Y>& rhs) {
    return lhs.get() == rhs.get();
}

//----------------------------------------------------------------------

/**
 * @relates intrusive_ptr
 */
template <class X, typename Y>
bool operator!=(const intrusive_ptr<X>& lhs, const intrusive_weak_ptr<Y>& rhs) {
    return !(lhs == rhs);
}

//----------------------------------------------------------------------

/**
 * @relates intrusive_ptr
 */
template <class X, typename Y>
bool operator==(const intrusive_weak_ptr<X>& lhs, const intrusive_ptr<Y>& rhs) {
    return lhs.get() == rhs.get();
}

//----------------------------------------------------------------------

/**
 * @relates intrusive_ptr
 */
template <class X, typename Y>
bool operator!=(const intrusive_weak_ptr<X>& lhs, const intrusive_ptr<Y>& rhs) {
    return !(lhs == rhs);
}

//----------------------------------------------------------------------

} //end of yazik namespace

namespace std {

//----------------------------------------------------------------------

template <typename T>
struct hash<::yazik::intrusive_weak_ptr<T>> {
    size_t operator()(const ::yazik::intrusive_weak_ptr<T> & x) const {
        return std::hash<void*>()(x.get());
    }
};

//----------------------------------------------------------------------

template <typename T>
struct hash<::yazik::intrusive_ptr<T>> {
    size_t operator()(const ::yazik::intrusive_weak_ptr<T> & x) const {
        return std::hash<void*>()(x.get());
    }
};

//----------------------------------------------------------------------

} //end of std namespace

#pragma once
#include <bits/stdc++.h>

namespace My {

template <class T, class Deleter = std::default_delete<T>>
class unique_ptr {
public:
    /*** C++ Standard Named Requirements for unique_ptr***/
    using element_type = T;
    using deleter_type = Deleter;
    using pointer = T*;


    // Constructors
    constexpr unique_ptr() noexcept
    : __ptr(nullptr), __del() {}

    explicit unique_ptr(pointer p) noexcept
    : __ptr(p), __del() {}

    unique_ptr(pointer p, const Deleter& d) noexcept
    : __ptr(p), __del(d) {}

    unique_ptr(pointer p, Deleter&& d) noexcept
    : __ptr(p), __del(std::move(d)) {}


    // Move ctors
    unique_ptr(unique_ptr &&u) noexcept
    : __ptr(u.__ptr), __del(std::move(u.__del)) {
        u.__ptr = nullptr;
    }

    unique_ptr(const unique_ptr &) = delete;

    ~unique_ptr() {
        if (__ptr)
            __del(__ptr);
    }


    // Assignment
    unique_ptr& operator=(unique_ptr &&r) noexcept {
        if (this == &r) return *this;

        reset(r.release());
        __del = std::forward<Deleter>(r.get_deleter());

        return *this;
    }

    unique_ptr& operator=(std::nullptr_t) noexcept {
        reset();
        return *this;
    }

    unique_ptr& operator=(const unique_ptr&) = delete;


    // Modifiers
    pointer release() noexcept {
        auto out = __ptr;
        __ptr = nullptr;
        return out;
    }

    void reset(pointer ptr = pointer()) noexcept {
        if (__ptr && ptr != __ptr)
            __del(__ptr);

        __ptr = ptr;
    }

    void swap(unique_ptr &other) noexcept {
        std::swap(__ptr, other.__ptr);
        std::swap(__del, other.__del);
    }


    /*** Observers ***/
    pointer get() const noexcept { return __ptr; }
    Deleter& get_deleter() noexcept { return __del; }
    const Deleter& get_deleter() const noexcept { return __del; }
    explicit operator bool() const noexcept { return __ptr; }

    std::add_lvalue_reference_t<T> operator*() const noexcept { return *__ptr; }
    pointer operator->() const noexcept { return __ptr; }

private:
    pointer __ptr;
    Deleter __del;
 
};

template<typename T1, typename T2>
bool operator==(My::unique_ptr<T1>& lhs, My::unique_ptr<T2>& rhs){
    return lhs.get() == rhs.get();
}

/*template<typename T1> */
template<typename T1>
bool operator==(My::unique_ptr<T1>& lhs, std::nullptr_t rhs){
    return lhs.get() == nullptr;
}

//Overload operator!=
template<typename T1, typename T2>
bool operator!=(My::unique_ptr<T1>& lhs, My::unique_ptr<T2>& rhs){
    return !(lhs == rhs);
}

template<typename T1>
bool operator!=(My::unique_ptr<T1>& lhs, std::nullptr_t rhs){
    return !(lhs == rhs);
}



}

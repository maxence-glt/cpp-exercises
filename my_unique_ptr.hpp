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

    template <class U, class E>
    unique_ptr(unique_ptr<U, E> &&u) noexcept
    : __ptr(u.release()), __del(std::forward<E>(u.get_deleter())) {}


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

    template <class U, class E>
    unique_ptr& operator=(unique_ptr<U, E>&& u) noexcept {
        reset(u.release());
        __del = std::forward<E>(u.get_deleter());
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

template <class T, class... Args>
std::enable_if_t<!std::is_array<T>::value, unique_ptr<T>>
make_unique(Args&&... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <class T, class... Args>
std::enable_if_t<std::is_array<T>::value, unique_ptr<T>>
make_unique(Args&&...) = delete;



int test(){
    /* Test Cases*/

    {
        /* Non member function operator== 
           Comparison with another unique_ptr or nullptr */ 
        My::unique_ptr<int> ptr1(new int(10));
        My::unique_ptr<int> ptr2(new int(10));
        assert(ptr1 == ptr1);   
        assert(ptr1 != ptr2);
        std::cout << "Test 1 passed\n";
    }

    {
        /* Create and access */
        My::unique_ptr<int> ptr(new int(10));
        assert(ptr != nullptr);
        assert(*ptr == 10);
        std::cout << "Test 2 passed\n";
    }

    {
        /* Reset unique_ptr 
           Replaces the managed object with a new one*/
        My::unique_ptr<int> ptr(new int(10));
        ptr.reset(new int(20));
        assert(ptr != nullptr);
        assert(*ptr == 20);

        // Self-reset test
        ptr.reset(ptr.get());
        std::cout << "Test 3 passed\n";
    }

    {
        /* Release unique_ptr ownership - 
           Returns a pointer to the managed object and releases ownership */
        My::unique_ptr<double> ptr(new double(3.14));
        double* rawPtr = ptr.release();
        assert(ptr == nullptr);
        assert(rawPtr != nullptr);
        assert(*rawPtr == 3.14);
        std::cout << "Test 4 passed\n";
    }

    {
        /* Non-member function swap
           Swap the managed objects */
        My::unique_ptr<int> ptr1(new int(10));
        My::unique_ptr<int> ptr2(new int (20));
        swap(ptr1, ptr2);
        assert(*ptr1 == 20);
        assert(*ptr2 == 10);
        std::cout << "Test 5 passed\n";
    }

    {
        /* Get the raw underlying pointer */
        int* x = new int(10);
        My::unique_ptr<int> ptr(x);
        int* rawPtr = ptr.get();
        assert(*rawPtr == 10);
        assert(rawPtr == x);
        std::cout << "Test 6 passed\n";
    }

    {
        /* operator bool to test if the unique pointer owns an object */
        My::unique_ptr<int> ptr(new int(42));
        assert(ptr);
        ptr.reset(nullptr);
        assert(!ptr);
        std::cout << "Test 7 passed\n";
    }

    {
        /* indirection operator* to dereference pointer to managed object,
           member access operator -> to call member function*/ 
        struct X{
            int n;
            int foo(){ return n; }
        };

        My::unique_ptr<X> ptr(new X{10});
        assert((*ptr).n == 10);
        assert(ptr->foo() == 10);
        std::cout << "Test 8 passed\n";
    }

    return 0;
}

}

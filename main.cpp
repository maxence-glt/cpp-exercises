#include "bits/stdc++.h"
#include "my_vector.hpp"
#include "my_unique_ptr.hpp"

int main(){
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

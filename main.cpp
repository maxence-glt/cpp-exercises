#include "bits/stdc++.h"
#include "my_vector.hpp"

int main() {
    My::vector<int> vec(10);
    vec.back() = 1;
    std::cout << (*(vec.end() - 1));
    using namespace My;
    std::allocator<int> alloc;

    // Testing constructors
    vector<int> v1;
    vector<int> v2(alloc);
    vector<int> v3(10);
    vector<int> v4(10, 67);
    vector<int> v5(v3.begin(), v3.end());
    vector<int> v6(v4);
    // (5) Skipping ranges
    vector<int> v8(std::move(v4));
    vector<int> v9(v8, alloc);
    vector<int> v10(std::move(v8), alloc);
    vector<int> v11{1, 2, 3, 4};
    // v4 and v8 have been MOVED

    // Testing assignment operator
    v1 = v3;
    v1 = std::move(v3);
    const auto ilist = {1, 2, 3, 4};
    v1 = ilist;
    // v3 (and v4/v8) has been MOVED

    std::cout << v1.data() << '\n';
    std::cout << v3.data();

    // Testing assign

    return 0;
}

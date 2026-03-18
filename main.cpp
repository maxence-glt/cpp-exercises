#include <iostream>
#include <unordered_map>
#include "my_unordered_map.hpp"
#include "my_unique_ptr.hpp"
#include "my_vector.hpp"

int main() {
    std::unordered_map<int, int> m;
    m[3] = 3;
    std::cout << sizeof(m);
    return 0;
}

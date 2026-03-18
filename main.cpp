#include <iostream>
#include <unordered_map>
#include "my_unordered_map.hpp"
#include "my_unique_ptr.hpp"
#include "my_vector.hpp"

int main() {
    My::unordered_map<int, int> m;
    m.insert({1, 1});
    std::cout << "1";

    return 0;
}

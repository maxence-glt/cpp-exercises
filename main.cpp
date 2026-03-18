#include <iostream>
#include <unordered_map>
#include "my_unordered_map.hpp"
#include "my_unique_ptr.hpp"
#include "my_vector.hpp"

int main() {
    My::unordered_map<int, int> m;
    m.insert({3, 67});
    m.insert({5, 7});
    m.insert({6, 7});

    return 0;
}

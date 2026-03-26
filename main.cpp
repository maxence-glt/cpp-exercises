#include <iostream>
#include <unordered_map>
#include "my_unordered_map.hpp"
#include "my_unique_ptr.hpp"
#include "my_vector.hpp"
#include <unistd.h>
#include <pthread.h>

int main() {
    My::unordered_map<int, int> m;
    m[3] = 33;
    m[9] = 3;
    m[91] = 3;
    m[19] = 3;

    std::cout << m.toString();


    My::unordered_map<int, int> m2(m);

    return 0;
}

#include "bits/stdc++.h"
#include "my_vector.hpp"

int main() {
    using namespace My;
    vector<int> v1 = {1, 2, 3};
    vector<int> v2;
    v2 = std::move(v1);
    const auto w = {4, 5, 6, 7};
    v2 = w;

    return 0;
}

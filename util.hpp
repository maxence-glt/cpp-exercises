#pragma once

#include <random>
#include <type_traits>
#include <chrono>

namespace Rand {

inline std::mt19937_64& engine() {
    static thread_local std::mt19937_64 e{ std::random_device{}() };
    return e;
}

template <typename T>
requires std::is_floating_point_v<T>
inline T random() {
    std::uniform_real_distribution<T> dis(T(0), T(1));
    return dis(engine());
}

template <typename T>
requires std::is_floating_point_v<T>
inline T random(T min, T max) {
    return min + (max - min)*random<T>();
}

} // namespace Rand

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;

using std::chrono::nanoseconds;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::seconds;
using namespace std::chrono_literals;

using timePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

// current time
inline timePoint curr_time() {
    return high_resolution_clock::now();
}

template <typename T>
inline T diff_time(timePoint t1, timePoint t2) {
    return duration_cast<T>(t2 - t1);
}

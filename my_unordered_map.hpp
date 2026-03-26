#pragma once

#include <algorithm>
#include <bits/stdc++.h>
#include <functional>
#include <memory>
#include <string>

namespace My {

/*
 * My (custom) implementation of C++'s unordered_map
 * https://en.cppreference.com/w/cpp/container/unordered_map.html
 * NOTE: I mostly ignore exception handling, sorry I don't 
 *       want this to be an extra 2000 lines
 *
 * Iterator invalidation
 * All read only operations, swap, std::swap - Never
 * clear, rehash, reserve, operator=         - Always
 * insert, emplace, emplace_hint, oeprator[] - Only if causes rehash
 * erase                                     - Only to the element erased
 */

constexpr std::size_t default_resize = 2; // Size of backing array after first resize

static constexpr std::array<int, 170> primes = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
    157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
    239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317,
    331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419,
    421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503,
    509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607,
    613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701,
    709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811,
    821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911,
    919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013
};

[[nodiscard]] static bool isPrime(int n) {
    if (n < 2) return false;

    for (int p : primes) {
        const std::int64_t pp = 1LL * p * p;
        if (pp > n)
            return true;
        if (n % p == 0)
            return n == p;
    }

    for (std::int64_t d = primes.back() + 2; d * d <= n; d += 2)
        if (n % d == 0)
            return false;

    return true;
}

[[nodiscard]] static int nextPrime(int n) {
    constexpr int kMax = std::numeric_limits<int>::max();

    if (n >= kMax)
        throw std::overflow_error("nextPrime: no prime exists above INT_MAX");

    if (n < 2)
        return 2;

    const int firstCandidate = (n == 2) ? 3 : (n % 2 == 0 ? n + 1 : n + 2);
    if (firstCandidate <= primes.back())
        return *std::lower_bound(primes.begin(), primes.end(), firstCandidate);

    int candidate = firstCandidate;
    while (candidate > 0) {
        if (isPrime(candidate))
            return candidate;

        if (candidate > kMax - 2)
            break;
        candidate += 2;
    }

    throw std::overflow_error("nextPrime: no prime exists in int range");
}

template <typename Key,
          typename T,
          typename Hash      = std::hash<Key>,
          typename KeyEqual  = std::equal_to<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
class unordered_map {
public:
    /*** C++ Standard Named Requirements for Containers***/
    using key_type        = Key;
    using mapped_type     = T;
    using value_type      = std::pair<const Key, T>;

    using hasher          = Hash;
    using key_equal       = KeyEqual;
    using allocator_type  = Allocator;
    using __alloc_traits  = std::allocator_traits<allocator_type>;

    using size_type       = typename __alloc_traits::size_type;
    using difference_type = typename __alloc_traits::difference_type;

    using reference       = value_type&;
    using const_reference = const value_type&;

    using pointer         = typename __alloc_traits::pointer;
    using const_pointer   = typename __alloc_traits::const_pointer;
    using iterator        = pointer;
    using const_iterator  = const_pointer;

    static_assert((std::is_same<typename allocator_type::value_type, value_type>::value),
                  "Allocator::value_type must be same type as value_type");


    /*** Constructors and Destructors***/
    // Default constructors, sets _max_load_factor() to 1.0
    unordered_map();
    explicit unordered_map(size_type bucket_count,
                           const Hash &hash = Hash(),
                           const key_equal &equal = key_equal(),
                           const Allocator &alloc = Allocator());

    unordered_map(size_type bucket_count, const Allocator &alloc)
        : unordered_map(bucket_count, Hash(), key_equal(), alloc) {}

    unordered_map(size_type bucket_count, const Hash &hash, const Allocator &alloc)
        : unordered_map(bucket_count, hash, key_equal(), alloc) {}

    explicit unordered_map(const Allocator &alloc);

    // Range constructors
    template<class InputIt>
    unordered_map(InputIt first, InputIt last,
                  size_type bucket_count = 0,
                  const Hash& hash = Hash(),
                  const key_equal& equal = key_equal(),
                  const Allocator& alloc = Allocator() );

    template<class InputIt>
    unordered_map(InputIt first, InputIt last, size_type bucket_count, const Allocator& alloc)
        : unordered_map(first, last, bucket_count, Hash(), key_equal(), alloc) {}

    template<class InputIt>
    unordered_map(InputIt first, InputIt last,
                  size_type bucket_count,
                  const Hash& hash,
                  const Allocator& alloc)
        : unordered_map(first, last, bucket_count, hash, key_equal(), alloc) {}

    // Copy constructor
    unordered_map(const unordered_map &other) 
        : unordered_map(other, std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.get_allocator())) {};
    unordered_map(const unordered_map &other, const Allocator &alloc);

    // TODO: Destructor looks good but need to check
    ~unordered_map() = default;

    allocator_type get_allocator() const noexcept { return _allocator; }

    /*** Lookup ***/
    T& at(const Key& key) {
        return const_cast<T&>(std::as_const(*this).at(key));
    }
    const T& at(const Key& key) const;

    T& operator[](const Key& key) { return subscriptHelper(key); }
    T& operator[](Key&& key) { return subscriptHelper(std::move(key)); }

    /*** Modifiers ***/
    void clear() noexcept {
        _buckets = std::make_unique<std::list<value_type>[]>(default_resize);
        _bucket_count = default_resize;
        _size = 0;
    }

    std::pair<iterator, bool> insert(const value_type &x) {
        return emplace_unique(std::forward<value_type>(x));
    }

    std::pair<iterator, bool> insert(value_type &&x) {
        return emplace_unique(std::move(x));
    }

    template <class P, std::__enable_if_t<std::is_constructible<value_type, P>::value, int> = 0>
    std::pair<iterator, bool> insert(P &&x) {
        return emplace_unique(std::forward<P>(x));
    }

    // I ignore the const_iterator portion due to the non homogeneous linked list backing array
    iterator insert(const_iterator, const value_type &x) { return insert(x).first; }
    iterator insert(const_iterator, value_type &&x) { return emplace_unique(std::move(x)).first; }

    template <class P, std::__enable_if_t<std::is_constructible<value_type, P>::value, int> = 0>
    iterator insert(const_iterator, P &&x) {
        return insert(std::forward<P>(x)).first;
    }

    template<class InputIt>
    void insert(InputIt first, InputIt last) {
        for (; first != last; ++first)
            emplace_unique(*first);
    }

    void insert(std::initializer_list<value_type> ilist) {
        insert(ilist.begin(), ilist.end());
    }

    template <class... _Args>
    std::pair<iterator, bool> emplace(_Args&&... __args) {
        return emplace_unique(std::forward<_Args>(__args)...);
    }

    iterator erase(const_iterator pos) {
        auto index = _hash_function(pos) % bucket_count();
    }

    iterator erase(const_iterator first, const_iterator last);
    size_type erase(const key_type& key);


    /*** Capacity ***/
    bool empty()         const noexcept { return !_size; }
    size_type size()     const noexcept { return _size; }
    size_type max_size() const noexcept { 
        return std::min<size_type>(
            _buckets.get().max_size(),
            std::numeric_limits<difference_type >::max()
        );
    }

    /*** Bucket interface ***/
    size_type bucket_count() const { return _bucket_count; }

    /*** Hash policy ***/
    float max_load_factor() const { return _max_load_factor; }
    void max_load_factor(float ml) { _max_load_factor = max_load_factor(); } ;
    void rehash(size_type count);
    
    // not defined in STL
    std::string toString() {
        std::string out;
        size_type idx = 0;

        for (; idx < _bucket_count; ++idx) {
            out += std::to_string(idx) + " : ";
            for (const auto &kv : _buckets[idx])
                out += std::to_string(kv.first) 
                    + "-" 
                    + std::to_string(kv.second) 
                    + (&kv != &_buckets[idx].back() ? ", " : " ");

            out.back() = '\n';
        }

        return out;
    }

private:
/* If you examine the LLVM Clang++ 21 unordered_map implementation you find
 * 4 member variables:
 *  __bucket_list                                         __bucket_list_;
 *  __compressed_pair<__first_node, __node_allocator>     __p1_;
 *  __compressed_pair<size_type, hasher>                  __p2_;
 *  __compressed_pair<float, key_equal>                   __p3_;
 *
 *  After a couple hours of digging,
 *
 *  __bucket_list is an array of pointers into the linked list where the index is a node's hash
 *  the __bucket_list delete contains the amount of buckets
 *  __p1_ is the pointer to the first node in the linked list, __bucket_list indexes into this
 *  __p2_ is a compressed pair of the number of elments and the hasher, if provided
 *  __p3_ is the max loadfactor (default 1.0) which is the number of entries / number of buckets
 *
 *  This is obviously too complex to implement so I will be sticking with 
 *  the STL's linked-list implementation
 */
    std::unique_ptr<std::list<value_type, allocator_type>[]> _buckets;
    size_type                                _bucket_count;
    size_type                                _size;
    float                                    _max_load_factor;
    hasher                                   _hash_function;
    key_equal                                _key_eq;
    allocator_type                           _allocator;

    /*** Private helpers ***/
    T& subscriptHelper(key_type &&key) {
        if (!_bucket_count) {
            int newBucketCount = nextPrime(bucket_count());
            rehash(newBucketCount);
        }

        auto index = _hash_function(key) % bucket_count();

        for (auto &node : _buckets[index])
            if (_key_eq(node.first, key))
                return node.second;

        if (size()+1 > bucket_count()*max_load_factor()) {
            int newBucketCount = nextPrime(bucket_count());
            rehash(newBucketCount);
        }

        index = _hash_function(key) % bucket_count();

        _buckets[index].push_front(value_type{key, T{}});
        ++_size;
        return _buckets[index].front().second;
    }

    std::pair<iterator, bool> emplace_unique(value_type &&x) {
        if (size()+1 > bucket_count()*max_load_factor()) {
            int newBucketCount = nextPrime(bucket_count());
            rehash(newBucketCount);
        }

        auto key = _hash_function(x.first);
        auto index = key % bucket_count();
        
        for (auto &node : _buckets[index])
            if (_key_eq(node.first, key)) 
                return std::pair{&node, false};

        _buckets[index].push_front(x);
        ++_size;
        return {&_buckets[index].front(), true};
    }
};

/*** Constructors ***/
template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
unordered_map<Key, T, Hash, KeyEqual, Allocator>
::unordered_map()
: _buckets(nullptr), _bucket_count(0), _size(0), _max_load_factor(1.f),
    _hash_function(hasher{}), _key_eq(key_equal{})
{/* Empty ctor */}

template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
unordered_map<Key, T, Hash, KeyEqual, Allocator>
::unordered_map(size_type bucket_count, 
                const Hash &hash, 
                const key_equal &equal, 
                const Allocator &alloc)
: _buckets(nullptr), _bucket_count(0), _size(0), _max_load_factor(1.f),
    _hash_function(hash), _key_eq(equal), _allocator(alloc)
{/* Empty ctor */}


template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
unordered_map<Key, T, Hash, KeyEqual, Allocator>
::unordered_map(const Allocator& alloc)
: _buckets(nullptr), _bucket_count(0), _size(0), _max_load_factor(1.f),
    _hash_function(hasher{}), _key_eq(key_equal{}), _allocator(alloc)
{/* Empty ctor */}

template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
template<class InputIt>
unordered_map<Key, T, Hash, KeyEqual, Allocator>
::unordered_map(InputIt first, InputIt last,
                size_type bucket_count,
                const Hash& hash,
                const key_equal& equal,
                const Allocator& alloc)
: _buckets(nullptr), _bucket_count(0), _size(0), _max_load_factor(1.f),
    _hash_function(hash), _key_eq(equal), _allocator(alloc)
{ insert(first, last); }

template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
unordered_map<Key, T, Hash, KeyEqual, Allocator>
::unordered_map(const unordered_map &other, const Allocator &alloc)
: _allocator(alloc)
{
    _buckets = std::make_unique<std::list<value_type>[]>(other._bucket_count);
    _bucket_count = other._bucket_count;
    _size = other.size();
    _max_load_factor = other.max_load_factor();
    _key_eq = other._key_eq;

    for (int i = 0; i < _bucket_count; ++i)
        _buckets[i] = other._buckets[i];
}

/*** Lookup ***/
template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
const T& unordered_map<Key, T, Hash, KeyEqual, Allocator>::at(const Key &key) const {
    if (empty())
        throw(std::out_of_range("Empty map"));

    for (auto &node : _buckets[_hash_function(key) % _bucket_count])
        if (node.first == key) 
            return node.second;

    throw(std::out_of_range("No key found"));
}

/*** Hash policy ***/
template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
void unordered_map<Key, T, Hash, KeyEqual, Allocator>::rehash(size_type n) {
    if (n < default_resize) n = default_resize;

    const size_type min_buckets =
        nextPrime(static_cast<size_type>(
            std::ceil(static_cast<double>(size() / max_load_factor()))));

    if (n < min_buckets)
        n = min_buckets;

    if (n == _bucket_count)
        return;

    auto newBuckets = std::make_unique<std::list<value_type>[]>(n);

    for (int i = 0; i < bucket_count(); ++i) {
        auto it = _buckets[i].begin();
        while (it != _buckets[i].end()) {
            auto curr = it++;
            auto newIndex = _hash_function(curr->first) % n;
            newBuckets[newIndex].splice(newBuckets[newIndex].begin(), _buckets[i], curr);
        }
    }

    std::swap(_buckets, newBuckets);
    _bucket_count = n;
}

} // namespace My

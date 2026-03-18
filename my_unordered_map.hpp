#pragma once

#include <bits/stdc++.h>
#include <functional>
#include <memory>

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

template <typename Key,
          typename T,
          typename Hash      = std::hash<Key>,
          typename KeyEqual  = std::equal_to<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
class unordered_map {
public:
    /*** C++ Standard Named Requirements for Containers***/
    using key_type    = Key;
    using mapped_type = T;
    using value_type  = std::pair<const Key, T>;

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
    // Default constructors, sets max_load_factor() to 1.0
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

    // Destructor
    ~unordered_map();

    /*** Modifiers ***/
    template<class InputIt>
    void insert(InputIt first, InputIt last);

    

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
    std::list<value_type> *buckets;
    size_type             bucket_count;
    size_type             total_elements;
    float                 max_load_factor;
    hasher                hash;
    key_equal             comparator;
    allocator_type        allocator;

};

template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
unordered_map<Key, T, Hash, KeyEqual, Allocator>
::unordered_map()
: buckets(nullptr), bucket_count(0), total_elements(0), max_load_factor(1.f),
    hash(hasher{}), comparator(key_equal{}), allocator(allocator_type{})
{/* Empty ctor */}

template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
unordered_map<Key, T, Hash, KeyEqual, Allocator>
::unordered_map(size_type bucket_count, 
                const Hash &hash, 
                const key_equal &equal, 
                const Allocator &alloc)
: buckets(nullptr), bucket_count(0), total_elements(0), max_load_factor(1.f),
    hash(hash), comparator(equal), allocator(alloc)
{/* Empty ctor */}


template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
unordered_map<Key, T, Hash, KeyEqual, Allocator>
::unordered_map(const Allocator& alloc)
: buckets(nullptr), bucket_count(0), total_elements(0), max_load_factor(1.f),
    hash(hasher{}), comparator(key_equal{}), allocator(alloc)
{/* Empty ctor */}

template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
template<class InputIt>
unordered_map<Key, T, Hash, KeyEqual, Allocator>
::unordered_map(InputIt first, InputIt last,
                size_type bucket_count,
                const Hash& hash,
                const key_equal& equal,
                const Allocator& alloc)
: buckets(nullptr), bucket_count(0), total_elements(0), max_load_factor(1.f),
    hash(hash), comparator(equal), allocator(alloc)
{ insert(first, last); }

template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
template<class InputIt>
void unordered_map<Key, T, Hash, KeyEqual, Allocator>
::insert(InputIt first, InputIt last) {
    const difference_type numOfElems = std::distance(first, last);
}

} // namespace My

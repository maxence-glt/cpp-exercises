#include <bits/stdc++.h>
#include <iterator>
#include <memory>

namespace My {

/*
 * My (custom) implementation of C++'s vector
 * https://en.cppreference.com/w/cpp/container/vector.html
 */

template <typename T, class Allocator = std::allocator<T>>
class vector {
public:
    /*** C++ Standard Named Requirements for Containers***/
    using value_type      = T;
    using reference       = T&;
    using const_reference = const T&;
    using iterator        = T*;
    using const_iterator  = const T*;
    using difference_type = std::ptrdiff_t;
    using size_type       = std::size_t;
    using allocator_type  = Allocator;

    /*** Constructors and Destructors***/
    // Default constructors
    vector() noexcept;
    explicit vector(const allocator_type &alloc);

    // Fill constructors
    explicit vector(size_type count, const allocator_type &alloc = Allocator());
    vector(size_type count, const_reference v, const allocator_type &alloc = Allocator());

    // Range constructor
    template <class InputIt,
              class Enable = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
    vector(InputIt first, InputIt last, const allocator_type &alloc = Allocator());

    // Copy constructors
    vector(const vector &other);
    vector(const vector &other, const allocator_type &alloc);

    // Move constructors
    vector(vector &&other);
    vector(vector &&other, const allocator_type &alloc);

    // List initializer
    vector(std::initializer_list<value_type> init,
           const Allocator &alloc = Allocator());

    // Destructor
    ~vector();

    /*** Assignment operators ***/
    vector &operator=(const vector &other);
    vector &operator=(vector &&other);
    void assign(size_type count, const_reference value);

    /*** Element access ***/
    reference at(size_type pos);
    const_reference at(size_type pos) const;
    reference  operator[](const size_type position) { return data[position]; } 
    const_reference operator[](const size_type position) const { return data[position]; }

    /*** Iterators ***/
    iterator       begin() noexcept { return data; }
    const_iterator begin() const noexcept { return data; }
    const_iterator cbegin() const noexcept { return data; }

    iterator       end() noexcept { return data + size_; }
    const_iterator end() const noexcept { return data + size_; }
    const_iterator cend() const noexcept { return data + size_; }

    /*** Capacity ***/
    bool empty()         const noexcept { return !size_; }
    size_type size()     const noexcept { return size_; }
    size_type capacity() const noexcept { return capacity_; }
    size_type max_size() const noexcept { 
        return std::allocator_traits<allocator_type>::max_size(allocator_); 
    }

    /*** Modifiers ***/
    void clear();
    iterator insert(const_iterator pos, const_reference value);
    iterator erase(iterator pos);
    void push_back(const_reference value);
    void pop_back();
    void resize(size_type count, const_reference value = size_type());

    /*** Other ***/
    allocator_type get_allocator() const { return allocator_; }


private:
    value_type*    data;
    size_type      capacity_;
    size_type      size_;
    allocator_type allocator_;

    void swap(vector &first, vector &second) {
        std::swap(first.size_, second.size_);
        std::swap(first.data, second.data);
        std::swap(first.capacity_, second.capacity_);
        std::swap(first.allocator_, second.allocator_);
    }

    void grow(size_type newCap) {
        auto temp = std::allocator_traits<allocator_type>::allocate(allocator_, newCap);

        int i = 0;
        auto from = begin(), to = end();
        for (; from != to && i < newCap; ++from, ++i) {
            std::allocator_traits<allocator_type>::construct(allocator_, 
                                                             temp + i, 
                                                             *from);
            std::allocator_traits<allocator_type>::destroy(allocator_,
                                                           from);
        }

        std::allocator_traits<allocator_type>::deallocate(allocator_, data, capacity_);

        data = temp;
        capacity_ = newCap;
    }
};

// Default constructors
template <typename T, class Allocator>
vector<T, Allocator>::vector() noexcept
: data(nullptr), capacity_(0), size_(0), allocator_(allocator_type{})
{ /* Empty ctor */ }


template <typename T, class Allocator>
vector<T, Allocator>::vector(const Allocator &alloc)
: data(nullptr), capacity_(0), size_(0), allocator_(alloc)
{ /* Empty constructor */}


// Fill constructors
// The only exception handling I'll do as an example
template <typename T, class Allocator>
vector<T, Allocator>::vector(size_type count,
                             const allocator_type &alloc)
: data(nullptr), capacity_(count), size_(0), allocator_(alloc)
{
    try {
        data = std::allocator_traits<allocator_type>::allocate(allocator_, capacity_);

        for (; size_ < count; ++size_)
            std::allocator_traits<allocator_type>::construct(allocator_, data + size_);
    } catch (...) {
        auto from = begin();
        auto to   = end();

        for(; from != to; ++from)
            std::allocator_traits<allocator_type>::destroy(allocator_, from);

        std::allocator_traits<allocator_type>::deallocate(allocator_, data, capacity_);

        throw;
    }
}


template <typename T, class Allocator>
vector<T, Allocator>::vector(size_type count, const_reference v,
                             const allocator_type &alloc)
: data(nullptr), capacity_(count), size_(0), allocator_(alloc)
{
    data = std::allocator_traits<allocator_type>::allocate(allocator_, capacity_);

    for (; size_ < count; ++size_)
        std::allocator_traits<Allocator>::construct(allocator_, data + size_, v);
}


// Range constructors
template <typename T, class Allocator>
template <class InputIt, class Enable>
vector<T, Allocator>::vector(InputIt first, InputIt last,
                             const allocator_type &alloc)
: size_(0), allocator_(alloc)
{
    const difference_type numOfElems = std::distance(first, last);

    capacity_ = numOfElems;
    data = std::allocator_traits<allocator_type>::allocate(allocator_, capacity_);

    for (; first != last; ++first, ++size_)
        std::allocator_traits<allocator_type>::construct(allocator_, data + size_, *first);
}


// Copy constructors
template <typename T, class Allocator>
vector<T, Allocator>::vector(const vector &other)
: data(nullptr), capacity_(other.capacity_), size_(0), allocator_(other.allocator_)
{
    data = std::allocator_traits<allocator_type>::allocate(allocator_, capacity_);

    for (; size_ < other.size_; ++size_)
        std::allocator_traits<Allocator>::construct(allocator_, data + size_, *(other.data + size_));
}

template <typename T, class Allocator>
vector<T, Allocator>::vector(const vector &other, const allocator_type &alloc)
: data(nullptr), capacity_(other.capacity_), size_(0), allocator_(alloc)
{
    data = std::allocator_traits<allocator_type>::allocate(allocator_, capacity_);

    for (; size_ < other.size_; ++size_)
        std::allocator_traits<Allocator>::construct(allocator_, data + size_, *(other.data + size_));
}


// Move constructors
template <typename T, class Allocator>
vector<T, Allocator>::vector(vector &&other)
: data(other.data), capacity_(other.capacity_), 
    size_(other.size_), allocator_(std::move(other.allocator_))
{
    other.size_ = 0;
    other.capacity_ = 0;
    other.data = nullptr;
}

template <typename T, class Allocator>
vector<T, Allocator>::vector(vector &&other, const allocator_type &alloc)
: data(nullptr), capacity_(0), size_(0), allocator_(alloc)
{
    using traits = std::allocator_traits<allocator_type>;
    if (allocator_ == other.allocator_) {
        data = other.data; size_ = other.size_; capacity_ = other.capacity_;
        other.data = nullptr; other.size_ = other.capacity_ = 0;
    } else {
        if (other.size_) {
            data = traits::allocate(allocator_, other.size_);
            capacity_ = size_ = other.size_;
            size_type i = 0;
            try {
                for (; i < size_; ++i)
                    traits::construct(allocator_, data + i, std::move(other.data[i]));
            } catch (...) {
                for (size_type j = 0; j < i; ++j) traits::destroy(allocator_, data + j);
                traits::deallocate(allocator_, data, capacity_);
                throw;
            }
        }
    }
}

// List initializer constructor
template <typename T, class Allocator>
vector<T, Allocator>::vector(std::initializer_list<value_type> init,
                             const Allocator &alloc)
: data(nullptr), capacity_(init.size()), size_(0), allocator_(alloc)
{
    data = std::allocator_traits<Allocator>::allocate(allocator_, capacity_);

    for(; size_ < init.size(); ++size_)
        std::allocator_traits<Allocator>::construct(allocator_, data + size_, *(init.begin() + size_));
}


// Destructor
template <typename T, class Allocator>
vector<T, Allocator>::~vector()
{
    auto from = begin();
    auto to = end();

    for (; from != to; ++from)
        std::allocator_traits<Allocator>::destroy(allocator_, from);

    std::allocator_traits<allocator_type>::deallocate(allocator_, data, capacity_);

    size_ = 0; capacity_ = 0;
    data = nullptr;
}

/*** Assignment operators ***/
// Copy-on-swap https://stackoverflow.com/a/3279550/21144460
template <typename T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(const vector &other)
{
    vector temp(other);
    swap(*this, temp);

    return *this;
}


template <typename T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(vector &&other)
{
    vector temp(std::move(other));
    swap(*this, temp);

    return *this;
}


template <typename T, class Allocator>
void vector<T, Allocator>::assign(size_type count, const_reference value)
{
    // TODO
}

/*** Element access ***/
template <typename T, class Allocator>
T& vector<T, Allocator>::at(size_type pos)
{
    if (pos < size_)
        return data[pos];

    throw(std::out_of_range("Index out-of-range"));
}

template <typename T, class Allocator>
const T& vector<T, Allocator>::at(size_type pos) const
{
    if (pos < size_)
        return data[pos];

    throw(std::out_of_range("Index out-of-range"));

}

template <typename T, class Allocator>
void vector<T, Allocator>::clear()
{
    for (auto from = begin(); from != end(); ++from)
        std::allocator_traits<allocator_type>::destroy(allocator_, from);
    size_ = 0;
}

template <typename T, class Allocator>
T* vector<T, Allocator>::insert(const_iterator pos, const_reference value)
{
    using traits = std::allocator_traits<allocator_type>;
    size_type idx = pos - cbegin();

    if (size_ == capacity_)
        grow(capacity_ ? capacity_ << 1 : 1);

    T* const out = data + idx;

    if (idx == size_) {
        traits::construct(allocator_, end(), value);
        ++size_;
        return out;
    }

    traits::construct(allocator_, end(), std::move(data[size_ - 1]));

    for (size_type i = size_ - 1; i > idx; --i)
        data[i] = std::move(data[i - 1]);

    data[idx] = value;

    ++size_;
    return out;
}

template <typename T, class Allocator>
void vector<T, Allocator>::push_back(const_reference value)
{
    if (size_ == capacity_)
        grow(!capacity_ ? 1 : capacity_ << 1);

    std::allocator_traits<allocator_type>::construct(allocator_, 
                                                     data + size_,
                                                     value);
    ++size_;
}


template <typename T, class Allocator>
void vector<T, Allocator>::pop_back()
{
    --size_;
    std::allocator_traits<allocator_type>::destroy(allocator_, end());
}


template <typename T, class Allocator>
T* vector<T, Allocator>::erase(iterator pos)
{
    using traits = std::allocator_traits<allocator_type>;
    size_type idx = pos - begin();

    T* const out = data + idx;

    --size_;
    for (size_type i = idx; i < size_; ++i)
        data[i] = std::move(data[i + 1]);

    traits::destroy(allocator_, end());

    return out;
}


template <typename T, class Allocator>
void vector<T, Allocator>::resize(size_type count, const_reference value)
{    
    using traits = std::allocator_traits<allocator_type>;
    if (count == size_) return;

    const size_type n_to_move = (size_ < count) ? size_ : count;

    auto new_data = traits::allocate(allocator_, count);
    auto cur = new_data;

    for (size_type i = 0; i < n_to_move; ++i, ++cur)
            traits::construct(allocator_, cur, std::move(data[i]));

    for (auto it = begin(); it != end(); ++it)
        traits::destroy(allocator_, it);

    for (size_type i = n_to_move; i < capacity_; ++i)
        traits::construct(allocator_, begin() + i, value);

    traits::deallocate(allocator_, data, capacity_);
    data = new_data;
    capacity_ = count;
    size_ = n_to_move;
}

}

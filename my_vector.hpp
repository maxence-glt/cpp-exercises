#pragma once

#include <bits/stdc++.h>
#include <memory>

namespace My {

/*
 * My (custom) implementation of C++'s vector
 * https://en.cppreference.com/w/cpp/container/vector.html
 * NOTE: I mostly ignore exception handling, sorry I don't 
 *       want this to be an extra 2000 lines
 */

template <typename T, class Allocator = std::allocator<T>>
class vector {
public:
    /*** C++ Standard Named Requirements for Containers***/
    using allocator_type  = Allocator;
    using __alloc_traits  = std::allocator_traits<Allocator>;
    using size_type       = typename __alloc_traits::size_type;

    using value_type      = T;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using difference_type = typename __alloc_traits::difference_type;
    using pointer         = typename __alloc_traits::pointer;
    using const_pointer   = typename __alloc_traits::const_pointer;
    using iterator        = pointer;
    using const_iterator  = const_pointer;

    static_assert((std::is_same<typename allocator_type::value_type, value_type>::value),
                  "Allocator::value_type must be same type as value_type");



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
    vector &operator=(std::initializer_list<value_type> ilist);

    void assign(size_type count, const_reference value);
    template <class InputIt,
              class Enable = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list<value_type> ilist);

    /*** Element access ***/
    reference at(size_type pos);
    const_reference at(size_type pos) const;

    reference  operator[](const size_type position) { return data_[position]; } 
    const_reference operator[](const size_type position) const { return data_[position]; }

    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }

    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }

    pointer data() { return data_; }
    const_pointer data() const { return data_; }

    /*** Iterators ***/
    iterator       begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }
    const_iterator cbegin() const noexcept { return data_; }

    iterator       end() noexcept { return data_ + size_; }
    const_iterator end() const noexcept { return data_ + size_; }
    const_iterator cend() const noexcept { return data_ + size_; }

    /*** Capacity ***/
    bool empty()         const noexcept { return !size_; }
    size_type size()     const noexcept { return size_; }
    size_type capacity() const noexcept { return capacity_; }
    size_type max_size() const noexcept { 
        return __alloc_traits::max_size(allocator_); 
    }

    /*** Modifiers ***/
    void clear();
    iterator insert(const_iterator pos, const_reference value);
    iterator erase(iterator pos);
    void push_back(const_reference value);
    void pop_back();
    void resize(size_type count, const_reference value = size_type());
    void swap(vector &other) {
        std::swap(size_, other.size_);
        std::swap(data_, other.data_);
        std::swap(capacity_, other.capacity_);
        std::swap(allocator_, other.allocator_);
    }


    /*** Other ***/
    allocator_type get_allocator() const { return allocator_; }


private:
    pointer        data_;
    size_type      capacity_;
    size_type      size_;
    allocator_type allocator_;


    void grow(size_type newCap) {
        auto temp = __alloc_traits::allocate(allocator_, newCap);

        int i = 0;
        auto from = begin(), to = end();

        for (; from != to && i < newCap; ++from, ++i) {
            __alloc_traits::construct(allocator_, temp + i, *from);
            __alloc_traits::destroy(allocator_, from);
        }

        __alloc_traits::deallocate(allocator_, data_, capacity_);

        data_ = temp;
        capacity_ = newCap;
    }
};

// Default constructors
template <typename T, class Allocator>
vector<T, Allocator>::vector() noexcept
: data_(nullptr), capacity_(0), size_(0), allocator_(allocator_type{})
{ /* Empty ctor */ }


template <typename T, class Allocator>
vector<T, Allocator>::vector(const Allocator &alloc)
: data_(nullptr), capacity_(0), size_(0), allocator_(alloc)
{ /* Empty constructor */}


// Fill constructors
// The only exception handling I'll do as an example
template <typename T, class Allocator>
vector<T, Allocator>::vector(size_type count,
                             const allocator_type &alloc)
: data_(nullptr), capacity_(count), size_(0), allocator_(alloc)
{
    try {
        data_ = __alloc_traits::allocate(allocator_, capacity_);

        for (; size_ < count; ++size_)
            __alloc_traits::construct(allocator_, data_ + size_);
    } catch (...) {
        auto from = begin();
        auto to   = end();

        for(; from != to; ++from)
            __alloc_traits::destroy(allocator_, from);

        __alloc_traits::deallocate(allocator_, data_, capacity_);

        throw;
    }
}


template <typename T, class Allocator>
vector<T, Allocator>::vector(size_type count, const_reference v,
                             const allocator_type &alloc)
: data_(nullptr), capacity_(count), size_(0), allocator_(alloc)
{
    data_ = std::allocator_traits<allocator_type>::allocate(allocator_, capacity_);

    for (; size_ < count; ++size_)
        std::allocator_traits<Allocator>::construct(allocator_, data_ + size_, v);
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
    data_ = std::allocator_traits<allocator_type>::allocate(allocator_, capacity_);

    for (; first != last; ++first, ++size_)
        std::allocator_traits<allocator_type>::construct(allocator_, data_ + size_, *first);
}


// Copy constructors
template <typename T, class Allocator>
vector<T, Allocator>::vector(const vector &other)
: data_(nullptr), capacity_(other.capacity_), size_(0), allocator_(other.allocator_)
{
    data_ = std::allocator_traits<allocator_type>::allocate(allocator_, capacity_);

    for (; size_ < other.size_; ++size_)
        std::allocator_traits<Allocator>::construct(allocator_, data_ + size_, *(other.data_ + size_));
}

template <typename T, class Allocator>
vector<T, Allocator>::vector(const vector &other, const allocator_type &alloc)
: data_(nullptr), capacity_(other.capacity_), size_(0), allocator_(alloc)
{
    data_ = std::allocator_traits<allocator_type>::allocate(allocator_, capacity_);

    for (; size_ < other.size_; ++size_)
        std::allocator_traits<Allocator>::construct(allocator_, data_ + size_, *(other.data_ + size_));
}


// Move constructors
template <typename T, class Allocator>
vector<T, Allocator>::vector(vector &&other)
: data_(other.data_), capacity_(other.capacity_), 
    size_(other.size_), allocator_(std::move(other.allocator_))
{
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
}

template <typename T, class Allocator>
vector<T, Allocator>::vector(vector &&other, const allocator_type &alloc)
: data_(nullptr), capacity_(0), size_(0), allocator_(alloc)
{
    using traits = std::allocator_traits<allocator_type>;
    if (allocator_ == other.allocator_) {
        data_ = other.data_; size_ = other.size_; capacity_ = other.capacity_;
        other.data_ = nullptr; other.size_ = other.capacity_ = 0;
    } else {
        if (other.size_) {
            data_ = traits::allocate(allocator_, other.size_);
            capacity_ = size_ = other.size_;
            size_type i = 0;
            try {
                for (; i < size_; ++i)
                    traits::construct(allocator_, data_ + i, std::move(other.data_[i]));
            } catch (...) {
                for (size_type j = 0; j < i; ++j) traits::destroy(allocator_, data_ + j);
                traits::deallocate(allocator_, data_, capacity_);
                throw;
            }
        }
    }
}

// List initializer constructor
template <typename T, class Allocator>
vector<T, Allocator>::vector(std::initializer_list<value_type> init,
                             const Allocator &alloc)
: data_(nullptr), capacity_(init.size()), size_(0), allocator_(alloc)
{
    data_ = std::allocator_traits<Allocator>::allocate(allocator_, capacity_);

    for(; size_ < init.size(); ++size_)
        std::allocator_traits<Allocator>::construct(allocator_, data_ + size_, *(init.begin() + size_));
}


// Destructor
template <typename T, class Allocator>
vector<T, Allocator>::~vector()
{
    auto from = begin();
    auto to = end();

    for (; from != to; ++from)
        std::allocator_traits<Allocator>::destroy(allocator_, from);

    std::allocator_traits<allocator_type>::deallocate(allocator_, data_, capacity_);

    size_ = 0; capacity_ = 0;
    data_ = nullptr;
}

/*** Assignment operators ***/
// Copy-on-swap https://stackoverflow.com/a/3279550/21144460
template <typename T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(const vector &other)
{
    vector temp(other);
    swap(temp);

    return *this;
}


template <typename T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(vector &&other)
{
    vector temp(std::move(other));
    swap(temp);

    return *this;
}


template <typename T, class Allocator>
vector<T, Allocator>&
vector<T, Allocator>::operator=(std::initializer_list<value_type> ilist)
{
    vector temp(ilist);
    swap(temp);

    return *this;
}


template <typename T, class Allocator>
void vector<T, Allocator>::assign(size_type count, const_reference value)
{
    clear();

    if (count > capacity_) {
        __alloc_traits::deallocate(allocator_, data_, capacity_);
        data_ = __alloc_traits::allocate(allocator_, count);
        capacity_ = count;
    }

    for (; size_ < count; ++size_)
        __alloc_traits::construct(allocator_, begin() + size_, value);

}

template <typename T, class Allocator>
template <class InputIt, class Enable>
void vector<T, Allocator>::assign(InputIt first, InputIt last)
{
    clear();

    const difference_type numOfElems = std::distance(first, last);

    if (numOfElems > capacity_) {
        __alloc_traits::deallocate(allocator_, data_, capacity_);
        data_ = __alloc_traits::allocate(allocator_, numOfElems);
        capacity_ = numOfElems;
    }

    for (; first != last; ++first, ++size_)
        std::allocator_traits<allocator_type>::construct(allocator_, data_ + size_, *first);
}

template <typename T, class Allocator>
void vector<T, Allocator>::assign(std::initializer_list<value_type> ilist)
{
    clear();

    if (ilist.size() > capacity_) {
        __alloc_traits::deallocate(allocator_, data_, capacity_);
        data_ = __alloc_traits::allocate(allocator_, ilist.size());
        capacity_ = ilist.size();
    }

    size_ = ilist.size();
    for (const auto &i : ilist)
        std::allocator_traits<allocator_type>::construct(allocator_, data_ + size_, i);
}

/*** Element access ***/
template <typename T, class Allocator>
T& vector<T, Allocator>::at(size_type pos)
{
    if (pos < size_)
        return data_[pos];

    throw(std::out_of_range("Index out-of-range"));
}

template <typename T, class Allocator>
const T& vector<T, Allocator>::at(size_type pos) const
{
    if (pos < size_)
        return data_[pos];

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
typename vector<T, Allocator>::iterator 
vector<T, Allocator>::insert(const_iterator pos, const_reference value)
{
    using traits = std::allocator_traits<allocator_type>;
    size_type idx = pos - cbegin();

    if (size_ == capacity_)
        grow(capacity_ ? capacity_ << 1 : 1);

    T* const out = data_ + idx;

    if (idx == size_) {
        traits::construct(allocator_, end(), value);
        ++size_;
        return out;
    }

    traits::construct(allocator_, end(), std::move(data_[size_ - 1]));

    for (size_type i = size_ - 1; i > idx; --i)
        data_[i] = std::move(data_[i - 1]);

    data_[idx] = value;

    ++size_;
    return out;
}

template <typename T, class Allocator>
void vector<T, Allocator>::push_back(const_reference value)
{
    if (size_ == capacity_)
        grow(!capacity_ ? 1 : capacity_ << 1);

    std::allocator_traits<allocator_type>::construct(allocator_, 
                                                     data_ + size_,
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
typename vector<T, Allocator>::iterator 
vector<T, Allocator>::erase(iterator pos)
{
    using traits = std::allocator_traits<allocator_type>;
    size_type idx = pos - begin();

    T* const out = data_ + idx;

    --size_;
    for (size_type i = idx; i < size_; ++i)
        data_[i] = std::move(data_[i + 1]);

    traits::destroy(allocator_, end());

    return out;
}


template <typename T, class Allocator>
void vector<T, Allocator>::resize(size_type count, const_reference value)
{    
    using traits = std::allocator_traits<allocator_type>;
    if (count == size_) return;

    const size_type n_to_move = (size_ < count) ? size_ : count;

    auto new_data_ = traits::allocate(allocator_, count);
    auto cur = new_data_;

    for (size_type i = 0; i < n_to_move; ++i, ++cur)
            traits::construct(allocator_, cur, std::move(data_[i]));

    for (auto it = begin(); it != end(); ++it)
        traits::destroy(allocator_, it);

    for (size_type i = n_to_move; i < capacity_; ++i)
        traits::construct(allocator_, begin() + i, value);

    traits::deallocate(allocator_, data_, capacity_);
    data_ = new_data_;
    capacity_ = count;
    size_ = n_to_move;
}

}

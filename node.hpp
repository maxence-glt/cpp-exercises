// code i scraped from my unordered_map implementation but I may use when implementing std::list

/*
// https://stackoverflow.com/questions/65262899/what-is-the-purpose-of-pointer-rebind
// The hash_node code is taken (and simplified) from the LLVM project
template <typename NodePtr>
struct hash_node_base {
    using node_type         = typename std::pointer_traits<NodePtr>::element_type;
    using first_node        = hash_node_base;
    using node_base_pointer = typename std::pointer_traits<NodePtr>::template rebind<first_node>;
    using node_pointer      = NodePtr;

    using next_pointer = 
        typename std::conditional<std::is_pointer<node_pointer>::value,
                                  node_base_pointer,
                                  node_pointer>::type;

    next_pointer next;

    next_pointer ptr() noexcept {
        return static_cast<next_pointer>(
            std::pointer_traits<node_base_pointer>::pointer_to(*this));
    }

    node_pointer upcast() noexcept {
        return static_cast<node_pointer>(
            std::pointer_traits<node_base_pointer>::pointer_to(*this));
    }

    size_t hash() const noexcept {
        return static_cast<node_type const&>(*this).__hash_;
    }

    hash_node_base() noexcept : next(nullptr) {}
};

template <class Tp, class VoidPtr>
struct hash_node 
    : public hash_node_base
        <
            typename std::pointer_traits<VoidPtr>::template rebind<hash_node<Tp, VoidPtr>>
        >
{
    using node_value_type = Tp;

    size_t          hash;
    node_value_type value;
};

template <class _NodePtr, class NodeT = typename std::pointer_traits<_NodePtr>::element_type>
struct hash_node_types;

template <class _NodePtr, class _Tp, class _VoidPtr>
struct hash_node_types<_NodePtr, hash_node<_Tp, _VoidPtr> >
    : public hash_key_value_types<_Tp>, hash_map_pointer_types<_Tp, _VoidPtr>

{
  typedef __hash_key_value_types<_Tp>           __base;

public:
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;

  typedef typename __rebind_pointer<_NodePtr, void>::type       __void_pointer;

  typedef typename pointer_traits<_NodePtr>::element_type       __node_type;
  typedef _NodePtr                                              __node_pointer;

  typedef __hash_node_base<__node_pointer>                      __node_base_type;
  typedef typename __rebind_pointer<_NodePtr, __node_base_type>::type
                                                             __node_base_pointer;

  typedef typename __node_base_type::__next_pointer          __next_pointer;

  typedef _Tp                                                 __node_value_type;
  typedef typename __rebind_pointer<_VoidPtr, __node_value_type>::type
                                                      __node_value_type_pointer;
  typedef typename __rebind_pointer<_VoidPtr, const __node_value_type>::type
                                                __const_node_value_type_pointer;

private:
    static_assert(!std::is_const<__node_type>::value,
                "_NodePtr should never be a pointer to const");
    static_assert((std::is_same<typename std::pointer_traits<_VoidPtr>::element_type, void>::value),
                  "_VoidPtr does not point to unqualified void type");
    static_assert((std::is_same<typename __rebind_pointer<_VoidPtr, __node_type>::type,
                          _NodePtr>::value), "_VoidPtr does not rebind to _NodePtr.");
};

template <class NodeValueTp, class VoidPtr>
struct make_hash_node_types {
    using NodeTp  = hash_node<NodeValueTp, VoidPtr>;
    using NodePtr = typename std::pointer_traits<VoidPtr>::template rebind_pointer<NodeTp>;
    using type    = hash_node_types<NodePtr>;
};
*/

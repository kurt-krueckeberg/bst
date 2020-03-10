/*
See GNU C++ implementation uses 

  template<typename _Tp> struct _Rb_tree_iterator {/...}; 

from /usr/include/c++/9/bits/stl_tree.h (copied locally), which in turn uses two non-member increment and decrement method,
which can be found in the subdirectory libstdc++-v3 of the g++ git repository at https://github.com/gcc-mirror/gcc
*/
/*
template<typename _Tp>
struct _Rb_tree_iterator {

  typedef _Tp  value_type;
  typedef _Tp& reference;
  typedef _Tp* pointer;

  typedef bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t			 difference_type;

  typedef _Rb_tree_iterator<_Tp>		_Self;
  typedef _Rb_tree_node_base::_Base_ptr	_Base_ptr;
  typedef _Rb_tree_node<_Tp>*		_Link_type;

  _Rb_tree_iterator() _GLIBCXX_NOEXCEPT
  : _M_node() { }

  explicit
  _Rb_tree_iterator(_Base_ptr __x) _GLIBCXX_NOEXCEPT
  : _M_node(__x) { }

  reference
  operator*() const _GLIBCXX_NOEXCEPT
  { return *static_cast<_Link_type>(_M_node)->_M_valptr(); }

  pointer
  operator->() const _GLIBCXX_NOEXCEPT
  { return static_cast<_Link_type> (_M_node)->_M_valptr(); }

  _Self&
  operator++() _GLIBCXX_NOEXCEPT
  {
    _M_node = _Rb_tree_increment(_M_node);
    return *this;
  }

  _Self
  operator++(int) _GLIBCXX_NOEXCEPT
  {
    _Self __tmp = *this;
    _M_node = _Rb_tree_increment(_M_node);
    return __tmp;
  }

  _Self&
  operator--() _GLIBCXX_NOEXCEPT
  {
    _M_node = _Rb_tree_decrement(_M_node);
    return *this;
  }

  _Self
  operator--(int) _GLIBCXX_NOEXCEPT
  {
    _Self __tmp = *this;
    _M_node = _Rb_tree_decrement(_M_node);
    return __tmp;
  }

  friend bool
  operator==(const _Self& __x, const _Self& __y) _GLIBCXX_NOEXCEPT
  { return __x._M_node == __y._M_node; }

  friend bool
  operator!=(const _Self& __x, const _Self& __y) _GLIBCXX_NOEXCEPT
  { return __x._M_node != __y._M_node; }

  _Base_ptr _M_node;
};

template<typename _Tp> struct _Rb_tree_const_iterator {

  typedef _Tp	 value_type;
  typedef const _Tp& reference;
  typedef const _Tp* pointer;

  typedef _Rb_tree_iterator<_Tp> iterator;

  typedef bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t			 difference_type;

  typedef _Rb_tree_const_iterator<_Tp>		_Self;
  typedef _Rb_tree_node_base::_Const_Base_ptr	_Base_ptr;
  typedef const _Rb_tree_node<_Tp>*			_Link_type;

  _Rb_tree_const_iterator() _GLIBCXX_NOEXCEPT
  : _M_node() { }

  explicit
  _Rb_tree_const_iterator(_Base_ptr __x) _GLIBCXX_NOEXCEPT
  : _M_node(__x) { }

  _Rb_tree_const_iterator(const iterator& __it) _GLIBCXX_NOEXCEPT
  : _M_node(__it._M_node) { }

  iterator
  _M_const_cast() const _GLIBCXX_NOEXCEPT
  { return iterator(const_cast<typename iterator::_Base_ptr>(_M_node)); }

  reference
  operator*() const _GLIBCXX_NOEXCEPT
  { return *static_cast<_Link_type>(_M_node)->_M_valptr(); }

  pointer
  operator->() const _GLIBCXX_NOEXCEPT
  { return static_cast<_Link_type>(_M_node)->_M_valptr(); }

  _Self&
  operator++() _GLIBCXX_NOEXCEPT
  {
    _M_node = _Rb_tree_increment(_M_node);
    return *this;
  }

  _Self
  operator++(int) _GLIBCXX_NOEXCEPT
  {
    _Self __tmp = *this;
    _M_node = _Rb_tree_increment(_M_node);
    return __tmp;
  }

  _Self&
  operator--() _GLIBCXX_NOEXCEPT
  {
    _M_node = _Rb_tree_decrement(_M_node);
    return *this;
  }

  _Self
  operator--(int) _GLIBCXX_NOEXCEPT
  {
    _Self __tmp = *this;
    _M_node = _Rb_tree_decrement(_M_node);
    return __tmp;
  }

  friend bool
  operator==(const _Self& __x, const _Self& __y) _GLIBCXX_NOEXCEPT
  { return __x._M_node == __y._M_node; }

  friend bool
  operator!=(const _Self& __x, const _Self& __y) _GLIBCXX_NOEXCEPT
  { return __x._M_node != __y._M_node; }

  _Base_ptr _M_node;
};
*/


// increment
static _Rb_tree_node_base*
_Rb_tree_increment(_Rb_tree_node_base* __x) throw ()
{
    if (__x->_M_right != 0) {
  
        __x = __x->_M_right;
  
        while (__x->_M_left != 0)
          __x = __x->_M_left;
  
    } else {
  
        _Rb_tree_node_base* __y = __x->_M_parent;
  
        while (__x == __y->_M_right) {
  
            __x = __y;
            __y = __y->_M_parent;
        }
  
        if (__x->_M_right != __y)
            __x = __y;
    }
    return __x;
}

// decrement
static _Rb_tree_node_base*
_Rb_tree_decrement(_Rb_tree_node_base* __x) throw ()
{
    if (__x->_M_color == _S_red && __x->_M_parent->_M_parent == __x)
        __x = __x->_M_right;
  
    else if (__x->_M_left != 0) {
  
        _Rb_tree_node_base* __y = __x->_M_left;
  
        while (__y->_M_right != 0)
          __y = __y->_M_right;
  
        __x = __y;
  
    } else {
  
        _Rb_tree_node_base* __y = __x->_M_parent;
  
        while (__x == __y->_M_left) {
            __x = __y;
            __y = __y->_M_parent;
        }
  
        __x = __y;
    }
    return __x;
}

class iterator_inorder {  // This not efficient to copy due to the stack container inside it.

   using node_type = bstree<Key, Value>::node_type;

   node_type *current;

   const bstree<Key, Value>& tree;

   // See libc++ source code for rb_iterator.
   iterator_inorder& increment() noexcept // Go to next node.
   {
     // case 1: is leaf
     if (current->is_leaf()) 
         if (current == tree.root.get()) return *this; // root is leaf node
         else {

        }
      else { // current is internal node

      }    
      return *this;
   }

  public:

   using difference_type  = std::ptrdiff_t; 
   using value_type       = bstree<Key, Value>::value_type; 
   using reference        = value_type&; 
   using pointer          = value_type*;
       
   using iterator_category = std::forward_iterator_tag; 

   explicit iterator_inorder(bstree<Key, Value>& bstree) : tree{bstree}
   {
      current = bstree.root.get();
      while(current->left) 
         current->left.get();
   }
   
   iterator_inorder(const iterator_inorder& lhs) : current{lhs.current}, tree{lhs.tree}
   {
   }
   
   iterator_inorder(iterator_inorder&& lhs) : current{lhs.current}, stack{std::move(lhs.stack)}, tree{lhs.tree}
   {
       lhs.current = nullptr;
   }
   
   iterator_inorder& operator++() noexcept 
   {
      increment();
      return *this;
   } 
   
   iterator_inorder operator++(int) noexcept
   {
      iterator_inorder tmp(*this);

      increment();

      return tmp;
   } 
     
   reference operator*() const noexcept 
   { 
       return current->__get_value();
   } 
   
   pointer operator->() const noexcept
   { 
      return &(operator*()); 
   } 
   
   struct sentinel {}; // Use for determining "at the end" in 'bool operator==(const iterator_inorder&) const' below

   bool operator==(const iterator_inorder::sentinel& sent) const noexcept
   {
      return stack.empty(); // We are done iterating when the stack becomes empty.
   }
   
   bool operator!=(const iterator_inorder::sentinel& lhs) const noexcept
   {
     return !operator==(lhs);    
   }
};
iterator_inorder begin() noexcept
{
   iterator_inorder iter{*this}; 
   return iter; 
}

iterator_inorder::sentinel end() noexcept // TODO: Can I use a sentinel? a C++17 feature.
{
    typename iterator_inorder::sentinel sent;
    return sent;
}


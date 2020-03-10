/*
See GNU C++ implementation uses 

  template<typename _Tp> struct _Rb_tree_iterator {/...}; 

from /usr/include/c++/9/bits/stl_tree.h (copied locally), which in turn uses two non-member increment and decrement method,
which can be found in the subdirectory libstdc++-v3 of the g++ git repository at https://github.com/gcc-mirror/gcc

 class const_iterator:

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

static _Rb_tree_node_base*
_Rb_tree_increment() 
{
  Node *__y = current;

    if (__y->right) { // There is a right child, a greater value to the right
  
        __y = __y->right.get();
  
        while (__y->left) // Get the last value in its left subtree.
           __y = __y->left.get();
  
    } else {
  
        auto parent = __y->parent;

        // Ascend to the first parent that is not a right-most child, 
        // and thus is greater than __y 
        while (__y == parent->right.get()) {

            if (parent == tree.root.get()) // We reached the root -> there is not successor
                return current;
 
            __y = parent;

            parent = parent->parent;
        }
    }

    return __y;
}

// decrement
static _Rb_tree_node_base*
_Rb_tree_decrement() throw ()
{
   auto __x = current;
 
   if (__x->left) { // There is a left child, a left subtree.
  
        auto __y = __x->left;
  
        while (__y->right) // Get its largest value
          __y = __y->right;
  
        __x = __y;
  
    } else {
  
        auto parent = __x->parent;

        // Ascend to first parent ancestor that is not a left child
        // and thus is less than __x.
        while (__x == parent->left.get()) {

           if (parent == tree.root.get()) // The parent is the root -> there is no predecessor.
               return current;

            __x = parent;
            parent = parent->parent;
        }
  
        __x = parent; // Set __x to first parent less than __x.
    }
    return __x;
}
*/

// TODO: Make this class a friend of bstree.
class iterator_inorder {  // This not efficient to copy due to the stack container inside it.

   using node_type = bstree<Key, Value>::node_type;

   node_type *current;

   bstree<Key, Value>& tree;

   //enum class position : char { start, in_between, end };  
   position pos;

   Node *increment() 
   {
     Node *__y = current;
   
       if (__y->right) { // current has a right child, a greater value to the right
     
           __y = __y->right.get();
     
           while (__y->left) // Get the smallest value in its right subtree, the smallest value in the r. subtree.
              __y = __y->left.get();
     
       } else {
     
           auto parent = __y->parent;
   
           // Ascend to the first parent ancestor that is not a right child, 
           // and thus is greater than __y 
           while (__y == parent->right.get()) {
   
               if (parent == tree.root.get()) // We reached the root -> there is no successor
                   return current;
    
               __y = parent;
   
               parent = parent->parent;
           }

           __y = parent; // First parent ancestor that is not a right child. 
       }
   
       return __y;
   }

   // decrement
   Node *decrement()
   {
      auto __x = current;
    
      if (__x->left) { // There is a left child, a left subtree.
     
           auto __y = __x->left;
     
           while (__y->right) // Get its largest value. This is the predecessor to current.
             __y = __y->right;
     
           __x = __y;
     
       } else {
     
           auto parent = __x->parent;
   
           // Ascend to first parent ancestor that is not a left child
           // and thus is less than __x.
           while (__x == parent->left.get()) {
   
              if (parent == tree.root.get()) // The parent is the root -> there is no predecessor.
                  return current;
   
               __x = parent;
               parent = parent->parent;
           }
     
           __x = parent; // Set __x to first parent less than __x.
       }
        
       return __x;
   }
   
  public:

   using difference_type  = std::ptrdiff_t; 
   using value_type       = bstree<Key, Value>::value_type; 
   using reference        = value_type&; 
   using pointer          = value_type*;
       
   using iterator_category = std::bidirectional_iterator_tag; 

   explicit iterator_inorder(bstree<Key, Value>& bstree) : tree{bstree}
   {
      // Set current to nodee with smallest key.
      auto __y = bstree.root.get();

      while(__y->left) 
         __y->left.get();

      current = __y;
      pos = position::start;
   }
   
   iterator_inorder(const iterator_inorder& lhs) : current{lhs.current}, tree{lhs.tree}, pos{lhs.pos}
   {
   }
   
   iterator_inorder(iterator_inorder&& lhs) : current{lhs.current}, tree{lhs.tree}
   {
       lhs.current = nullptr;
       lhs.pos = position::end;
   }
   
   iterator_inorder& operator++() noexcept 
   {
      current = increment();
      return *this;
   } 
   
   iterator_inorder operator++(int) noexcept
   {
      iterator_inorder tmp(*this);

      current = increment();

      return tmp;
   } 
    
   iterator_inorder& operator--() noexcept 
   {
      current = decrement();
      return *this;
   } 
   
   iterator_inorder operator--(int) noexcept
   {
      iterator_inorder tmp(*this);

      current = decrement();

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
      return ????; 
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


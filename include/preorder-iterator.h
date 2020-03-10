

// TODO: Make this class a friend of bstree.
class iterator_preorder {  // This not efficient to copy due to the stack container inside it.

   using node_type = bstree<Key, Value>::node_type;

   node_type *current;

   bstree<Key, Value>& tree;

   //enum class position : char { start, in_between, end };  
   position pos;

   Node *increment() 
   {
     Node *__y = current;

     if (__y->left) // Prefer left child
         __y = __y->left.get();

     else if (__y->right) // otherwise, the right 
         __y = __y->right.get();

     else  { // current is a leaf node, find parent that is left childIf leaf node, ascend until ? 
         auto parent = __y->parent;
         while (__y == parent->right.get()) {
		 
		 // TODO: special case when the parent is the root.
		 __y = parent;
                 parent = __y->parent;
	 }
	 __y = parent; // __y is now ancestor parent and current is in its left subtree.
	 if (__y->right)
             __y = __y->right.get();
	 else    
             __y = current; // There is no next pre-order node.		   
     }     
     return __y;
   }
   
  public:

   using difference_type  = std::ptrdiff_t; 
   using value_type       = bstree<Key, Value>::value_type; 
   using reference        = value_type&; 
   using pointer          = value_type*;
       
   using iterator_category = std::forward_iterator_tag; 

   explicit iterator_preorder(bstree<Key, Value>& bstree) : tree{bstree}
   {
      // Set current to nodee with smallest key.
      current = bstree.root.get();

   }
   
   iterator_preorder(const iterator_preorder& lhs) : current{lhs.current}, tree{lhs.tree}, pos{lhs.pos}
   {
   }
   
   iterator_preorder(iterator_preorder&& lhs) : current{lhs.current}, tree{lhs.tree}
   {
       lhs.current = nullptr;
   }
   
   iterator_preorder& operator++() noexcept 
   {
      current = increment();
      return *this;
   } 
   
   iterator_preorder operator++(int) noexcept
   {
      iterator_preorder tmp(*this);

      current = increment();

      return tmp;
   } 
          
   reference operator*() const noexcept 
   { 
       return current->__get_value(); // May want 'Node *' itself
   } 
   
   pointer operator->() const noexcept
   { 
      return &(operator*()); 
   } 
   
   struct sentinel {}; // Use for determining "at the end" in 'bool operator==(const iterator_preorder&) const' below

   bool operator==(const iterator_preorder::sentinel& sent) const noexcept
   {
      return (current == increment()) ? true: false; 
   }
   
   bool operator!=(const iterator_preorder::sentinel& lhs) const noexcept
   {
     return !operator==(lhs);    
   }
};
iterator_preorder begin() noexcept
{
   iterator_preorder iter{*this}; 
   return iter; 
}

iterator_preorder::sentinel end() noexcept // TODO: Can I use a sentinel? a C++17 feature.
{
    typename iterator_preorder::sentinel sent;
    return sent;
}


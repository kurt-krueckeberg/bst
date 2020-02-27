/* 
 * Converted Pre-order Java iterator. This is NOT an in-order iterator!!
 *
 * The Binary Trees lecture is at:
 * 
 * https://www.cs.cmu.edu/~adamchik/15-121/lectures/Trees/trees.html
 *
 * The BST code with the Pre-order iterator class was taken from:
 */
class preorder_stack_iterator {
    
   using node_type = bstree<Key, Value>::node_type;
   
   std::stack<node_type*> stack; 
   
   node_type *current;

   const bstree<Key, Value>& tree;

   iterator& increment() noexcept 
   {
      Node *current = stack.top();
      
      if (current->left)       // If left not nullptr, push it onto stack.  
      
          stack.push(current->left.get());
      
      else {
      
          Node *tmp = stack.top();

          stack.pop(); 
      
          while (!tmp->right) {  // While tmp->right is nullptr
      
             if (stack.empty()) 
                  return *this;  // current is properly set
      
             tmp = stack.top();
             stack.pop();  
          }
      
          stack.push(tmp->right.get());
      }
      
      return *this;  
   }  

 public:
   // traits for forward iterator
   using difference_type  = std::ptrdiff_t; 
   using value_type       = tree234<Key, Value>::value_type; 
   using reference        = value_type&; 
   using pointer          = value_type*;
   
   using iterator_category = std::forward_iterator_tag; 
	
   preorder_stack_iterator(const bstree<Key, Value>& lhs) : tree{lhs}
   {
      if (tree.root) {

          stack.push(root);
          increment();  // Go to first node to retrieve
      }
   }
 
   preorder_stack_iterator& operator++() noexcept 
   {
      increment();
      return *this;
   } 
   
   preorder_stack_iterator operator++(int) noexcept
   {
      preorder_stack_iterator tmp(*this);
   
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

   struct sentinel {};

   bool operator==(const sentinel& sent) const noexcept
   {
      return !stack.empty();
   }

   bool operator!=(const sentinel& sent) const noexcept
   {
      return !operator==(sent);
   }

   value_type& operator*() 
   {
       return current->__get_value();
   }
};   

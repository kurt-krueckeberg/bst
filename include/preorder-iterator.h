/* 
 * Converted Pre-order Java iterator. This is NOT an in-order iterator!!
 *
 * From Carneige Mellon 'Binary Trees' lecture: https://www.cs.cmu.edu/~adamchik/15-121/lectures/Trees/trees.html
 *
 * with accompanying source: https://www.cs.cmu.edu/~adamchik/15-121/lectures/Trees/code/BST.java
 */
class preorder_iterator {

   std::stack<node_type *> stack;

   const bstree<Key, Value>& tree;

   Node *current;

 public:
   // traits for forward iterator
   using difference_type  = std::ptrdiff_t; 
   using value_type       = tree234<Key, Value>::value_type; 
   using reference        = value_type&; 
   using pointer          = value_type*;
   
   using iterator_category = std::forward_iterator_tag; 
	
   preorder_iterator(const bstree<Key, Value>& lhs) : tree{lhs}
   {
      if (tree.root) {

          stack.push(root);

          increment();
      }
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
                 return *this;  // current->__get_value();
      
            tmp = stack.top();
            stack.pop();  
         }
      
         stack.push(tmp->right.get());
      }
      
      return *this;           //  current->__get_value();
   }

   value_type& operator*() 
   {
       return current->__get_value();
   }
   

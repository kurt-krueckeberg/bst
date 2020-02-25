#include <iterator>

// Make this a nested class.

class inorder_iterator {

   using node_type = bstree<Key, Value>::value_type;

   std::stack<node_type*> stack;

   node_type *current;

   friend class tree234<Key, Value>; 
    
   bstree<Key, Value>& tree;

   inorder_iterator& increment() // Go to next node.
   {
      if (stack.empty()) {

        current = nullptr;
        return *this;
      }

      current = stack.pop();
   
      if (pnode->right)  { // Next, go right, if we can, and.... 
   
         pnode = pnode->right;
   
         while (pnode)  { 

     	    stack.push(pnode);   //....push the right node onto the stack.
      	    pnode = pnode->left; // Then go as far left as we can, pushing all nodes onto stack.
         }
      }
   }

  public:

   using difference_type  = std::ptrdiff_t; 
   using value_type       = tree234<Key, Value>::value_type; 
   using reference        = value_type&; 
   using pointer          = value_type*;
       
   using iterator_category = std::forward_iterator_tag; 
				           
   inorder_iterator(bstree<Key, Value>& in) : tree{lhs},  {}
   {
      const pnode* = root.get();
   
      while (pnode) { // Go to min(root), pushing all nodes onto stack.
   
	 stack.push(pnode);
	 pnode = pnode->left;
      }
   }
   
   boolean has_next() 
   {
      return !stack.empty();
   }
   
   iterator& operator++() noexcept 
   {
      increment();
      return *this;
   } 
   
   iterator operator++(int) noexcept
   {
      iterator tmp(*this);
      increment();
      return tmp;
   } 
     
   reference operator*() const noexcept 
   { 
       return current->__vt.__get_value();
   } 
   
   pointer operator->() const noexcept
   { 
      return &(operator*()); 
   } 
};

#ifndef inorder_stack_iterator
#define inorder_stack_iterator

#include "bstree.h"
#include <iterator>

class inorder_stack_inorder_stack_iterator {

   using node_type = bstree<Key, Value>::node_type;

   std::stack<const node_type*> stack;

   const node_type *current;

   const bstree<Key, Value>& tree;

   inorder_stack_iterator& increment() noexcept // Go to next node.
   {
      if (stack.empty()) {

        current = nullptr; 
        return *this;
      }

      current = stack.top();

      stack.pop();
   
      if (pnode->right)  { // Next, go right, if we can, and.... 
   
         pnode = pnode->right.get();
   
         while (pnode != nullptr)  { 

     	    stack.push(pnode);         //....push the right node onto the stack.

      	    pnode = pnode->left.get(); // Then go as far left as we can, pushing all nodes onto stack.
         }
      }

      return *this;
   }
 
   bool has_next() const noexcept 
   {
      return !stack.empty();
   }

  public:

   using difference_type  = std::ptrdiff_t; 
   using value_type       = tree234<Key, Value>::value_type; 
   using reference        = value_type&; 
   using pointer          = value_type*;
       
   using iterator_category = std::forward_iterator_tag; 
				           
   using value_type = bstree<Key, Value>::value_type;

   inorder_stack_iterator(bstree<Key, Value>& in) : tree{lhs},  {}
   {
      const pnode* = root.get();
   
      while (pnode) { // Go to min(root), pushing all nodes onto stack.
   
	 stack.push(pnode);
	 pnode = pnode->left;
      }
   }

   iterator& operator++() noexcept 
   {
      increment();
      return *this;
   } 
   
   inorder_stack_iterator operator++(int) noexcept
   {
      inorder_stack_iterator tmp(*this);

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
};
#endif

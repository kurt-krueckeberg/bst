#ifndef bst_h_18932492374
#define bst_h_18932492374

#include <memory>
#include <utility>
#include <algorithm>
#include <stdlib.h>
#include "value-type.h"
#include <iostream>  
#include <exception>
#include <iterator>
/*
Sources:

https://www.geeksforgeeks.org/postorder-successor-node-binary-tree/         <-- post-order successor logic w/o stack

in file po.h
*/

class iterator_postorder {  // This not efficient to copy due to the stack container inside it.

   using node_type = bstree<Key, Value>::node_type;

   node_type *current;
   bool at_end = false;

   bstree<Key, Value>& tree;

   Node *increment() 
   {
       if (at_end || current == root.get()) return current;
        
       Node *__y = current;
   
       // If given node is the right child of its parent or parent's right is empty, then the 
       // parent is postorder successor. 
       auto parent = __y->parent; 
    
       if (!parent->right || __y == parent->right.get()) 
           __y = parent; 
       
       else {
    
          // In all other cases, find the left-most child in the right substree of parent. 
          auto pnode = parent->right.get(); 
       
          while (pnode->left) 
              pnode = pnode->left.get(); 

           __y = parent;
       }          

      if (__y == current) // TODO: Or __y == ptree->root.get()?
         at_end = true;

     return __y;
  }     
   
  public:

   using difference_type  = std::ptrdiff_t; 
   using value_type       = bstree<Key, Value>::value_type; 
   using reference        = value_type&; 
   using pointer          = value_type*;
       
   using iterator_category = std::forward_iterator_tag; 

   explicit iterator_postorder(bstree<Key, Value>& bstree) : tree{bstree}
   {
      if (root.get() 
      current = min(root.get());
      //...
   }
   
   iterator_postorder(const iterator_postorder& lhs) : current{lhs.current}, tree{lhs.tree}
   {
   }
   
   iterator_postorder(iterator_postorder&& lhs) : current{lhs.current}, tree{lhs.tree}
   {
       lhs.current = nullptr;
   }
   
   iterator_postorder& operator++() noexcept 
   {
      current = increment();
      return *this;
   } 
   
   iterator_postorder operator++(int) noexcept
   {
      iterator_postorder tmp(*this);

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
   
   struct sentinel {}; // Use for determining "at the end" in 'bool operator==(const iterator_postorder&) const' below

   bool operator==(const iterator_postorder::sentinel& sent) noexcept
   {
       return at_end; 
   }
   
   bool operator!=(const iterator_postorder::sentinel& lhs) noexcept
   {
     return !operator==(lhs);    
   }
};
#endif

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

class iterator_postorder {  // This not efficient to copy due to the stack container inside it.

   using node_type = bstree<Key, Value>::node_type;

   node_type *current;
   bool at_end = false;

   bstree<Key, Value>& tree;

   Node *increment() 
   {
     if (at_end) return curren
     Node *__y = current;
   
     if (__y->left) // Prefer left child
         __y = __y->left.get();
     else if (__y->right) // otherwise, the right 
         __y = __y->right.get();
   
     else  { // current is a leaf node
   
        // If leaf is a left child and it's parent has a right child, make it current
        if (current == current->parent->left.get() && current->parent->right) 
            
               __y = current->parent->right.get();
          
        else {// leaf is a right child (or a left child whose parent does not have a right child). Ascend the parent chain until we find a parent whose right child's key > current->key()
          
          for(auto parent = __y->parent; 1; parent = parent->parent) {
   
             // When parent's key is > current->key(), we are high enough in the parent chain to determine if the parent's right child's key > current->key().
             // If it is, this is the preorder successor for the leaf node current
             // Note: we combine all three tests--right child of parent exits, parent key is > current's, and parent's right child's key > current's--into one if-test. 
             if (parent->right && parent->key() > current->key() && parent->right->key() > current->key()) { 
                  __y = parent->right.get();
                  break; 
             } 
             if (parent == tree.root.get()) {
                 __y = current; // There is no pre-order successor because we ascended to the root, and the root's right child is < current->key()
                 break; 
             }
          } 
        } 
     } 
     if (__y == current)
         at_end = tru
     return __y;
  }     
   
  public:

   using difference_type  = std::ptrdiff_t; 
   using value_type       = bstree<Key, Value>::value_type; 
   using reference        = value_type&; 
   using pointer          = value_type*;
       
   using iterator_category = std::bidirectional_iterator_tag; 

   explicit iterator_postorder(bstree<Key, Value>& bstree) : tree{bstree}
   {
      current = bstree.root.get();
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

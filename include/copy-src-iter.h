#ifndef __copy_iterators_h_
#define __copy_iterators_h_

template<typename Key, typename Value>
class copy_source_iterator  {

  private:
                           
      bstree<Key, Value>&  dest_tree;

      copy_source_iterator& __src_iter;
  
      const Node *__src_node; 
      Node *__dest_node; 

      Node *dest_parent;

  void next()
  {
      if (__src_node->left) {
          __src_node = __src_node->left.get();
          dest_parent = ?? 
      } else if (__src_node->right) {       // otherwise, the right 
          __src_node = __src_node->right.get();
          dest_parent = ?? 

      } else if (__src_node->parent == nullptr) // root is leaf. There is no 'next'.
           return;
 
      else  { // __src_node is a leaf
 
         // If the leaf is a left child and it's parent has a right child, that right child is the pre-order next.
         if (__src_node == __src_node->parent->left.get() && __src_node->parent->right)  {
             
                __src_node = __src_node->parent->right.get();

                dest_parent = dest_node->parent;
              
         } else {// The leaf is a right child (or a left child whose parent does not have a right child).
                // So we must ascend the parent chain until we find a parent whose right child's key > __src_node->key()

           dest_parent = dest_node->parent; // dest_parent paralell's the role of parent below. dest_parent will be the
                                            // parent of the next node to be created when make_unique<Node> gets called again.

           for(auto parent = __src_node->parent; 1; parent = parent->parent) {
      
              // When parent's key is > prior->key(), we are high enough in the parent chain to determine if the
              // parent's right child's key > prior->key(). If it is, this is the preorder next for the leaf node prior. 
 
              // Note: we combine all three tests--right child of parent exits, parent key is > prior's,
              // and parent's right child's key > prior's--into one if-test. 
              if (parent->right && parent->key() > __src_node->key() && parent->right->key() > __src_node->key()) { 
 
                   __src_node = parent->right.get();
                   break; 
              } 
              
              if (parent == tree.root.get()) {
                  __src_node = tree.root.get(); // There is no pre-order next because we ascended to the root,
                  break;             // and the root's right child is < prior->key().
              }
              dest_parent = dest_parent->parent;   
           } 
         } 
      }
  }
   
  public:
     
  using iterator_category = std::forward_iterator_tag; 

  copy_source_iterator(bstree<Key, Value>& dest_tree) : __src{src_iter}, dest_tree{__src.dest_tree}, dest_parent{nullptr}
  {
     __src_node = this->root.get(); // Initial value is root
     __dest_node = dest_tree.root.get();
  }

  // Copy constructor.
  copy_source_iterator(const copy_source_iterator& __obj) : __src{__obj.__src}, dest_tree{__obj.dest_tree} 
  { 
  }

  copy_source_iterator& operator=(const Node * __src_node)
  {
     std::unique_ptr<Node> ptr = std::make_unique<Node>(__src_node->__vt); // copy source node
 
    if (!__src_node->parent) // If _y is the root of the source tree, we know ptr set to nullptr above by ctor.
       
        dest_tree.root = std::move(ptr); 
 
    else if (__src.dest_parent->key() > ptr->key()) // dest_node is left child  
           
        __src.dest_parent->connectLeft(ptr); 
           
    else // new node is a right child
           
        __src.dest_parent->connectRight(ptr); 
 
    __src.dest_parent->key() 

    return *this;
  }

  copy_source_iterator&
  operator*()
  {
  }

  copy_source_iterator&
  operator++()
  {
  }

  copy_source_iterator&
  operator++(int)
  {
  }
};

#endif

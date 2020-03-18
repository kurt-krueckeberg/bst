#ifndef __copy_iterators_h_
#define __copy_iterators_h_

/*
bstree methods to create iterators for copying trees:

pair<copy_dest_iterator, copy_srouce_iterator> make_copy_iterators_begin(bstree<Key, Value>& dest_tree); 

copy_dest_iterator make_dest_iterator_end(bstree<Key, Value>& dest_tree); 

But then destination iterator is created from the source iterator:

  copy_source_iterator copy_source_iterator(const copy_dest_iterator& dest_iter); 

So make_copy_iterators_begin() will invoke the copy_source_iterator constructor.

New code to replace copy_tree() with:

      auto&[src_iter, dest_iter] = make_dest_iterator_begin(bstree<Key, Value>& dest_tree); 
      auto src_iter_end = make_dest_iterator_end(bstree<Key, Value>& dest_tree); 
    
      copy_source_iterator copy_source_iterator(const copy_dest_iterator& dest_iter); 
         
      copy(src_iter, src_iter_end, dest_iter);
      OR
      move(src_iter, src_iter_end, dest_iter);
*/

template<typename Key, typename Value>
class copy_source_iterator  {

  private:
      bstree<Key, Value>&  src_tree;
      bstree<Key, Value>&  dest_tree;
      const Node *__y; 

  void successor()
  {
   if (!dest_tree.root) 
       return;

   Node *__y = tree.root.get(); // Move to ctor
 
   Node *dest_parent = nullptr; // The parent of the node we copy. Used to call connectLeft/connectRight 
                                // to attach it to the new tree.
   Node *dest_node = nullptr;   // Raw pointer to 
   
   do {   
       
        std::unique_ptr<Node> dest_ptr = std::make_unique<Node>(__y->__vt);
        
        dest_node = dest_ptr.get(); //TODO: This is dest_parent also in the else-if and else
 
        if (!__y->parent) {// Since __y was the root, we set parent of dest_node to nullptr.
           
            src_tree.root = std::move(dest_ptr);
            dest_parent = src_tree.root.get();
 
        }  else if (dest_parent->key() > dest_ptr->key()) { // dest_node is left child  
               
            dest_parent->connectLeft(dest_ptr); 
            dest_parent = dest_parent->left.get();
               
        } else {    // new node is a right child
               
            dest_parent->connectRight(dest_ptr); 
            dest_parent = dest_parent->right.get();
        }
        
        if (__y->left)          // We traversal left first
            __y = __y->left.get();
        else if (__y->right)       // otherwise, the right 
            __y = __y->right.get();
        else if (__y->parent == nullptr) {} // root is leaf. Do nothing. Loop will terminate 
        else  { // __y is a leaf
 
           // If the leaf is a left child and it's parent has a right child, that right child is the pre-order successor.
           if (__y == __y->parent->left.get() && __y->parent->right)  {
               
                  __y = __y->parent->right.get();

                  dest_parent = dest_node->parent;
                
           } else {// The leaf is a right child (or a left child whose parent does not have a right child).
                  // So we must ascend the parent chain until we find a parent whose right child's key > __y->key()

             dest_parent = dest_node->parent; // dest_parent paralell's the role of parent below. dest_parent will be the
                                              // parent of the next node to be created when make_unique<Node> gets called again.

             for(auto parent = __y->parent; 1; parent = parent->parent) {
        
                // When parent's key is > prior->key(), we are high enough in the parent chain to determine if the
                // parent's right child's key > prior->key(). If it is, this is the preorder successor for the leaf node prior. 
 
                // Note: we combine all three tests--right child of parent exits, parent key is > prior's,
                // and parent's right child's key > prior's--into one if-test. 
                if (parent->right && parent->key() > __y->key() && parent->right->key() > __y->key()) { 
 
                     __y = parent->right.get();
                     break; 
                } 
                
                if (parent == tree.root.get()) {
                    __y = tree.root.get(); // There is no pre-order successor because we ascended to the root,
                    break;             // and the root's right child is < prior->key().
                }
                dest_parent = dest_parent->parent;   
             } 
           } 
        }
    } while(__y != tree.root.get()); 
  }
   
  public:
     
  using iterator_category = std::forward_iterator_tag; 

  copy_source_iterator(const bstree<Key, Value>& src_tree, bstree<Key, Value>& dest_tree) : __src{src_iter}, dest_tree{__src.dest_tree}
  {
     __y = tree.root.get(); // Move to ctor
  }

  // Copy constructor.
  copy_source_iterator(const copy_source_iterator& __obj) : __src{__obj.__src}, dest_tree{__obj.dest_tree} 
  { 
  }

  copy_source_iterator& operator=(const Node * __y)
  {
     std::unique_ptr<Node> ptr = std::make_unique<Node>(__y->__vt); // copy source node
 
    if (!__y->parent) // If _y is the root of the source tree, we know ptr set to nullptr above by ctor.
       
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

template<typename Key, typename Value>
class copy_dest_iterator output_iterator_tag {
public:

    private:
      bstree<Key, Value>&  dest_tree;
      copy_source_iterator<Key, Value>& __src; // Can we eliminate this?
      
    public:
     
  using iterator_category = std::output_iterator_tag; 

  copy_dest_iterator(copy_src_iterator& src_iter) : __src{src_iter}, dest_tree{__src.dest_tree}
  {
  }

  // Copy constructor.
  copy_dest_iterator(const copy_dest_iterator& __obj) : __src{__obj.__src}, dest_tree{__obj.dest_tree} 
  { 
  }

  copy_dest_iterator& operator=(const Node * __y)
  {
     std::unique_ptr<Node> ptr = std::make_unique<Node>(__y->__vt); // copy source node
 
    if (!__y->parent) // If _y is the root of the source tree, we know ptr set to nullptr above by ctor.
       
        dest_tree.root = std::move(ptr); 
 
    else if (__src.dest_parent->key() > ptr->key()) // dest_node is left child  
           
        __src.dest_parent->connectLeft(ptr); 
           
    else // new node is a right child
           
        __src.dest_parent->connectRight(ptr); 
 
    __src.dest_parent->key() 

    return *this;
  }

  copy_dest_iterator&
  operator*()
  { return *this; }

  copy_dest_iterator&
  operator++()
  { return *this; }

  copy_dest_iterator&
  operator++(int)
  { return *this; }
};
#endif

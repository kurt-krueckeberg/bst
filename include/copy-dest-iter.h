#ifndef __copy_dest_iterators_h_
#define __copy_dest_iterators_h_

/*
1. option 1: one iterator that is both source annd destination with an assignment operator that copies a source node and connects it to the destination tree. <--
2. option 2:

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
class copy_dest_iterator output_iterator_tag {
public:

    private:
      bstree<Key, Value>&  dest_tree;

      //--copy_source_iterator<Key, Value>& __src_it; // Can we eliminate this?

      Node *&dest_parent;
      
    public:
     
  using iterator_category = std::output_iterator_tag; 

  copy_dest_iterator(copy_src_iterator& src_iter) : /*__src_it{src_iter,*/ dest_tree{__src_it.dest_tree}, dest_parent{___src_it.dest_parent}
  {
  }

  // Copy constructor.
  copy_dest_iterator(const copy_dest_iterator& __obj) : /* __src_it{__obj.__src_it}, */ dest_tree{__obj.dest_tree}, dest_parent{___src_it.dest_parent}
  { 
  }

  copy_dest_iterator& operator=(const Node* __y)
  {
    std::unique_ptr<Node> ptr = std::make_unique<Node>(__y->__vt);
    
    if (!__y->parent) // If __y was the root, move ptr into root (parent was auto. set to nullptr)
       
        dest_tree.root = std::move(ptr);

    else if (dest_parent->key() > ptr->key()) { // dest_node is left child  
           
        dest_parent->connectLeft(ptr); 
           
    else // new node is a right child
           
        dest_parent->connectRight(ptr); 

    return *this;
  }

  // Other methods are no ops.
  copy_dest_iterator& operator*() { return *this; }

  copy_dest_iterator& operator++() { return *this; }

  copy_dest_iterator& operator++(int) { return *this; }
};
#endif

#ifndef __copy_iterators_h_
#define __copy_iterators_h_

/*
bstree methods to create iterators for copying trees:

  copy_dest_iterator make_dest_iterator_begin(bstree<Key, Value>& dest_tree); 
  copy_dest_iterator make_dest_iterator_end(bstree<Key, Value>& dest_tree); 

Then destination iterator is created from the source iterator:

  copy_source_iterator copy_source_iterator(const copy_dest_iterator& dest_iter); 

New code to replace copy_tree() with:

      copy_dest_iterator make_dest_iterator_begin(bstree<Key, Value>& dest_tree); 
      copy_dest_iterator make_dest_iterator_end(bstree<Key, Value>& dest_tree); 
    
      copy_source_iterator copy_source_iterator(const copy_dest_iterator& dest_iter); 
         
      copy(siter, siter_end, diter);
      OR
      move(siter, siter_end, diter);
*/

template<typename Key, typename Value>
class copy_source_iterator output_iterator_tag {

  private:
      bstree<Key, Value>&  src_tree;
      bstree<Key, Value>&  dest_tree;
      
  public:
     
  using iterator_category = std::forward_iterator_tag; 

  copy_source_iterator(const bstree<Key, Value>& src_tree, bstree<Key, Value>& dest_tree) : __src{src_iter}, dest_tree{__src.dest_tree}
  {
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

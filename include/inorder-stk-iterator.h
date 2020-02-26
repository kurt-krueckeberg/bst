/*
The recursive in-order algorithm

.. code-block:: cpp

     void in_order(std::unique_ptr<Node>& current) const noexcept
     {
         if (!current) return;
   
         in_order(current->left);
   
         f(current->__get_value());
   
         in_order(current->right);
     }

repeatedly invokes itself passing current's left child until it is nullptr when it returns. It then visits the parent. Next it repeats this process (of recursing down the left child) with the the right child.

This can be converted to an iterative algorithm using a stack
void inOrder(struct Node *root) 
{ 
    stack<Node *> s; 
    Node *curr = root; 
  
    while (curr != NULL || s.empty() == false) 
    { 
        // Reach the left most Node of the 
        // curr Node
        while (curr !=  NULL) 
        { 
            // Place pointer to a tree node on 
            // the stack before traversing 
            // the node's left subtree 
            s.push(curr); 
            curr = curr->left; 
        } 
  
        // Current must be NULL at this point 
        curr = s.top(); 
        s.pop(); 
  
        cout << curr->data << " "; 
  
        // We have visited the node and its 
        // left subtree.  Now, it's right 
        // subtree's turn 
        curr = curr->right; 
  
    }
} 

*/
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

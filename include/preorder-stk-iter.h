/* 
  Stack-based iterative pre-order iterator class.

  Converted Java code from: 
  https://www.cs.cmu.edu/~adamchik/15-121/lectures/Trees/code/BST.java

  Also a complete discussion of how to implement at stack-based iterator is at:
  http://courses.cs.vt.edu/~cs3114/Fall17/barnette/notes/Tree-Iterators.pdf

 */
class preorder_iterator {

   std::stack<node_type&> stack;
   bstree<Key, Value>& tree;

   node_type& current;

 public:
   // traits for forward iterator
   using difference_type  = std::ptrdiff_t; 
   using value_type       = tree234<Key, Value>::value_type; 
   using reference        = value_type&; 
   using pointer          = value_type*;
   
   using iterator_category = std::forward_iterator_tag; 
	
   preorder_iterator(const bstree<Key, Value>& lhs) : tree{lhs}
   {
      if (tree.root)
          stack.push(root);
   }

    boolean hasNext()
   {
      return !stk.isEmpty();
   }

   iterator& increment() noexcept 
   {
      node_type& current = stack.top();
      
      if (current.left)    
      
         stack.push(current.left);
      
      else {
      
         node_type& tmp = stack.pop();
      
         while (!tmp.right) {
      
            if (stack.empty()) 
                 return current.data;
      
            tmp = stack.pop();
         }
      
         stack.push(tmp.right);
      }
      
      return .data;
   }
   

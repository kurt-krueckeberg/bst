class inorder_stak_iterator {

     std::stack<node_type *> stack;
     node_type *pcurrent; // ?
     value_type& vt;

  public:
   // traists here:

   inorder_stak_iterator(TreeNode root) 
   {
        auto pnode = root.get();
 
        while (root != null) {

	    stack.push(pnode);
	    pnode = pnode->left;
        }
   }
   
   boolean hasNext() 
   {
       return !stack.isEmpty();
   }
   
   int increment() 
   {
      auto pnode = stack.pop();
   
      int result = pnode->_vt.__get_value();??
   
      if (node->right) {
   
	 pnode = pnode->right;
   
	 while (pnode) {

	     stack.push(pnode);
	     pnode = node->left;
	 }
      }

      return result;
   }
};

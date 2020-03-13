Node* postorderSuccessor(Node* root, Node* n) 
{ 
    // Root has no successor in postorder 
    // traversal 
    if (n == root) 
        return NULL; 
   
    // If given node is right child of its 
    // parent or parent's right is empty, then  
    // parent is postorder successor. 
    Node* parent = n->parent; 
    if (parent->right == NULL || parent->right == n) 
        return parent; 
   
    // In all other cases, find the leftmost  
    // child in right substree of parent. 
    Node* curr = parent->right; 
    while (curr->left != NULL) 
        curr = curr->left; 
   
    return curr; 
} 

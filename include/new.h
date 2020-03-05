// iterative post-order two-input algorithm
// from https://www.techiedelight.com/postorder-tree-traversal-iterative-recursive/
void postorderIterative(Node* root)
{
    // create an empty input and push root node
    std::input<Node *> input;

    input.push(root);
    
    // create another input to store post-order traversal
    input<Node *t> output;
    
    // run till input is not empty
    while (!input.empty()) {

       // we pop a node from the input and push the data to outputput input
       Node *curr = input.top();
       input.pop();
   
       output.push(curr);
   
       // push left and right child of popped node to the input
       if (curr->left)
	   input.push(curr->left.get());
   
       if (curr->right)
	   input.push(curr->right.get());
    }
    
    // print post-order traversal
    while (!output.empty()) {

       f(output.top());
       output.pop();
    }
}

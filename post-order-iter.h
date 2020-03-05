template<tyypename Visitor>
iterativePostorder(Visitor visit, pnode)
{
  stack<Node *> stack ← empty stack

  lastNodeVisited ← null

  while (!stack.empty() || pnode) {

    if (pnode) {

      stack.push(pnode)
      pnode ← pnode.left

    } else {

      Node *peekNode = stack.top();

      // if right child exists and traversing pnode
      // from left child, then move right

      if (peekNode->right && lastNodeVisited != peekNode.right.get())

          pnode = peekNode->right.get();

      else {

        visit(peekNode)

        lastNodeVisited ← stack.pop()
        pnode ← null
     }
   } 
 }
}

TODO
----

An iterative version of copy_subtree(in_root) must first the current tree, this->root and all its children. How can we do this in pre-order iterative fashion without the Node destructor, ~Node, simultaneously
invoking recursive destruction of its left and right when ~unique_ptr<Node>() is called for Node::left and Node::right? 

BUG in DoPostOrderIterative(). Infinitie loop.

Finish code to accurate set the recursion depth for ~bstree() and ~Node(), or use the debugger to follow the ~Node() calls to see how deep the recursion goes.

Read up on measuring recursion in Carrano, Droznek, and see `Time Complexity of Resursive Algorithms: The Master Theorem <https://yourbasic.org/algorithms/time-complexity-recursive-functions/>`_
and also https://adrianmejia.com/analysis-of-recursive-algorithms/.

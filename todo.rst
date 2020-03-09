TODO
====

Sources to Read
---------------

* C code that implements both recursive and iterative versions of traversal algorithms: https://code.google.com/archive/p/treetraversal/downloads
* `Tree traversal without recursion: the tree as a state machine <https://www.perlmonks.org/?node_id=600456>`_ explains and shows how iterative tree traversal can be done withan explicit stack. 
* Theory:  `Time Complexity of Resursive Algorithms: The Master Theorem <https://yourbasic.org/algorithms/time-complexity-recursive-functions/>`_
  and also https://adrianmejia.com/analysis-of-recursive-algorithms/.

Specific todoes
---------------

1. `Tree traversal without recursion: the tree as a state machine <https://www.perlmonks.org/?node_id=600456>`_ explains and shows how iterative tree traversal can be done withan explicit stack. It shows  
   a pre-order version that uses the parent pointer and doesn't require a stack. Mentions this and implement it. 
2. Do likewise with pre- and post-order iterative traversals.

Using 'if consexpr' to implement one version: 

   if constexpr (not_exists(Functor::parm_type)) {
   else if constexpr (is_node_type(Functor::parm_type)) {

   } else {
        std::throw
   } 



Finish code to accurate set the recursion depth for ~bstree() and ~Node(), or use the debugger to follow the ~Node() calls to see how deep the recursion goes.

Read up on measuring recursion in Carrano, Droznek, and see `Time Complexity of Resursive Algorithms: The Master Theorem <https://yourbasic.org/algorithms/time-complexity-recursive-functions/>`_
and also https://adrianmejia.com/analysis-of-recursive-algorithms/.

TODO
====

* iterator_inorder needs a default copy ctor and an assigment operator, which menas the 'tree' cannot be a reference. It must be a raw pointer.
* How does iterator_inorder handle the case when operator--() is called and we are already at the the first element?
* See TODO comments in code. They may be relevant to fixing the bug.
* Test both iterator_inorder and iterator_preorder. Test both with 'big' trees.
* Note: The iterator_inorder code does not support reverse iterator. Remove rbegin() and rend(). The extra complexity of supporting it is not worth the extra time.
* The postorder destruction call by ~bstree() and bstree(conost bstree&) need to operator on 'Node *' and not 'bstree:value_type'
* The preorder call in 'bstree(const bstree&)' needs to operator agains 'Node *' and not 'bstree:value_type'

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

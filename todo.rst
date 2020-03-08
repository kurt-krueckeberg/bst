TODO
----

auto functor = [](auto& x) {
  using parm_type = decltype(x);
};

class Functor {
  public:

   void operator(bstree<Key, Value>::node_type


 if constexpr (decltype(param_type)

TODO: Add const and non-const versions of the traveral methods. Add privaate non-const version that visit a Node& and not bstree::value_type.
Use this more thorougly discussed and commented version of postOrderIterative:

  `post-order Iterative algorithm <https://www.java67.com/2017/05/binary-tree-post-order-traversal-in-java-without-recursion.html>`_.

Finish code to accurate set the recursion depth for ~bstree() and ~Node(), or use the debugger to follow the ~Node() calls to see how deep the recursion goes.

Read up on measuring recursion in Carrano, Droznek, and see `Time Complexity of Resursive Algorithms: The Master Theorem <https://yourbasic.org/algorithms/time-complexity-recursive-functions/>`_
and also https://adrianmejia.com/analysis-of-recursive-algorithms/.

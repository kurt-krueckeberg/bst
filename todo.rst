TODO
====

1. See line 1714. dest_node and dest_parent are the saem value. Confirm this. And if so, then dest_node is not needed, and the call to preorder_copy_next() can be changed: the dest_node parameter does not need to be passed; 
   however, thhe first line or preoder_copy_next() should copy dest_parent as dest_node.

   Once the above is done, remove the  assert() at around line 1716.

2. Test preorder_copy() and preorder_copy_next().

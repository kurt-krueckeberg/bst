TODO
====

Bug
~~~

cout << tree_in;

auto tree_out = copy_tree(tree_in);

cout << tree_out;             // <---- BUG: Output differs from tree_in!! 

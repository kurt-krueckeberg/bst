TODO 
====

The new code is in bst-key-value.h, remove() is not yet implemented. The algorithms all follow the book Introduction to Algoritms by Cormen, et. al. -- and the helper transplant() method -- that are givin starting on page 295 of Introduction to Algorithms.

Other General Comments
----------------------

1. First, prove that using shared_ptr causes state/nodes to be shared, so we can't have any methods that return Value& or pair<const Key, Value>&, and we can't do things like 'map[some_key]++, say. Prove that this is true for
2. Trace the Node copy ctor and assignemnt ctors to confirm them operation.
3. Study std::map then add the other insert methods, after I understand them.

.. code-block:: cpp

    #include <iostream>
    #include <map>
    #include <string>
     
    int main()
    {
        std::map<std::string, std::string> myMap;
        myMap.insert_or_assign("a", "apple"     );
        myMap.insert_or_assign("b", "bannana"   );
        myMap.insert_or_assign("c", "cherry"    );
        myMap.insert_or_assign("c", "clementine"); // <--- Overwrite value at key "c", changing tree.
     
        for (const auto &pair : myMap) {
            std::cout << pair.first << " : " << pair.second << '\n';
        }
    }
    
See also the std::map methods ``T& operator[]( const Key& key );`` and ``T& operator[]( Key&& key );`` that also return reference to the value, ``T&``, at ``key`` from
https://en.cppreference.com/w/cpp/container/map/operator_at

.. code-block:: cpp

    #include <iostream>
    #include <string>
    #include <vector>
    #include <map>
     
    int main()
    {
        std::map<char, int> letter_counts {{'a', 27}, {'b', 3}, {'c', 1}};
     
        std::cout << "initially:\n";
        for (const auto &pair : letter_counts) {
            std::cout << pair.first << ": " << pair.second << '\n';
        }
     
        letter_counts['b'] = 42;  // update an existing value
        letter_counts['x'] = 9;  // insert a new value
     
        std::cout << "after modifications:\n";
        for (const auto &pair : letter_counts) {
            std::cout << pair.first << ": " << pair.second << '\n';
        }
     
        // count the number of occurrences of each word
        // (the first call to operator[] initialized the counter with zero)
        std::map<std::string, size_t>  word_map;
        for (const auto &w : { "this", "sentence", "is", "not", "a", "sentence",
                               "this", "sentence", "is", "a", "hoax"}) {
            ++word_map[w];
        }
     
        for (const auto &pair : word_map) {
            std::cout << pair.second << " occurrences of word '" << pair.first << "'\n";
        }
    }
		    
shared-bst.h and unique-bst.h use shared_ptr and unique_ptr, respectively.

TODO
----

1. Prove that using shared_ptr causes state/nodes to be shared, so we can't have any methods that return Value& or pair<const Key, Value>&. See code above.,
2. Use __value_type from value-type.h in bst.h, and test it, check the logic.
3. Add the std::map methods that are missing like emplace() and insert_or_assign().
4. Migrate the __value_type() solution to the tree23 code and tree23.
5. Write up in ~/c/tree23 and ~/c/tree234 why unique_ptr is preferred over shared_ptr and comment on the implementation, say, of Node() class and __value_type's use. 

TODO: Use the include/value-type.h code extracted from /usr/lib/llvm-9/include/c++/v1/map. 
or using this:

.. code-block:: cpp

    // types:
    typedef _Key                                     key_type;
    typedef _Tp                                      mapped_type;
    typedef pair<const key_type, mapped_type>        value_type;
    typedef value_type&                              reference;
    typedef const value_type&                        const_reference;


shared_ptr use and issues
-------------------------

Using shared_ptr<Node> improves efficiency, but causes problems (see below). For example, when we copy a tree, we only have to copy a shared_ptr, but there is a drawback. For the details see:

* `C++11 Smart Pointer Tutorial Series: Part 1 of **shared_ptr Tutorial and Examples** <https://thispointer.com/learning-shared_ptr-part-1-usage-details/>`_
* `C++11 Smart Pointer – Part 5: shared_ptr, Binary trees and the problem of Cyclic References <https://thispointer.com/shared_ptr-binary-trees-and-the-problem-of-cyclic-references/>`_
* `Implementation of Binary Search Trees ViaSmart Pointer <https://thispointer.com/shared_ptr-binary-trees-and-the-problem-of-cyclic-references/>`_
* `Exploring std::shared_ptr <https://shaharmike.com/cpp/shared-ptr/>`_

Questions about the first article:

* What is the fundamental issue with the 'parent' pointer?

* Can the issue be solved by simply using a raw pointer instead of a using weak_ptr, or is weark_ptr necessary to ensure memory is properly managed in trees? 

.. todo:: Once I have the answer, add the reasoning to the 'binary search tree' text. Same with 2-3 trees and 2-3-4 trees. And ultimately red-black trees

* Why does weak_ptr exist, what does doe it do exactly, and when must it be used?

* What are the issues with shared_ptr in terms of its effect on not managing memory properly?
  
A: Currently copy_tree creates new nodes on the heap offering no advantage over unique_ptr. There is a design problem with using shared_ptr, too. If we share nodes between trees, an update to a node's Values (which
can be returned by reference using 'map[some_key]') will be reflected in any other tree created as a copy. 

.. todo:: Test the above and verify it, then implement or revert to a unique_ptr version.

Questions about BST implented using shared_ptr on StackExchange
---------------------------------------------------------------

* `Using smart pointers for C++ binary search tree <https://stackoverflow.com/questions/45651401/using-smart-pointers-for-c-binary-search-tree>`_.
* `Binary Search Tree Data Structure Implementation in C++11 using Smart Pointers <https://codereview.stackexchange.com/questions/191211/binary-search-tree-data-structure-implementation-in-c11-using-smart-pointers>`_
* `how to implement BinarySearchTree on shared_ptr in c++? <https://stackoverflow.com/questions/20887489/how-to-implement-binarysearchtree-on-shared-ptr-in-c>`_
* `Binary Search Tree implementation using smart pointers <https://codereview.stackexchange.com/questions/212692/binary-search-tree-implementation-using-smart-pointers/212696>`_

Resources for Understanding in-place construction using C++17
-------------------------------------------------------------

*  Explanation of map::emplace(): https://en.cppreference.com/w/cpp/container/map/emplace 
*  **STL Cookbook Item 56** explains try_emplace()
* `Improved insertion interface for unique-key maps <https://isocpp.org/files/papers/n4279.html>`_
* `Perfect Forwarding of Parameter Groups in C++11 <http://cpptruths.blogspot.com/2012/06/perfect-forwarding-of-parameter-groups.html>`_.

#ifndef _test_h_
#define _test_h_

#include <initializer_list>

void test(std::initializer_list<int>& test_lst);

template<typename Key, typename Value> class bstree;

template<typename Key, typename Value> void test_preorder(const bstree<Key, Value>& tree);

template<typename Key, typename Value> void test_reverse_iterators(bstree<Key, Value>& tree);

template<typename Key, typename Value> void test_tree_assignment(bstree<Key, Value>& tree);

#endif

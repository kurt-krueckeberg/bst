#include <cstdlib>
#include <utility>
#include <iostream>
#include <iomanip>
#include <initializer_list>
#include "bstree.h"
#include "test.h"

using namespace std;

void test(std::initializer_list<int>& test_lst)
{
  bstree<int, int> test_tree;
  
  for (const auto& i : test_lst) 
      test_tree.insert(i, i);

  test_tree_assignment(test_tree);

  return;

  auto trace_printer = [](const auto& pr) {

      const auto&[key, value] = pr;

      cout << "  " << key << "  ";
  };

  auto key_printer = [](const auto& pr) {
      const auto&[key, value] = pr;
      
      cout << setw(3) << key << '\n';
  };
/*

  cout << "test_tree.printlevelOrder(key_printer) = " << flush;

  test_tree.printlevelOrder(key_printer); 

  cout << "test_tree.inOrderTraverse(key_printer) = ";

  test_tree.inOrderTraverse(key_printer);
  
  cout << "for (auto& [key, value] : test_tree) cout << key " << endl; 

  test_iterators(test_tree); // Subsume code below
  
  auto iter_end = test_tree.end();
   
  auto iter = test_tree.begin();
  
  for (; iter != iter_end; ++iter) {

    auto& [key, value] = *iter;
    cout << key << ", \n" << flush;
  }
  cout << '\n' << flush;
  
  decltype(iter) iter2;

  iter2 = iter;
  
  iter = test_tree.begin();
 */ 
 
  test_preorder(test_tree);
 /* 
  cout << "\ntest_tree.postOrderIterative(key_printer) = ";
  
  test_tree.postOrderIterative(key_printer);
  
  cout << '\n' << flush;

*/
  /*
  test_reverse_iterators(test_tree);
 */
  cout << "Test bstree copy ctor\n";
  
  test_tree_assignment(test_tree);
}

template<typename Key, typename Value>
void test_preorder(const bstree<Key, Value>& tree)
{
  auto key_printer = [](const auto& pr) {
      const auto&[key, value] = pr;
      
      cout << setw(3) << key << '\n';
  };
  
  cout << "\ntree.preOrderTraverse(key_printer) = ";

  tree.preOrderTraverse(key_printer);

  cout << "\ntree.preOrderStackIterative(key_printer) = ";
  
  tree.preOrderStackIterative(key_printer);
  
  cout << '\n';
  
  cout << "\ntree.preOrderIterative(key_printer) = ";
  
  tree.preOrderIterative(key_printer);
  
  cout << '\n';
}

template<typename Key, typename Value>
void test_reverse_iterators(bstree<Key, Value>& tree)
{
 cout << "testing bstree reverse iteration." << endl;
  
  auto riter = tree.rbegin();
  auto riter_end = tree.rend();
  
  for (; riter != riter_end; ++riter) { // BUG: In decrement()

    auto& [key, value] = *riter;

    cout << key << ", \n" << flush;
  }
}

template<typename Key, typename Value>
void test_tree_assignment(bstree<Key, Value>& tree)
{
  decltype(tree) tree2 = tree;

  auto key_printer = [](const auto& pr) {
      const auto&[key, value] = pr;
      
      cout << setw(3) << key << '\n';
  };
   
  cout << "Printing input tree.inOrderIterative(key_printer) = \n";

  tree.inOrderIterative(key_printer);
 
  cout << "Printing tree copy, tree2. tree2.inOrderIterative(key_printer) = \n";

  tree2.inOrderIterative(key_printer);

  cout << "Print input tree using iterator_inorder =\n";
  for (const auto& pr : tree) {
  
        auto& [key, value] = pr;
        cout << key << ", " << endl;
  }

  cout << '\n';

  cout << "Print ouput tree using iterator_inorder =\n";
  for (const auto& pr :  tree2) {
        auto& [key, value] = pr;
        cout << key << ", " << endl;
  }
   
  cout << "Printing input tree.preOrderIterative(key_printer) = \n";

  tree.preOrderIterative(key_printer);
 
  cout << "Printing tree copy, tree2. tree2.preOrderIterative(key_printer) = \n";

  tree2.preOrderIterative(key_printer);
}

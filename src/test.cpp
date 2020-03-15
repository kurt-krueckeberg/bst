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

  auto trace_printer = [](const auto& pr) {

      const auto&[key, value] = pr;

      cout << "  " << key << "  ";
  };

  auto key_printer = [](const auto& pr) {
      const auto&[key, value] = pr;
      
      cout << setw(3) << key << '\n';
  };

  cout << "test_tree.printlevelOrder(key_printer) = " << flush;

  test_tree.printlevelOrder(key_printer); 

  cout << "test_tree.inOrderTraverse(key_printer) = ";

  test_tree.inOrderTraverse(key_printer);
  
  cout << "for (auto& [key, value] : test_tree) cout << key " << endl; 
  
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
  
  auto& [key, value] = *iter;
  
  cout << key << endl;
  
  --iter;        
  
  cout << key << endl;

  cout << "testing bstree reverse iteration." << endl;
  
  // Check
  auto riter = test_tree.rbegin();
  auto riter_end = test_tree.rend();
  
  for (; riter != riter_end; ++riter) { // BUG: In decrement()

    auto& [key, value] = *riter;

    cout << key << ", \n" << flush;
  }
  
  cout << "\ntest_tree.preOrderTraverse(key_printer) = ";

  test_tree.preOrderTraverse(key_printer);

  cout << "\ntest_tree.preOrderStackIterative(key_printer) = ";
  
  test_tree.preOrderStackIterative(key_printer);
  
  cout << '\n';
  
  cout << "\ntest_tree.preOrderIterative(key_printer) = ";
  
  test_tree.preOrderIterative(key_printer);
  
  cout << '\n';
  cout << "\ntest_tree.postOrderIterative(key_printer) = ";
  
  test_tree.postOrderIterative(key_printer);
  
  cout << '\n' << flush;
  
  cout << '\n';
  cout << "\ntest_tree testing iterator_preorder  = ";
 
  auto preiter = test_tree.begin_pre();
  auto preend = test_tree.end_pre();
  
  for (;preiter != preend; ++preiter) {
      const auto&[key, value] = *preiter;  
      
      cout << key << ",\n" << flush; 
  
  }
 
}

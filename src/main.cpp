#include <cstdlib>
#include <utility>
#include <iostream>
#include <iomanip>
#include <initializer_list>
#include "test.h"
#include "bstree.h"

using namespace std;


int main(int argc, char** argv) 
{
  bstree<int, int> test_tree;

  std::initializer_list<int> test_lst = {7, 1, 30, 0, 3, 8, 50, 20, 9, 2, 5, 4, 6, -10, -5, 40, 60, 55, 65, 54, -20};

  for (const auto& i : test_lst) 
      test_tree.insert(i, i);

  auto trace_printer = [](const auto& pr) {
      const auto&[key, value] = pr;
      //cout << "\tPopped and visited: " << key;
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

  cout << "\ntest_tree.preOrderIterative(key_printer) = ";
  
  auto null_functor = [](const auto& pr) {
      //const auto&[key, value] = pr;
      
      //cout << setw(3) << key << '\n';
      return;
  };

  test_tree.preOrderIterative(null_functor);
  cout << '\n';
  cout << "\ntest_tree testing iterator_preorder  = ";
 
  auto preiter = test_tree.begin_pre();
  auto preend = test_tree.end_pre();
  
  for (;preiter != preend; ++preiter) {
      const auto&[key, value] = *preiter;  
      
      cout << key << ",\n" << flush; 
  
  }
  
  return 0;
 
  cout << '\n';
  cout << "\ntest_tree.postOrderIterative(key_printer) = ";
  
  test_tree.postOrderIterative(key_printer);
  
  cout << '\n' << flush;
  
  std::initializer_list<int> lst = {100, 50, 200, 20, 70, 150, 250, -10, 40, 60, 90, 125, 175, 225, 275, -40, 10, 30, 45, 55, 65, 80, 95, 110, 130, 165, 190, 220, 230, 260, 290,\
    -70, -30, -5, 15, 25, 35, 42, 47, 52, 57, 62, 67, 92, 97, 105, 115, 127, 135, 160, 170, 180, 195, 210, 222, 227, 235, 260, 280 };

  bstree<int, int> t2;

  for (auto& x : lst) 
      t2.insert(x, x);
    
  t2 = test_tree;
  
  cout << "\ntest_tree.inOrderIterative(key_printer) = ";

  test_tree.inOrderIterative(key_printer);

  cout << flush << "\ntest_tree.preOrderIterative(key_printer) = " ;
  
  test_tree.preOrderIterative(key_printer);
  
  cout << '\n' << flush;

  bstree<int, int> bal_tree;

  for (const auto& i : lst) 
      bal_tree.insert(i, i);
  
  cout << "bal_tree.inOrderIterative(key_printer) = \n";
  
  bal_tree.inOrderIterative(key_printer);
  cout << "\nUsing range loop\n";
  
  for (const auto& vt : bal_tree) {
        const auto&[key, value] = vt;
        cout << key << ", " << flush;  
  } 

//  bal_tree.printlevelOrder(cout, key_printer); // BUG <-- Why pass cout, when key_printer never uses this parameter! Providing only levelOrderTraverse() is sufficient.
    bal_tree.levelOrderTraverse(key_printer); // BUG <-- Why pass cout, when key_printer never uses this parameter! Providing only levelOrderTraverse() is sufficient.
  
  cout << "bal_tree::preorder_stack_iterator loop: for(iter =..; iter != iter_end; ++iter) = \n";
  
  bal_tree.inOrderIterative(key_printer);

  cout << "\n--------------\nPrinting tree_copy, a copy of the above bal_tree.\n"; 

  int hidden = 0;

  bstree<int, int> tree_copy = bal_tree;

  tree_copy.levelOrderTraverse(key_printer);
  
  cout << "floor(37) = " << bal_tree.floor(37) << '\n';

  cout << "ceiling(37) = " << bal_tree.ceiling(37) << '\n';
 
  cout << "floor(41) = " << bal_tree.floor(41) << '\n';

  cout << "ceilling(41) = " << bal_tree.ceiling(41) << '\n';

  bal_tree.levelOrderTraverse(key_printer);
  
  cout << "\nbal_tree.postOrderTraverse(key_printer) = \n";
  
  bal_tree.postOrderTraverse(key_printer);
  
  cout << '\n' << flush;

  for (auto& x : lst) {

     cout << "--------------------------------\n";
       
     cout << "bal_tree.remove(" << x << ")\n";
     if (x == 15) {
         auto debug = 10;
         ++debug;
     }
     bal_tree.remove(x);
     bal_tree.levelOrderTraverse(key_printer);
  } 

  return 0;
  /*      
  cout << "This is the input tree " << tree << " printed in debug level order:\n";
  
  tree.levelOrderTraverse(???);

  cout << "We now start the removal loop\n";
  
  for (const auto& key : lst) {
      
      cout << "Tree before removal of Test{" << key << "} " << tree; 
      
      if (key == 65) {
          auto debug = 10;
          ++debug;
      }
               
      //tree.remove(Test{key});
      tree.remove(key);

      cout << "Tree after removal of Test{" << key << "}. " << tree; 
      cout << "\nLevel-order print after the removal of Test{" << key << "}\n";
      
      tree.levelOrderTravese(key_printer);

      cout << flush << "\nDebug print\n";

  }

  return 0;
*/
/*

    bstree<Test, Test> tree1 = { 5, 15, 7, 17, 3, 13, 4, 14,  2, 12,  6, 16, 9, 19}; 
    
    cout << "tree1 = " << tree1 << '\n' << "Printing tree1 level order:\n";
    
    tree1.levelOrderTraverse(key_printer);
        
    bstree<Test> tree2 = { Test{15}, Test{25}, Test{17}, Test{27}, Test{13}, Test{23}, Test{14}, Test{24},  Test{12}, Test{41},  Test{16}, Test{6}, Test{19}, Test{20}}; 
    
    cout << "\ntree2 = " << tree2 << "\nPrinting tree2 level order:\n";
    
    tree2 = tree1; 
    tree2.levelOrderTraverse(key_printer);
    
   cout << "\nAfter 'tree2 = tree1', tree2 = " << tree2 << "\nPrinting tree2 level order:\n" << flush;
   tree2.levelOrderTraverse(key_printer);
   
   vector<int> vec = {-5, -15, -7, -17, -3, -13, -4, -14,  -2, -12,  -6, -16, -9, -19}; 
   for (const auto& ref : vec) {

     tree2.remove(ref); 

     cout << "After removing " << ref << " from tree2, tree2 = \n";
     
     tree2.levelOrderTraverse(key_printer);
     cout << flush;
   }
   
   return 0;
 * */
}


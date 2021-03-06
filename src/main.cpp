#include <cstdlib>
#include <utility>
#include <iostream>
#include <iomanip>
#include <initializer_list>
#include "bstree.h"
#include "test.h"

using namespace std;

int main(int argc, char** argv) 
{
  bstree<int, int> tree;

  std::initializer_list<int> lst1 = {7, 1, 30, 0, 3, 8, 50, 20, 9, 2, 5, 4, 6, -10, -5, 40, 60, 55, 65, 54, -20};
  
  test(lst1);

  std::initializer_list<int> lst2 = {100, 50, 200, 20, 70, 150, 250, -10, 40, 60, 90, 125, 175, 225, 275, -40, 10, 30, 45, 55, 65, 80, 95, 110, 130, 165, 190, 220, 230, 260, 290,\
    -70, -30, -5, 15, 25, 35, 42, 47, 52, 57, 62, 67, 92, 97, 105, 115, 127, 135, 160, 170, 180, 195, 210, 222, 227, 235, 260, 280 };
  
  for (const auto& i : lst2) 
      tree.insert(i, i);

   cout << tree;

   decltype(tree) tree_out;

   tree_out = tree;

   cout << "\n====================\n" << tree_out;
   test(lst2); 
 
  return 0;
/*
  t2 = test_tree;
  
  cout << "\ntest_tree.inOrderStackIterative(key_printer) = ";

  auto key_printer = [](const auto& pr) {
      const auto&[key, value] = pr;
      
      cout << setw(3) << key << '\n';
  };

  test_tree.inOrderStackIterative(key_printer);

  cout << flush << "\ntest_tree.preOrderIterative(key_printer) = " ;
  
  test_tree.preOrderIterative(key_printer);
  
  cout << '\n' << flush;

  bstree<int, int> bal_tree;

  for (const auto& i : lst) 
      bal_tree.insert(i, i);
  
  cout << "bal_tree.inOrderStackIterative(key_printer) = \n";
  
  bal_tree.inOrderStackIterative(key_printer);
  cout << "\nUsing range loop\n";
  
  for (const auto& vt : bal_tree) {
        const auto&[key, value] = vt;
        cout << key << ", " << flush;  
  } 

//  bal_tree.printlevelOrder(cout, key_printer); // BUG <-- Why pass cout, when key_printer never uses this parameter! Providing only levelOrderTraverse() is sufficient.
    bal_tree.levelOrderTraverse(key_printer); // BUG <-- Why pass cout, when key_printer never uses this parameter! Providing only levelOrderTraverse() is sufficient.
  
  cout << "bal_tree::preorder_stack_iterator loop: for(iter =..; iter != iter_end; ++iter) = \n";
  
  bal_tree.inOrderStackIterative(key_printer);

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
 */ 
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


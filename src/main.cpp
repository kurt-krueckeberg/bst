#include <cstdlib>
#include <utility>
#include <iostream>
#include <initializer_list>
#include "test.h"
#include "bstree.h"

using namespace std;


int main(int argc, char** argv) 
{
  std::initializer_list<int> lst = {100, 50, 200, 20, 70, 150, 250, -10, 40, 60, 90, 125, 175, 225, 275, -40, 10, 30, 45, 55, 65, 80, 95, 110, 130, 165, 190, 220, 230, 260, 290,\
    -70, -30, -5, 15, 25, 35, 42, 47, 52, 57, 62, 67, 92, 97, 105, 115, 127, 135, 160, 170, 180, 195, 210, 222, 227, 235, 260, 280 };

  bstree<int, int> balanced_tree;

  for (const auto& i : lst) 
      balanced_tree.insert(i, i);
   
  auto key_printer = [](const auto& pr) {
      const auto&[key, value] = pr;
      cout << key << ", ";
  };
  
  balanced_tree.printlevelOrder(cout, key_printer);

  cout << "\n--------------\nPrinting tree_copy, a copy of the above balanced_tree.\n"; 

  int hidden = 0;

  auto setter = [&](int& int_ref) { int_ref = ++hidden; }; 

  balanced_tree.set_special(setter);

  auto sp_printer = [&](const pair<const int, int>& pr, const int& int_ref) {
      const auto&[key, value] = pr;
      cout << '(' << int_ref << ") [" << key << "], " << flush; }; 

  balanced_tree.visit_special(sp_printer); //TODO:  Want level order print that prints __d. In general, we want to print __d at will.

  bstree<int, int> tree_copy = balanced_tree;

  tree_copy.printlevelOrder(cout, key_printer);
  
  cout << "floor(37) = " << balanced_tree.floor(37) << '\n';

  cout << "ceiling(37) = " << balanced_tree.ceiling(37) << '\n';
 
  cout << "floor(41) = " << balanced_tree.floor(41) << '\n';

  cout << "ceilling(41) = " << balanced_tree.ceiling(41) << '\n';

  balanced_tree.printlevelOrder(cout, key_printer);

  for (auto& x : lst) {

     cout << "--------------------------------\n";
       
     cout << "balanced_tree.remove(" << x << ")\n";
     if (x == 15) {
         auto debug = 10;
         ++debug;
     }
     balanced_tree.remove(x);
     balanced_tree.printlevelOrder(cout, key_printer);
  } 

  return 0;
  /*      
  cout << "This is the input tree " << tree << " printed in debug level order:\n";
  
  tree.debug_printlevelOrder(cout);

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
      
      tree.printlevelOrder(cout, key_printer);

      cout << flush << "\nDebug print\n";

      tree.debug_printlevelOrder(cout);
  }

  return 0;
*/
/*

    bstree<Test, Test> tree1 = { 5, 15, 7, 17, 3, 13, 4, 14,  2, 12,  6, 16, 9, 19}; 
    
    cout << "tree1 = " << tree1 << '\n' << "Printing tree1 level order:\n";
    
    tree1.printlevelOrder(cout);
        
    bstree<Test> tree2 = { Test{15}, Test{25}, Test{17}, Test{27}, Test{13}, Test{23}, Test{14}, Test{24},  Test{12}, Test{41},  Test{16}, Test{6}, Test{19}, Test{20}}; 
    
    cout << "\ntree2 = " << tree2 << "\nPrinting tree2 level order:\n";
    
    tree2 = tree1; 
    tree2.printlevelOrder(cout);
    
   cout << "\nAfter 'tree2 = tree1', tree2 = " << tree2 << "\nPrinting tree2 level order:\n" << flush;
   tree2.printlevelOrder(cout);
   
   vector<int> vec = {-5, -15, -7, -17, -3, -13, -4, -14,  -2, -12,  -6, -16, -9, -19}; 
   for (const auto& ref : vec) {

     tree2.remove(ref); 

     cout << "After removing " << ref << " from tree2, tree2 = \n";
     
     tree2.printlevelOrder(cout);
     cout << flush;
   }
   
   return 0;
 * */
}


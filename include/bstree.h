#ifndef bst_h_18932492374
#define bst_h_18932492374

#include <memory>
#include <utility>
#include <queue>
#include <stack>
#include <list>
#include <algorithm>
#include <stdlib.h>
#include <initializer_list>
#include "value-type.h"
#include <iostream>  
#include <iomanip>
#include <exception>
#include <iterator>

template<class Key, class Value> class bstree; // Forward declaration.

template<class Key, class Value> class bstree {

  public:

    // Container typedef's used by STL.
    using key_type   = Key;
    using mapped_type = Value;

    using value_type = __value_type<Key, Value>::value_type;// = std::pair<const Key, Value>;  
    using difference_type = long int;
    using pointer         = value_type*; 
    using reference       = value_type&; 

  private:
   /*
    * The tree nodes are of type std::unique_ptr<Node>, and each node contains a __value_type member __vt, a convenience 
      wrapper for access to a pair<const Key, Value>. 
    */ 
   class Node {

        friend class bstree<Key, Value>;    

    public:   
        
        Node() : parent{nullptr}
        {
        }

        // Due to stack overflow concerns, the default ctor, which would successfully copy the the entire subtree of lhs,
        // is deleted.
        Node(const Node& lhs) : __vt{lhs.__vt}, left{nullptr}, right{nullptr} // = delete; 
        {
        }
        
        Node(const __value_type<Key, Value>& vt, Node *in_parent=nullptr) : __vt{vt}, left{nullptr}, right{nullptr}, parent{in_parent}
        {
        }  
        
        constexpr Node(const Key& key, const Value& value, Node *parent_in=nullptr) : __vt{key, value}, parent{parent_in}, left{nullptr}, right{nullptr} 
        {
        }
      
        Node& operator=(const Node&) noexcept; 

        Node(Node&&); // ...but we allow move assignment and move construction.
        /*
          '~Node() = default;' implictily invokes the Node destructor for left and right, which results in the recursive destruction of the entire subtree rooted at *this. However,
           this can cause stack overflow, especially if the Node being destructed is the root. To avoid this, ~bstree() calls destroy_subtree(root), which does a post-order traversal,
           calling node.reset(). The unique_ptr<Node>::reset() will invoke the ~Node destructor, which will implicitly invoke the destructor left and right. However, the post-order
           traversal ensures that left and right will already be nullptr (and thus no infinite recursion can occur).
         */

       ~Node() = default; // TODO: How can we make the dtor only delete  Node::__vt but not Node::left and Node::right
       
        std::ostream& print(std::ostream& ostr) const noexcept; 

        std::ostream& debug_print(std::ostream& ostr) const noexcept;

         constexpr friend std::ostream& operator<<(std::ostream& ostr, const Node& node) noexcept
        { 
            node.print(ostr);
            return ostr;
        }
        
        Node& operator=(Node&&) noexcept;

        constexpr void connectLeft(std::unique_ptr<Node>& node) noexcept
        {
            left = std::move(node);
            left->parent = this;
        }  

        constexpr void connectRight(std::unique_ptr<Node>& node) noexcept 
        {
            right = std::move(node);
            right->parent = this;
        }  

        constexpr void connectLeft(const Node& node) noexcept
        {
            left = std::make_unique<Node>(node);  
            left->parent = this;
        }  

        constexpr void connectRight(const Node& node) noexcept 
        {
            right = std::make_unique<Node>(node);    
            right->parent = this;
        }  

    private:

        __value_type<Key, Value> __vt;  // Convenience wrapper for std::pair<const Key, Value>

        int __order;           // This is for education purposes only
                              
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node *parent;

        constexpr const value_type&  __get_value() const noexcept
        {
	    return __vt.__get_value();
        }

        constexpr value_type&  __get_value() noexcept
        {
	    return __vt.__get_value();
        }

        constexpr const Key& key() const noexcept 
        {
           return __get_value().first; 
        } 

        constexpr const Value& value() const noexcept 
        { 
           return __get_value().second; 
        }  
        
        constexpr Value& value() noexcept 
        { 
           return __get_value().second; 
        }
    }; 

   template<typename Printer> class LevelOrderPrinter {
   
      int current_level;
      int height_;
      Printer do_print;
       
      std::ostream& (Node::*pmf)(std::ostream&) const noexcept;

      void display_level(int level) const noexcept
      {
         std::cout << "\n" << "current level = " <<  level << '\n'; 
      }
      
      public: 
      
      LevelOrderPrinter (const bstree<Key, Value>& tree, Printer p):  current_level{0}, do_print{p}
      { 
          height_ = tree.height(); 
      }

      LevelOrderPrinter (const LevelOrderPrinter& lhs) : current_level{lhs.current_level}, height_{lhs.height_}, do_print{lhs.do_print} {}
      
      void operator ()(const Node *pnode, int level)
      { 
          // Did current_level change?
          if (current_level != level) { 
         
              current_level = level;
         
              display_level(level);       
          }

          //--do_print(pnode->__get_value());
          std::cout << std::setw(3) << std::right << pnode->key();
         
          std::cout << '\n' << std::flush;
      }
   };

  private: 

    std::unique_ptr<Node> root; 

    std::size_t size;

    template<typename Functor> void inOrderTraverse(Functor f, const std::unique_ptr<Node>& root) const noexcept;
    template<typename Functor> void postOrderTraverse(Functor f,  const std::unique_ptr<Node>& root) const noexcept;
    template<typename Functor> void preOrderTraverse(Functor f, const std::unique_ptr<Node>& root) const noexcept;
    
    template<typename Functor> void inOrderTrace(Functor f, const std::unique_ptr<Node>& current, std::list<int>& list, int depth=1) const noexcept;

    template<typename Functor> void inOrderIterative(Functor f, const std::unique_ptr<Node>& root) const noexcept;

    template<typename Functor> void postOrderIterative(Functor f, const std::unique_ptr<Node>& root) const noexcept;

    template<typename Functor> void __postOrderIterative(Functor f, std::unique_ptr<Node>& root) noexcept; // private method. Mainly used to do post-order tree destruction
    template<typename Functor> void preOrderIterative(Functor f, const std::unique_ptr<Node>&) const noexcept;

    constexpr Node *min(std::unique_ptr<Node>& current) const noexcept
    {
        return min(current.get());
    }

    Node *min(Node *current) const noexcept;
   
    Node *getSuccessor(const Node *current) const noexcept;

    std::unique_ptr<Node>& get_unique_ptr(Node *pnode) noexcept;

    std::pair<bool, const Node *> findNode(const key_type& key, const Node *current) const noexcept; 

    int  height(const Node *pnode) const noexcept;
    int  depth(const Node *pnode) const noexcept;
    bool isBalanced(const Node *pnode) const noexcept;

    void move(bstree<Key, Value>&& lhs) noexcept;

    std::unique_ptr<Node>& find(Key key, std::unique_ptr<Node>&) const noexcept;

    void copy_tree(const std::unique_ptr<Node>& src) noexcept;

    constexpr Node *get_floor(Key key) const noexcept
    {
      const auto& pnode = get_floor(root, key);
   
      return pnode.get();
    }

    const std::unique_ptr<Node>& get_floor(const std::unique_ptr<Node>& current, Key key) const noexcept;
    
    constexpr Node *get_ceiling(Key key) const noexcept
    {
      const std::unique_ptr<Node>& pnode = get_ceiling(root, key);
      
      return pnode.get();
    }
    
    const std::unique_ptr<Node>& get_ceiling(const std::unique_ptr<Node>& current, Key key) const noexcept;

  public:
/*
Some prospective methods found in std::map:
    template< class InputIt >
    void insert( InputIt first, InputIt last );
    
    void insert( std::initializer_list<value_type> ilist );
    
    insert_return_type insert(node_type&& nh);
    
    iterator insert(const_iterator hint, node_type&& nh);
    
    void insert( std::initializer_list<value_type> ilist );
    
    insert_return_type insert(node_type&& nh);
    
    iterator insert(const_iterator hint, node_type&& nh);
    template< class InputIt >
    void insert( InputIt first, InputIt last );
From std::map insert_or_assign methods
    template <class M>
    pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj);
    template <class M>
    pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj);
    template <class M>
    iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj);
    template <class M>
    iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj);
*/

    // One other stl typedef.
    using node_type       = Node; 
  
    bstree() noexcept : root{nullptr}, size{0} { }

    // While the default destructor successfully frees all nodes. A huge recursive call invokes every Node's destructor.
    // will be invoke in one huge recursive call 
    ~bstree() noexcept
    {
       auto f = [] (std::unique_ptr<Node>& node) {
          node.reset();
       };
    
       __postOrderIterative(f, root);
    } 

    bstree(std::initializer_list<value_type>& list) noexcept; 

    bstree(const bstree&) noexcept; 

    bstree(bstree&& lhs) noexcept
    {
        move(std::move(lhs)); 
    }

    bstree& operator=(const bstree&) noexcept; 

    bstree& operator=(bstree&&) noexcept;

    bstree<Key, Value> clone() const noexcept; 

    constexpr bool isEmpty() const noexcept
    {
      return (size == 0) ? true : false;
    }

    void test_invariant() const noexcept;

    const Value& operator[](Key key) const;

    Value& operator[](Key key);
    
    std::size_t count() const noexcept
    {
        return size;
    }

    void insert(std::initializer_list<value_type>& list) noexcept; 

    constexpr bool insert(const key_type& key, const mapped_type& value) noexcept
    {
        return insert_or_assign(key, value);
    }

    bool insert_or_assign(const key_type& key, const mapped_type& value) noexcept; 
  
    Value& operator[](const Key& key) noexcept; 

    const Value& operator[](const Key& key) const noexcept; 

    // TODO: Add emplace() methods and other methods like std::map have, like insert_or_assign().

    constexpr bool remove(Key key) noexcept
    {
        return remove(key, root);
    } 
 
    bool remove(Key key, std::unique_ptr<Node>& subtree) noexcept; 

    constexpr bool find(Key key) const noexcept
    {
       return find(key, root) ? true : false;
    }

    Key floor(Key key) const 
    {
      if (isEmpty()) 
          throw new std::logic_error("floor() called with empty tree");

      const Node *pnode = get_floor(key);
      
      if (!pnode)
          throw new std::logic_error("argument to floor() is too small");
      else 
           return pnode->key();
    }

    Key ceiling(Key key) const 
    {
      if (isEmpty()) 
          throw new std::logic_error("floor() called with empty tree");

      const Node *pnode = get_ceiling(key);
       
      if (!pnode)
          throw new std::logic_error("argument to ceiling() is too large");
      else 
           return pnode->key();
    }
    
    // Breadth-first traversal
    // Functor must accept bstree<Key, Value>::value_type
    template<class Functor> void levelOrderTraverse(Functor f) const noexcept;

    // Functor must accept two parameters: 'const Node *' and 'int level'
    template<class Functor> void node_levelOrderTraverse(Functor f) const noexcept;

    // Depth-first traversals
    template<typename Functor> void inOrderTraverse(Functor f) const noexcept
    { 
      return inOrderTraverse(f, root); 
    }

    template<typename Functor> void inOrderTrace(Functor f) const noexcept
    { 
      std::list<int> list;
      return inOrderTrace(f, root, list); 
    }

    // Depth-first traversals
    template<typename Functor> void inOrderIterative(Functor f) const noexcept
    { 
      return inOrderIterative(f, root); 
    }
    
    template<typename Functor> void preOrderIterative(Functor f) const noexcept
    { 
      return preOrderIterative(f, root); 
    }

    template<typename Functor> void postOrderIterative(Functor f) const noexcept
    { 
      return postOrderIterative(f, root); 
    }

    template<typename Functor> void preOrderTraverse(Functor f) const noexcept  
    { 
      return preOrderTraverse(f, root); 
    }

    template<typename Functor> void postOrderTraverse(Functor f) const noexcept
    { 
      return postOrderTraverse(f, root); 
    }

    template<typename PrintFunctor> void  printlevelOrder(PrintFunctor pf) const noexcept;

    void debug_print(std::ostream& ostr) const noexcept;

    int height() const noexcept;
    bool isBalanced() const noexcept;

    friend std::ostream& operator<<(std::ostream& ostr, const bstree<Key, Value>& tree) noexcept
    {
       std::cout << "{ "; 
       
       auto functor = [](const auto& pair) { 
            const auto&[key, value] = pair;
            std::cout << key  << ", ";
       };
       
       tree.inOrderTraverse(functor);
       
       std::cout << "}\n" << std::flush;
       return ostr;
    }

    class inorder_stack_iterator {  // This not efficient to copy due to the stack container inside it.
    
       using node_type = bstree<Key, Value>::node_type;
    
       std::stack<node_type*> stack; 
    
       node_type *current;
    
       const bstree<Key, Value>& tree;
    
       inorder_stack_iterator& increment() noexcept // Go to next node.
       {
          if (stack.empty()) {
    
            current = nullptr; 
            return *this;
          }
    
          current = stack.top();
    
          stack.pop();
       
          push_leftmost(current->right.get()); // Next, go right, if we can, and push the left-most nodes.
    
          return *this;
       }

       void push_leftmost(Node *pnode)
       {
           while (pnode != nullptr)  { 
    
     	       stack.push(pnode);         //....push the right node onto the stack.
    
      	       pnode = pnode->left.get(); // Then go as far left as we can, pushing all nodes onto stack.
           }
       }

      public:
    
       using difference_type  = std::ptrdiff_t; 
       using value_type       = bstree<Key, Value>::value_type; 
       using reference        = value_type&; 
       using pointer          = value_type*;
           
       using iterator_category = std::forward_iterator_tag; 
    
       explicit inorder_stack_iterator(bstree<Key, Value>& lhs) : tree{lhs}
       {
          push_leftmost(tree.root.get());

          increment();  // go to first Node
       }
       
       inorder_stack_iterator(const inorder_stack_iterator& lhs) : current{lhs.current}, stack{lhs.stack}, tree{lhs.tree}
       {
       }
       
       inorder_stack_iterator(inorder_stack_iterator&& lhs) : current{lhs.current}, stack{std::move(lhs.stack)}, tree{lhs.tree}
       {
           lhs.current = nullptr;
       }
       // TODO: Are assignment operators required?
       inorder_stack_iterator& operator++() noexcept 
       {
          increment();
          return *this;
       } 
       
       inorder_stack_iterator operator++(int) noexcept
       {
          inorder_stack_iterator tmp(*this);
    
          increment();
    
          return tmp;
       } 
         
       reference operator*() const noexcept 
       { 
           return current->__get_value();
       } 
       
       pointer operator->() const noexcept
       { 
          return &(operator*()); 
       } 
       
       struct sentinel {}; // Use for determining "at the end" in 'bool operator==(const inorder_stack_iterator&) const' below
    
       bool operator==(const inorder_stack_iterator::sentinel& sent) const noexcept
       {
          return stack.empty(); // We are done iterating when the stack becomes empty.
       }
       
       bool operator!=(const inorder_stack_iterator::sentinel& lhs) const noexcept
       {
         return !operator==(lhs);    
       }
    };
    inorder_stack_iterator begin() noexcept
    {
       inorder_stack_iterator iter{*this}; 
       return iter; 
    }
    
    inorder_stack_iterator::sentinel end() noexcept // TODO: Can I use a sentinel? a C++17 feature.
    {
        typename inorder_stack_iterator::sentinel sent;
        return sent;
    }
    // preorder stack-based iterator
     /* 
     * Converted Pre-order Java iterator. This is NOT an in-order iterator!!
     *
     * The Binary Trees lecture is at:
     * 
     * https://www.cs.cmu.edu/~adamchik/15-121/lectures/Trees/trees.html
     *
     * The BST code with the Pre-order iterator class was taken from:
     */
    class preorder_stack_iterator {
        
       using node_type = bstree<Key, Value>::node_type;
       
       std::stack<node_type*> stack; 
       
       node_type *current;
    
       const bstree<Key, Value>& tree;
    
       preorder_stack_iterator& increment() 
       {
          if (stack.empty())
                throw std::logic_error("No such element"); //TODO: logic_error?
        
           current = stack.top();

           stack.pop();

           if (current->right)
               stack.push(current->right.get());

           if (current->left) 
               stack.push(current->left.get());
        
           return *this;
        
         /* Alternate implementation
          current = stack.top();
          
          if (current->left)       // If left not nullptr, push it onto stack.  
          
             stack.push(current->left.get());
          
          else {
          
             Node *tmp = stack.top();
    
             stack.pop(); 
          
             while (!tmp->right) {  // While tmp->right is nullptr
          
                if (stack.empty()) 
                     return *this;  // current->__get_value();
          
                tmp = stack.top();
                stack.pop();  
             }
          
             stack.push(tmp->right.get());
          }
          
          return *this;  
          */ 
       }  
    
     public:
       // traits for forward iterator
       using difference_type  = std::ptrdiff_t; 
       using value_type       = bstree<Key, Value>::value_type; 
       using reference        = value_type&; 
       using pointer          = value_type*;
       
       using iterator_category = std::forward_iterator_tag; 
	    
       preorder_stack_iterator(const bstree<Key, Value>& lhs) : tree{lhs}
       {
          if (tree.root) {
    
              stack.push(tree.root.get());
              increment();  // Set current
          }
       }
        
       preorder_stack_iterator(const preorder_stack_iterator& lhs) : current{lhs.current}, stack{lhs.stack}, tree{lhs.tree}
       {
       }
       
       preorder_stack_iterator(preorder_stack_iterator&& lhs) : current{lhs.current}, stack{std::move(lhs.stack)}, tree{lhs.tree}
       {
           lhs.current = nullptr;
       }
       // TODO: Are assignment operators required?
       
       preorder_stack_iterator& operator++() 
       {
          increment();
          return *this;
       } 
       
       preorder_stack_iterator operator++(int) 
       {
          preorder_stack_iterator tmp(*this);
       
          increment();
       
          return tmp;
       } 
         
       reference operator*() const noexcept 
       { 
           return current->__get_value();
       } 
       
       pointer operator->() const noexcept
       { 
          return &(operator*()); 
       } 
    
       struct sentinel {};
    
       bool operator==(const sentinel& sent) const noexcept
       {
          return stack.empty();
       }
    
       bool operator!=(const sentinel& sent) const noexcept
       {
          return !operator==(sent);
       }
    
       value_type& operator*() 
       {
           return current->__get_value();
       }
    };  
    
    preorder_stack_iterator pre_begin() noexcept
    {
       preorder_stack_iterator iter{*this}; 
       return iter; 
    }
    
    preorder_stack_iterator::sentinel pre_end() noexcept // TODO: Can I use a sentinel? a C++17 feature.
    {
        typename preorder_stack_iterator::sentinel sent;
        return sent;
    }
};

// pprovided for symmetry
template<typename Key, typename Value>
inline bool operator==(const typename bstree<Key, Value>::inorder_stack_iterator::sentinel& sent, const typename bstree<Key, Value>::inorder_stack_iterator& iter)  noexcept
{
     return iter == sent;    
}    

/*

 The Node copy ctor could be implemented recursively as below, but this results in more recursive calls and possible
 stack overflow than using a pre-order tree traversal that copies the input node. Therefore the default copy ctor 
 is deleted.
 
template<class Key, class Value>
bstree<Key, Value>::Node::Node(const Node& lhs) : __vt{lhs.__vt}, left{nullptr}, right{nullptr}
{
   if (!lhs.parent) // If lhs is the root, then set parent to nullptr.
       parent = nullptr;

   // This will recursively invoke the constructor again, resulting in the entire tree rooted at
   // lhs being copied.

   if (lhs.left) 
       connectLeft(*lhs.left); 
   
   if (lhs.right) 
       connectRight(*lhs.right); 
}
*/

/*
 The Node assignment operatorr could be implemented recursively as below, but this results in more recursive calls (and 
 possible stack overflow) than using a pre-order tree traversal that copies the input node. 
 
template<class Key, class Value> typename bstree<Key, Value>::Node&  bstree<Key, Value>::Node::operator=(const typename bstree<Key, Value>::Node& lhs) noexcept
{
   if (&lhs == this) return *this;

   __vt = lhs.__vt;

   if (lhs.parent == nullptr) // If we are copying a root pointer, then set parent.
       parent = nullptr;

   // The make_unique<Node> calls below results in the entire tree rooted at lhs being copied.
   if (lhs.left) 
       connectLeft(*lhs.left); 
   
   if (lhs.right)
       connectRight(*lhs.right); 
  
   return *this;
}
*/

/*
 The Node assignment operatorr could be implemented recursively as below, but this results more recursive calls than using
 a pre-order tree traversal that copies the input node. 
 */
template<class Key, class Value> typename bstree<Key, Value>::Node&  bstree<Key, Value>::Node::operator=(const typename bstree<Key, Value>::Node& lhs) noexcept
{
   if (&lhs == this) return *this;

   auto f = [] (std::unique_ptr<Node>& node) {
      node.reset();
   };

   __postOrderIterative(f, root);

   copy_tree(lhs.root);

/*
   __vt = lhs.__vt;

   if (lhs.parent == nullptr) // If we are copying a root pointer, then set parent.
       parent = nullptr;

   // The make_unique<Node> calls below results in the entire tree rooted at lhs being copied.
   if (lhs.left) 
       connectLeft(*lhs.left); 
   
   if (lhs.right)
       connectRight(*lhs.right); 
*/  
   return *this;
}


template<class Key, class Value> inline bstree<Key, Value>::bstree(std::initializer_list<value_type>& list)  noexcept : bstree()
{
   insert(list);
}

template<class Key, class Value> inline bstree<Key, Value>::bstree(const bstree<Key, Value>& lhs) noexcept : size{lhs.size}, root{nullptr}
{ 
   if (!lhs.root) return;
    
   copy_tree(lhs.root);
}

template<class Key, class Value> inline void bstree<Key, Value>::move(bstree<Key, Value>&& lhs) noexcept  
{
  root = std::move(lhs.root); 

  size = lhs.size;

  lhs.size = 0;
}

template<class Key, class Value> bstree<Key, Value>& bstree<Key, Value>::operator=(const bstree<Key, Value>& lhs) noexcept
{
  if (this == &lhs)  return *this;
  
  auto f = [] (std::unique_ptr<Node>& uptr) {
      uptr.reset();
  };
  
  __postOrderIterative(f, root);
  
  copy_tree(lhs.root);
 
  size = lhs.size; 

  return *this;
}

template<class Key, class Value> bstree<Key, Value>& bstree<Key, Value>::operator=(bstree<Key, Value>&& lhs) noexcept
{
  if (this == &lhs) return *this;
  
  move(std::move(lhs)); 

  return *this;
}

template<class Key, class Value> inline std::ostream& bstree<Key, Value>::Node::print(std::ostream& ostr) const noexcept
{
  ostr << "[ " << key() << ", " << value() << "] " << std::flush;  
  return ostr; 
}

template<class Key, class Value> std::ostream& bstree<Key, Value>::Node::debug_print(std::ostream& ostr) const noexcept
{
   ostr << " {["; 
 
   ostr << key() << ']';

   if (parent) 
      ostr << ", parent->key() =" << parent->key(); 
   else
      ostr << ", parent = nullptr";
 
   if (!left) 
     ostr << ", left = nullptr";
   else
      ostr << ", left->key() = " <<  left->key();
   
   if (!right) 
     ostr << ", right = nullptr";
   else
      ostr << ", right->key() = " << right->key();
   
   ostr << "}";
 
   return ostr;
}

template<typename Key, typename Value> 
template<typename PrintFunctor>
void  bstree<Key, Value>::printlevelOrder(PrintFunctor print_functor) const noexcept
{
  LevelOrderPrinter<PrintFunctor> tree_printer(*this, print_functor);  
  
  node_levelOrderTraverse(tree_printer);
  
  std::cout << std::flush;
}

template<typename Key, typename Value> inline void  bstree<Key, Value>::debug_print(std::ostream& ostr) const noexcept
{
  auto node_debug_printer = [&ostr] (const Node *current) { current->debug_print(ostr); };

  printlevelOrder(ostr, node_debug_printer);  
  
  ostr << std::flush;
}

template<class Key, class Value> inline bstree<Key, Value>::Node::Node(Node&& node) : __vt{std::move(node.__vt)}, left{std::move(node.left)}, right{std::move(node.right)}, parent{node.ptr2parent} 
{
}

/*
 * Input:  pnode is a raw Node *.
 * Return: A reference to the unique_ptr that manages pnode.
 */
template<class Key, class Value> std::unique_ptr<typename bstree<Key, Value>::Node>& bstree<Key, Value>::get_unique_ptr(Node *pnode) noexcept
{
  if (!pnode->parent) { // Is pnode the root? 

     return root; 

  } else {

     return (pnode->parent->left.get() == pnode) ? pnode->parent->left : pnode->parent->right;  
  }
}

template<class Key, class Value>
template<typename Functor>
void bstree<Key, Value>::inOrderTraverse(Functor f, const std::unique_ptr<Node>& current) const noexcept
{
   if (!current) return;

   inOrderTraverse(f, current->left);

   f(current->__get_value()); 

   inOrderTraverse(f, current->right);
}

template<class Key, class Value>
template<typename Functor>
void bstree<Key, Value>::inOrderIterative(Functor f, const std::unique_ptr<Node>& root_in) const noexcept
{
   if (!root_in) return;
   
   std::stack<const node_type *> stack;

   const Node *y = root_in.get();

   while (y || !stack.empty()) { /* Note: We need to check both y and whether the stack is empty, for consider a tree in which each node (including the root) has one right child and no left child.
                                    Then the inner while loop will only push one node (at a time) which will then be popped and visited, then y will be set to y->right.  The stack will be empty, but
                                    the next node to visit, y, will not be null.
                                    On the other hand, after the line y = y->right.get(), y will become null whenever its parent is a leaf node that was just been visited. In this case, the stack will
                                    not be null, unless y's parent was the right most node in the tree. 
                                   
                                  */
      while (y) { // put y and its left-most descendents onto the stack
      
         stack.push(y);
         y = y->left.get();
      } 

      y = stack.top();

      stack.pop();

      f(y->__get_value());  
      
      y = y->right.get(); // repeat the process with current's right child.
   }
}

template<class Key, class Value> template<typename Functor> void bstree<Key, Value>::inOrderTrace(Functor f, const std::unique_ptr<Node>& current, std::list<int>& list, int depth) const noexcept
{
   if (!current) {

      return;
   }

   list.push_back(current->key());

   inOrderTrace(f, current->left, list, depth + 1);
   
   std::cout << '[';
   for (auto riter = list.rbegin(); riter != list.rend(); ++riter)
       
      // Print out the simulated "stack". USe code above.
      std::cout << *riter << ", ";    //std::cout << '\t' << std::setw(4) << *riter << std::endl;
      
   std::cout << ']' << std::endl;
   
   f(current->__get_value()); 
    
   inOrderTrace(f, current->right, list, depth + 1);
   
   list.pop_back();
}

//TODO: Test
template<class Key, class Value>
void bstree<Key, Value>::copy_tree(const std::unique_ptr<typename bstree<Key, Value>::Node>& root_in) noexcept
{
   if (!root_in) { 
        root = nullptr;
        return;
   } 
  
   std::stack<const node_type *> stack; 
   stack.push(root_in.get()); 
  
   /*
     Pop all items one by one, and do the following for every popped item:
 
      a) invoke f 
      b) push its right child 
      c) push its left child 

   Note: the right child is pushed first so that left is processed first 
    */
   Node *prior_node = nullptr;
   while (!stack.empty()) { 

       auto pnode = stack.top();
       stack.pop(); 

       std::unique_ptr<Node> new_node = std::make_unique<Node>(*pnode); 

       if (!prior_node)
           root = std::move(new_node);

       else { 
       
           bool is_left_child = pnode->parent->left.get() == pnode ? true : false; 
   
           if (is_left_child) { 

               prior_node->connectLeft(new_node);    
               prior_node = prior_node->left.get(); 

           } else  {

               prior_node->connectRight(new_node);    
               prior_node = prior_node->right.get(); 
           }
       }

       // Push right and left non-null children of the popped node to stack 
       if (pnode->right) 
           stack.push(pnode->right.get()); 

       if (pnode->left) 
           stack.push(pnode->left.get()); 
   } 
}

template<class Key, class Value>
template<typename Functor>
void bstree<Key, Value>::preOrderIterative(Functor f, const std::unique_ptr<Node>& lhs) const noexcept
{
   if (!lhs) return;
  
    std::stack<const node_type *> stack; 
    stack.push(root.get()); 
  
    /*
      Pop all items one by one, and do the following for every popped item:
 
       a) invoke f 
       b) push its right child 
       c) push its left child 

    Note: the right child is pushed first so that left is processed first 
     */
    while (!stack.empty()) { 

        // Pop the top item from stack and print it 
        const node_type *node = stack.top(); 
        stack.pop(); 

        f(node->__get_value()); 

        // Push right and left non-null children of the popped node to stack 
        if (node->right) 
            stack.push(node->right.get()); 

        if (node->left) 
            stack.push(node->left.get()); 
    } 
}
/*
post order iterative implementations

1. implementation using two stacks:

  https://www.geeksforgeeks.org/iterative-postorder-traversal/

2. implementation using one stack:

  https://www.techiedelight.com/postorder-tree-traversal-iterative-recursive/

  https://algorithmsandme.com/iterative-postorder-traversal/

  https://www.geeksforgeeks.org/iterative-postorder-traversal-using-stack/

  https://stackoverflow.com/questions/54635756/iterative-postorder-traversal-of-a-binary-tree-with-a-single-stack-how-to-appro       <-- Python

*/
template<class Key, class Value>
template<typename Functor>
void bstree<Key, Value>::__postOrderIterative(Functor f, std::unique_ptr<Node>& root_in) noexcept
{
    // Check for empty tree 
    if (!root_in) 
        return; 
      
    std::stack<Node *> stack;

    Node *__current = root_in.get();

    do { 

        // Move to leftmost node 
        while (__current) { 

            // Push __current's right child and then __current to stack. 
            if (__current->right) 
                stack.push(__current->right.get()); 

            stack.push(__current); 
  
            // Set __current as __current's left child   
            __current = __current->left.get(); 
        } 
  
        // Pop an item from stack and set it as __current     
        __current = stack.top(); 
        stack.pop(); 
  
        // If the popped item has a right child and the right child is not 
        // processed yet, then make sure right child is processed before __current. 
        if (__current->right && stack.top() == __current->right.get())  { 

            stack.pop();  // remove right child from stack 

            stack.push(__current);  // push __current back to stack 

            __current = __current->right.get(); // change __current so that the right  
                                          // child is processed next 

        } else {  // Else invoke f, passing in the __current's data and set __current as NULL 
        
            if (!__current->parent) // __current is the root.get()

                f(root);
 
            else {

                // Using __current->parent, get unique_ptr<Node> reference.
                Node *parent = __current->parent;
                std::unique_ptr<Node>& current = parent->left.get() == __current ? parent->left : parent->right;

                f(current); 

            }

            __current = nullptr; 
        } 
    } while (!stack.empty()); 
}

template<class Key, class Value>
template<typename Functor>
void bstree<Key, Value>::postOrderIterative(Functor f, const std::unique_ptr<Node>& root_in) const noexcept
{
    // Check for empty tree 
    if (!root_in) 
        return; 
      
    std::stack<Node *> stack;

    Node *__current = root_in.get();

    do { 

        // Move to leftmost node 
        while (__current) { 

            // Push __current's right child and then __current to stack. 
            if (__current->right) 
                stack.push(__current->right.get()); 

            stack.push(__current); 
  
            // Set __current as __current's left child   
            __current = __current->left.get(); 
        } 
  
        // Pop an item from stack and set it as __current     
        __current = stack.top(); 
        stack.pop(); 
  
        // If the popped item has a right child and the right child is not 
        // processed yet, then make sure right child is processed before __current. 
        if (__current->right && stack.top() == __current->right.get())  { 

            stack.pop();  // remove right child from stack 

            stack.push(__current);  // push __current back to stack 

            __current = __current->right.get(); // change __current so that the right  
                                          // child is processed next 

        } else {  // Else invoke f, passing in the __current's data and set __current as NULL 
        
            if (!__current->parent) // __current is the root.get()

                f(root->__get_value());
 
            else {

                // Using __current->parent, get unique_ptr<Node> reference.
                Node *parent = __current;
                std::unique_ptr<Node>& current = parent->left.get() == __current ? parent->left : parent->right;

                f(current->__get_value()); 
            }

            __current = nullptr; 
        } 
    } while (!stack.empty()); 
}

template<class Key, class Value>
template<typename Functor>
void bstree<Key, Value>::preOrderTraverse(Functor f, const std::unique_ptr<Node>& current) const noexcept
{
   if (!current) return;

   f(current->__get_value()); 

   preOrderTraverse(f, current->left);

   preOrderTraverse(f, current->right);
}

template<class Key, class Value>
template<typename Functor>
void bstree<Key, Value>::postOrderTraverse(Functor f, const std::unique_ptr<Node>& current) const noexcept
{
   if (!current) return;

   postOrderTraverse(f, current->left);

   postOrderTraverse(f, current->right);

   f(current->__get_value()); 
}

template<class Key, class Value> std::unique_ptr<typename bstree<Key, Value>::Node>& bstree<Key, Value>::find(Key key, std::unique_ptr<Node>& current) const noexcept
{
  if (!current || current->key() == key)
     return current;
  if (key < current->key())
     return find(key, current->left);
  else return find(key, current->right);
}

/*
 * Returns pair<bool, const Node *>, where
 * If key found, {true, Node * of found node}
 * If key not node found, {false, Node * of leadf node where insert should occur}
*/
template<class Key, class Value> std::pair<bool, const typename bstree<Key, Value>::Node *> bstree<Key, Value>::findNode(const key_type& key, const typename bstree<Key, Value>::Node *current) const noexcept
{
  const Node *parent = nullptr;

  while (current != nullptr) {

     if (current->key() ==  key) return {true, current}; 

      parent = current;

      current = (key < current->key()) ? current->left.get() : current->right.get(); 
  }
  
  return {false, parent}; 
}

template<class Key, class Value> typename bstree<Key, Value>::Node *bstree<Key, Value>::min(typename bstree<Key, Value>::Node *current) const noexcept
{
  while (current->left != nullptr) {

       current = current->left.get();
  } 

  return current;  
}

/*
 The code for tree-successor is broken into two cases. If the right subtree of node x is nonempty, then the successor of x is just the left-most node in the right subtree, which is found
 by calling min(x->right). On the other hand, if the right subtree of node x is empty and x has a successor y, then y is the lowest ancestor of x whose left child is also an ancestor of x.
 To find y, we simply go up the tree from x until we encounter a node that is the left child of its parent.
 Pseudo code 
 tree-successor(x)
 {
    if x->right ==  NIL
  
       return min(x->right)
  
   y = x->parent
  
   while (y != NIL and x = y->right)
  
        x = y
  
        y = y->parent
   }
  
   return y
 }
 
 */
template<class Key, class Value>  typename bstree<Key, Value>::Node* bstree<Key, Value>::getSuccessor(const typename bstree<Key, Value>::Node *x) const noexcept
{
  if (!x->right) 
      return min(x->right);

  Node *parent = x->parent;

  // To find the smallest parent of child whose left child is also an ancestor of x, we ascend the parent chain until we find a node that is a left child.
  // If its parent is nullptr, then there we are at the root and there is no successor.  
  while(parent && x == parent->right.get()) {

       x = parent;

       parent = parent->parent;
  }

  return parent;
}

template<class Key, class Value>  
const typename std::unique_ptr<typename bstree<Key, Value>::Node>& bstree<Key, Value>::get_floor(const typename std::unique_ptr<typename bstree<Key, Value>::Node>& pnode, Key key) const noexcept
{   
   if (!pnode) 
       return pnode;

   if (key == pnode->key()) 
      return pnode;

   if (key < pnode->key())
       return get_floor(pnode->left, key);

   auto& pnode_r = get_floor(pnode->right, key);

   if (pnode_r) 
       return pnode_r;   
   else
       return pnode;
}

template<class Key, class Value>  
const typename std::unique_ptr<typename bstree<Key, Value>::Node>& bstree<Key, Value>::get_ceiling(const std::unique_ptr<typename bstree<Key, Value>::Node>& pnode, Key key) const noexcept
{   
   if (!pnode)  // nullptr
       return pnode;

   if (key == pnode->key()) 
       return pnode;

   if(key < pnode->key()) {

      auto& pnode_t = get_ceiling(pnode->left, key); 

      if (pnode_t)  // If pnode_t is not nullptr, return pnode_t
          return pnode_t;
      else 
          return pnode; // else return pnode
   }

   return get_ceiling(pnode->right, key);
}

template<class Key, class Value> void bstree<Key, Value>::insert(std::initializer_list<value_type>& list) noexcept 
{
   for (const auto& [key, value] : list) 

      insert_or_assign(key, value);
}

/*
 * Algorithm from page 294 of Introduction to Alogorithm, 3rd Edition by Cormen, et. al
 *
 */
template<class Key, class Value> bool bstree<Key, Value>::insert_or_assign(const key_type& key, const mapped_type& value) noexcept
{
  Node *parent = nullptr;
 
  Node *current = root.get();
 
  // parent will become the parent of the new node. One of its children that is a nullptr will become the new node. 
  while (current) { 
 
      parent = current;
 
      if (key == current->key()) {

          current->value() = value;
          return false;
      }
 
      else if (key < current->key())
           current = current->left.get();
      else
           current = current->right.get();
  }     
  std::unique_ptr<Node> node = std::make_unique<Node>(key, value, parent); 
  
  if (!parent)
     root = std::move(node); // tree was empty
  else if (node->key() < parent->key())
       parent->left = std::move(node);
  else 
       parent->right = std::move(node);  

  ++size;
  return true;
}

/*
CLRS, 2nd Edition, delete algorithm:
http://staff.ustc.edu.cn/~csli/graduate/algorithms/book6/chap13.htm 
Algorithm pseudo code like that below seems to become confusing when you use C++. The pseudo code doesn't translate to, say,
the use of std::unique_ptr.
tree-delete(z)
  // 1. Determine node y to splice out. It is either the input node z (if z has only one child), or
  // its successor, if y has two children.
  if z->left == NIL or z->right == NIL // case 1: z has only one child
      y =  z
  else                                // case 2: z is an internal node 
      y = tree-successor(z)
  // 2. Set x is to the non-NIL child of y, or to NIL if y has no children.
  if y->left !=  NIL    // If the sucessor is above z, the y->left will not be NIL, or if z              
      x = y->left
  else
      x = y->right  // y->left was NIL 
  if x != NIL
     x->parent = y->parent 
  if y->parent == NIL
      root = x
  else if y == y->parent->left
      y->parent->left = x
  else
      y->parent->right =  x
  if y != z
      z->key = y->key // If y has other fields, copy them, too.
   return y
Deletion CLRS, 3rd Edition
==========================
The overall strategy for deleting a node z from a binary search tree T has three basic cases, but,
as we shall see, one of the cases is a bit tricky (a sub case of the third case).
1. If z has no children, then we simply remove it by modifying its parent to replace z with NIL as its child.
2. If z has just one child, then we elevate that child to take z’s position in the tree
   by modifying z’s parent to replace z by z’s child.
3. If z has two children, then we find z’s successor y—which must be in z’s right subtree—and have y
   take z’s position in the tree. The rest of z’s original right subtree becomes y’s new right subtree,
   and z’s left subtree becomes y’s new left subtree. This case is the tricky one because, as we shall
   see, it matters whether y is z’s right child.
The procedure for deleting a given node z from a binary search tree T takes as arguments pointers to T and z.
It organizes its cases a bit differently from the three cases outlined previously by considering the four
cases shown in Figure 12.4.
1. If z has no left child (part (a) of the figure), then we replace z by its right child, which may or may not
be NIL . When z’s right child is NIL , this case deals with the situation in which z has no children. When z’s
right child is non- NIL , this case handles the situation in which z has just one child, which is its right
child.
2. If z has just one child, which is its left child (part (b) of the figure), then we replace z by its left
   child.
3. Otherwise, z has both a left and a right child. We find z’s successor y, which lies in z’s right subtree
   and has no left child (see Exercise 12.2-5). We want to splice y out of its current location and have it
   replace z in the tree.
    1. If y is z’s right child, then we replace z by y, leaving y’s right child alone.
    2. Otherwise, y lies within z’s right subtree but is not z’s right child.  In this case, we first replace
       y by its own right child, and then we replace z by y.
 */
template<class Key, class Value> bool bstree<Key, Value>::remove(Key key, std::unique_ptr<Node>& root_sub) noexcept // root of subtree
{
  std::unique_ptr<Node>& pnode = find(key, root_sub);
  
  if (!pnode) return false;

  // There are three cases to consider:
 
  // Case 1: If both children are NIL, we can simply delete the node (which sets it to NIL). 
  if (!pnode->left && !pnode->right) 
      pnode.reset();    

  // Case 2: pnode has just one child, thus we elevate that child to take pnode's position in the tree
  // by modifying pnode's parent to replace pnode by it's child.

  /*
   Case 2: Both children are non-NIL. We find pnode's successor y, which we know lies in pnode's right subtree and has no left child.
   We want to splice y out of its current location and have it replace pnode in the tree. There are two cases to consider:
  
   1. The easier case is, if y is pnode's right child, then we replace pnode by y, leaving y’s right child alone. Easy case.
  
   2. Otherwise, y lies within pnode's right subtree but is not pnode's right child (part (d)). In this case, we first
      replace y by its own right child, and then we replace pnode by y.
  */
  else if (pnode->left && pnode->right) {  // (pnode->left && p->right) == true

      if (!pnode->right->left) { // sub-case 1: Since pnode->right->left is NIL, we know the successor must be pnode->right.

          pnode->right->parent = pnode->parent; // Before the move-assignment of pnode with pnode->right, adjust pnode->right->parent
                                                // to be pnode's parent  
 
          pnode = std::move(pnode->right); // move-assign pdnoe with its right child, thus, deleting pnode.

      } else  { 

          // Because pnode has two children, we know its successor y lies within pnode's right subtree.

          Node *suc = min(pnode->right); // In this case, we swap pnode's underlying pointer with y's underlying pointer, and then we
                                         // replace pnode by it's right child, which before the 
                                                        // swap was y's right child.

          // Get the unique_ptr reference corresponding to the raw pointer
          std::unique_ptr<Node>& y = suc->parent->left.get() == suc ? suc->parent->left : suc->parent->right;

          pnode->__vt = std::move(y->__vt); // move-assign successor's values to pnode's values. No pointers change

          y = std::move(y->right);          // Replace successor with its right child.
      }
      
  } else { // Case 3: pnode has only one child. 

      std::unique_ptr<Node>& onlyChild = pnode->left ? pnode->left : pnode->right;

      onlyChild->parent = pnode->parent; // Before the move-assignment, we set onlyChild->parent to 
                                         // pnode's parent.

      pnode = std::move(onlyChild);      // Replace pnode by move-assignmetn with its only non-NIL child, thus, deleting pnode.
  }  

  --size; 

  return true; 
}

template<class Key, class Value> inline int bstree<Key, Value>::height() const noexcept
{
   return height(root.get());
}

/*
 * Returns -1 is pnode not in tree
 * Returns: 0 for root
 *          1 for level immediately below root
 *          2 for level immediately below level 1
 *          3 for level immediately below level 2
 *          etc. 
 */
template<class Key, class Value> int bstree<Key, Value>::depth(const Node *pnode) const noexcept
{
    if (pnode == nullptr) return -1;

    int depth = 0;
      
    for (const Node *current = root; current != nullptr; ++depth) {

      if (current->key() == pnode->key()) {

          return depth;

      } else if (pnode->key() < current->key()) {

          current = current->left;

      } else {

          current = current->right;
      }
    }

    return -1; // not found
}

template<class Key, class Value> int bstree<Key, Value>::height(const Node* pnode) const noexcept
{
   if (pnode == nullptr) {

       return -1;

   } else {

      return 1 + std::max(height(pnode->left.get()), height(pnode->right.get()));
   }
}
 
template<class Key, class Value> bool bstree<Key, Value>::isBalanced(const Node* pnode) const noexcept
{
   if (pnode == nullptr || findNode(pnode->key(), pnode)) return false; 
       
   int leftHeight = height(pnode->leftChild);

   int rightHeight = height(pnode->rightChild);

   int diff = std::abs(leftHeight - rightHeight);

   return (diff == 1 || diff ==0) ? true : false; // return true is absolute value is 0 or 1.
}


// Visits each Node, testing whether it is balanced. Returns false if any node is not balanced.
template<class Key, class Value> bool bstree<Key, Value>::isBalanced() const noexcept
{
   std::stack<Node> nodes;

   nodes.push(root.get());

   while (!nodes.empty()) {

     const Node *current = nodes.pop();

     if (isBalanced(current) == false)  return false; 

     if (current->rightChild != nullptr) 
         nodes.push(current->rightChild);
 
     if (current->leftChild != nullptr) 
         nodes.push(current->leftChild);
   }

   return true; // All Nodes were balanced.
}

// Breadth-first traversal. Useful for display the tree (with a functor that knows how to pad with spaces based on level).
template<class Key, class Value> template<typename Functor> void bstree<Key, Value>::levelOrderTraverse(Functor f) const noexcept
{
   std::queue< std::pair<const Node*, int> > queue; 

   Node* proot = root.get();

   if (proot == nullptr) return;
      
   auto initial_level = 1; // initial, top root level is 1.
   
   // 1. pair.first  is: const tree<Key, Value>::Node23*, the current node to visit.
   // 2. pair.second is: current level of tree.
   queue.push(std::make_pair(proot, initial_level));

   while (!queue.empty()) {

        auto[current, current_level] = queue.front(); // C++17 unpacking.

        f(current->__get_value());  
        
        if(current->left)
            queue.push(std::make_pair(current->left.get(), current_level + 1));  

        if(current->right)
            queue.push(std::make_pair(current->right.get(), current_level + 1));  

        queue.pop(); 
   }
}

// Breadth-first traversal. Useful for display the tree (with a functor that knows how to pad with spaces based on level).
template<class Key, class Value> template<typename Functor> void bstree<Key, Value>::node_levelOrderTraverse(Functor f) const noexcept
{
   std::queue< std::pair<const Node*, int> > queue; 

   Node* proot = root.get();

   if (proot == nullptr) return;
      
   auto initial_level = 1; // initial, top root level is 1.
   
   // 1. pair.first  is: const tree<Key, Value>::Node23*, the current node to visit.
   // 2. pair.second is: current level of tree.
   queue.push(std::make_pair(proot, initial_level));

   while (!queue.empty()) {

       /*
        std::pair<const Node *, int> pair_ = queue.front();
        const Node *current = pair_.first;
        int current_level = pair_.second;
       */

        auto[current, current_level] = queue.front(); // C++17 unpacking.

        f(current, current_level);  
        
        if(current->left)
            queue.push(std::make_pair(current->left.get(), current_level + 1));  

        if(current->right)
            queue.push(std::make_pair(current->right.get(), current_level + 1));  

        queue.pop(); 
   }
}
#endif

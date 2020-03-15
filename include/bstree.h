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

template<typename Key> class stack_tracer {

    std::list<Key> list;
    bool first_time;
public:

    stack_tracer() :first_time{true}
    {
        
    }

    stack_tracer(const stack_tracer&) = default;
    stack_tracer& operator=(const stack_tracer&) = default;

    void push(const Key& key)
    {
      list.push_back(key);
    }

    Key peek() const
    {
       return list.back();  
    }
    
    void pop()
    {
      list.pop_back();
    }
    
    void print()
    {
      if (first_time) {
          std::cout << "\n ";
          first_time = false;
      }  
      std::cout << '[';
    
      for (auto riter = list.rbegin(); riter != list.rend(); ++riter)
           
          // Print out the simulated "stack". USe code above.
          std::cout << *riter << ", ";    //std::cout << '\t' << std::setw(4) << *riter << std::endl;
          
       std::cout << ']' << std::endl;
    }
};
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
        Node(const Node& lhs) : __vt{lhs.__vt}, left{nullptr}, right{nullptr}, parent{nullptr} 
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
          '~Node() = default;' would implictily invoke the Node destructor for left and right, resulting in recursive destruction of the entire trees. This could
           cause stack overflow. To avoid this, ~bstree() calls 

                 node_postOrderIterator([](std::unique_ptr<Node>& ptr){ ptr.reset(); }, root)

           which does an iterative post-order traversal, calling std::unique_ptr<Node>::reset(). 
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

    private:

        __value_type<Key, Value> __vt;  // Convenience wrapper for std::pair<const Key, Value>

        int __order;     // This is for education purposes only
                              
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node *parent;

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
    
    template<typename Functor> void inOrderTrace(Functor f, const std::unique_ptr<Node>& current, stack_tracer<Key>& tracer, int depth=1) const noexcept;
    template<typename Functor> void preOrderTrace(Functor f, const std::unique_ptr<Node>& current, stack_tracer<Key>& tracer, int depth=1) const noexcept;

    template<typename Functor> void inOrderIterative(Functor f, const std::unique_ptr<Node>& root) const noexcept;
    template<typename Functor> void postOrderIterative(Functor f, const std::unique_ptr<Node>& root) const;

    template<typename Functor> void preOrderIterative(Functor f, const std::unique_ptr<Node>&) const noexcept;
    template<typename Functor> void preOrderStackIterative(Functor f, const std::unique_ptr<Node>&) const noexcept;

    // private Node visitation iterative traversals 
    template<typename Functor> void node_postOrderIterative(Functor f, std::unique_ptr<Node>& root) noexcept; 
    template<typename Functor> void node_preOrderIterative(Functor f, const std::unique_ptr<Node>& root) const noexcept; 

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

    Node *find__(Key key, const std::unique_ptr<Node>&) const noexcept;

    Node *find__(Key key) const noexcept
    {
       return find__(key, root);
    }

    bstree<Key, Value> copy_tree(const std::unique_ptr<Node>& src) const noexcept;

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

    // One other stl typedef.
    using node_type       = Node; 
  
    bstree() noexcept : root{nullptr}, size{0} { }

    ~bstree() noexcept
    {
       auto f = [] (std::unique_ptr<Node>& node) {
          node.reset();
       };
    
       node_postOrderIterative(f, root);
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
      stack_tracer<Key> tracer;
      return inOrderTrace(f, root, tracer); 
    }

    template<typename Functor> void preOrderTrace(Functor f) const noexcept
    { 
      stack_tracer<Key> tracer;
      return preOrderTrace(f, root, tracer); 
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
 
    template<typename Functor> void preOrderStackIterative(Functor f) const noexcept
    { 
      return preOrderStackIterative(f, root); 
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

    class stack_iterator_inorder {  // This not efficient to copy due to the stack container inside it.
    
       using node_type = bstree<Key, Value>::node_type;
    
       std::stack<node_type*> stack; 
    
       node_type *current;
    
       const bstree<Key, Value>& tree;
    
       stack_iterator_inorder& increment() noexcept // Go to next node.
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
    
       explicit stack_iterator_inorder(bstree<Key, Value>& lhs) : tree{lhs}
       {
          push_leftmost(tree.root.get());

          increment();  // go to first Node
       }
       
       stack_iterator_inorder(const stack_iterator_inorder& lhs) : current{lhs.current}, stack{lhs.stack}, tree{lhs.tree}
       {
       }
       
       stack_iterator_inorder(stack_iterator_inorder&& lhs) : current{lhs.current}, stack{std::move(lhs.stack)}, tree{lhs.tree}
       {
           lhs.current = nullptr;
       }
       
       stack_iterator_inorder& operator++() noexcept 
       {
          increment();
          return *this;
       } 
       
       stack_iterator_inorder operator++(int) noexcept
       {
          stack_iterator_inorder tmp(*this);
    
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
       
       struct sentinel {}; // Use for determining "at the end" in 'bool operator==(const stack_iterator_inorder&) const' below
    
       bool operator==(const stack_iterator_inorder::sentinel& sent) const noexcept
       {
          return stack.empty(); // We are done iterating when the stack becomes empty.
       }
       
       bool operator!=(const stack_iterator_inorder::sentinel& lhs) const noexcept
       {
         return !operator==(lhs);    
       }
    };
    
    
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
    
       const bstree<Key, Value>& tree; // TODO: const is only needed for a const_iterator.
    
       preorder_stack_iterator& increment() 
       {
          if (stack.empty())
                throw std::logic_error("No such element"); 
        
           current = stack.top();

           stack.pop();

           if (current->right)
               stack.push(current->right.get());

           if (current->left) 
               stack.push(current->left.get());
        
           return *this;
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
              increment();  // Sets current
          }
       }
        
       preorder_stack_iterator(const preorder_stack_iterator& lhs) : current{lhs.current}, stack{lhs.stack}, tree{lhs.tree}
       {
       }
       
       preorder_stack_iterator(preorder_stack_iterator&& lhs) : current{lhs.current}, stack{std::move(lhs.stack)}, tree{lhs.tree}
       {
           lhs.current = nullptr;
       }
       
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
    
    preorder_stack_iterator pre_stack_begin() noexcept
    {
       preorder_stack_iterator iter{*this}; 
       return iter; 
    }
    
    preorder_stack_iterator::sentinel pre_stack_end() noexcept 
    {
        typename preorder_stack_iterator::sentinel sent;
        return sent;
    }

   class iterator_preorder {  // This not efficient to copy due to the stack container inside it.
   
      using node_type = bstree<Key, Value>::node_type;
   
      node_type *current;
      bool at_end = false;
   
      bstree<Key, Value>& tree;
   
      Node *increment() 
      {
        if (at_end) return current;

        Node *__y = current;
      
        if (__y->left) // Prefer left child
            __y = __y->left.get();
        else if (__y->right) // otherwise, the right 
            __y = __y->right.get();
      
        else  { // current is a leaf node
      
           // If leaf is a left child and it's parent has a right child, make it current
           if (current == current->parent->left.get() && current->parent->right) 
               
                  __y = current->parent->right.get();
             
           else {// leaf is a right child (or a left child whose parent does not have a right child).
                 // Ascend the parent chain until we find a parent whose right child's key > current->key()
              
             for(auto parent = __y->parent; 1; parent = parent->parent) {// TODO: Could parent be nullptr, say, when root is only node?          
      
                // When parent's key is > current->key(), we are high enough in the parent chain to determine if the
                // parent's right child's key > current->key(). If it is, this is the preorder successor for the leaf node current. 

                // Note: we combine all three tests--right child of parent exits, parent key is > current's,
                // and parent's right child's key > current's--into one if-test. 
                //--if (parent->right && parent->key() > current->key() && parent->right->key() > current->key()) { 

                if (parent->right && parent->key() > __y->key() && parent->right->key() > __y->key()) { 
                     __y = parent->right.get();
                     break; 
                } 
                if (parent == tree.root.get()) {
                    //--__y = current; // There is no pre-order successor because we ascended to the root, and the root's right child is < current->key().
                    at_end = true;
                    break; 
                }
             } 
           } 
        } 

        return __y;
     }     
      
     public:
   
      using difference_type  = std::ptrdiff_t; 
      using value_type       = bstree<Key, Value>::value_type; 
      using reference        = value_type&; 
      using pointer          = value_type*;
          
      using iterator_category = std::bidirectional_iterator_tag; 
   
      explicit iterator_preorder(bstree<Key, Value>& bstree) : tree{bstree}
      {
         current = bstree.root.get();
      }
      
      iterator_preorder(const iterator_preorder& lhs) : current{lhs.current}, tree{lhs.tree}
      {
      }
      
      iterator_preorder(iterator_preorder&& lhs) : current{lhs.current}, tree{lhs.tree}
      {
          lhs.current = nullptr;
      }
      
      iterator_preorder& operator++() noexcept 
      {
         current = increment();
         return *this;
      } 
      
      iterator_preorder operator++(int) noexcept
      {
         iterator_preorder tmp(*this);
   
         current = increment();
   
         return tmp;
      } 
         
      reference operator*() const noexcept 
      { 
          return current->__get_value(); // May want 'Node *' itself
      } 
      
      pointer operator->() const noexcept
      { 
         return &(operator*()); 
      } 
      
      struct sentinel {}; // Use for determining "at the end" in 'bool operator==(const iterator_preorder&) const' below
   
      bool operator==(const iterator_preorder::sentinel& sent) noexcept
      {
          return at_end; 
      }
      
      bool operator!=(const iterator_preorder::sentinel& lhs) noexcept
      {
        return !operator==(lhs);    
      }
   };

   iterator_preorder begin_pre() noexcept
   {
      iterator_preorder iter{*this}; 
      return iter; 
   }
   
   iterator_preorder::sentinel end_pre() noexcept 
   {
       typename iterator_preorder::sentinel sent;
       return sent;
   }
   /*
     Source for post-order successor logic w/o stack:
     https://www.geeksforgeeks.org/postorder-successor-node-binary-tree/      
   */
   
   class iterator_postorder {  // This not efficient to copy due to the stack container inside it.
   
      using node_type = bstree<Key, Value>::node_type;
   
      node_type *current;

      enum class position {at_beg, between, at_end};
      position pos;
  
      bstree<Key, Value> *ptree;
    
      Node *increment() 
      {
          if (current == nullptr || pos == position::at_end) return current;
           
          Node *__y = current;
        
          // If given node is the right child of its parent or parent's right is empty, then the 
          // parent is postorder successor. 
          auto parent = __y->parent; 
         
          if (!parent->right || __y == parent->right.get()) 
              __y = parent; 
          
          else {
         
             // In all other cases, find the left-most child in the right substree of parent. 
             auto pnode = parent->right.get(); 
          
             while (pnode->left) 
                 pnode = pnode->left.get(); 
     
              __y = parent;
          }          
     
          return __y;
      }     
      
     public:
   
      using difference_type  = std::ptrdiff_t; 
      using value_type       = bstree<Key, Value>::value_type; 
      using reference        = value_type&; 
      using pointer          = value_type*;
          
      using iterator_category = std::forward_iterator_tag; 
    
      iterator_postorder() : current{nullptr}, ptree{nullptr}, pos{position::at_end}
      {
      }

      explicit iterator_postorder(bstree<Key, Value>& tree) : ptree{&tree}
      {
         if (ptree->root == nullptr) {

             pos = position::at_end; 
             current = nullptr;

         } else { 

           pos = position::at_beg;
           // Set current to node with smallest key.
           current = min(ptree->root.get());
         }
      }

      // Ctor for return the iterator_inorder returned by end();  
      iterator_postorder(bstree<Key, Value>& tree, int dummy) : ptree{&tree}
      {
          pos = position::at_end; 
          
         if (ptree->root == nullptr) 
             current = nullptr;
         else 
            // Set current to root 
            current = ptree->root.get();
      }
     
      iterator_postorder(const iterator_postorder& lhs) : current{lhs.current}, ptree{lhs.ptree}, pos{lhs.pos}
      {
      }
      
      iterator_postorder& operator++() noexcept 
      {
        switch (pos) {
      
           case position::at_end:
               break;
            
           case position::at_beg:
           case position::between:
           {
               auto next = increment();

               if (current == next) 
                   pos = position::at_end;
               else
                 current = next; 
           }
           break;
           default:
                 break;
         } 
         return *this;
      }
        
      reference operator*() const noexcept 
      { 
          return current->__get_value(); // May want 'Node *' itself
      } 
      
      pointer operator->() const noexcept
      { 
         return &(operator*()); 
      } 
      
      struct sentinel {}; // Use for determining "at the end" in 'bool operator==(const iterator_postorder&) const' below
   
      bool operator==(const iterator_postorder::sentinel& sent) noexcept
      {
          return (pos == position::at_end) ? true : false; 
      }
      
      bool operator!=(const iterator_postorder::sentinel& lhs) noexcept
      {
        return !operator==(lhs);    
      }
   };
   
   class iterator_inorder {  
       
      using node_type = bstree<Key, Value>::node_type;
   
      node_type *current;

      enum class position {at_beg, between, at_end};
      position pos;
  
      bstree<Key, Value> *ptree;
      
      Node *increment()
      {
          if (current == nullptr || pos == position::at_end) 
              return current;
          
          Node *__y = current;

          if (__y->right) { // current has a right child, a greater value to the right
        
              __y = __y->right.get();
        
              while (__y->left) // Get the smallest value in its right subptree, the smallest value in the r. subptree.
                 __y = __y->left.get();
        
          } else {
        
              auto parent = __y->parent;
      
              // Ascend to the first parent ancestor that is not a right child, 
              // and thus is greater than __y 
              while (__y == parent->right.get()) {
      
                  if (parent == ptree->root.get())  // We reached the root -> there is no successor
                      return current;
                         
                  __y = parent;
      
                  parent = parent->parent;
              }
   
              __y = parent; // First parent ancestor that is not a right child. 
          }

          return __y;
      }
      
      Node *decrement()
      {
         if (current == nullptr || pos == position::at_beg) 
             return current;

         Node *__x = current; 
       
         if (__x->left) { // Unlike increment() we check left child before right child. 
        
              auto __y = __x->left.get();
        
              while (__y->right) // Get its largest value. This is the predecessor to current.
                __y = __y->right.get();
        
              __x = __y;
        
          } else { // When we ascend, we look for a parent ancestor that is not a left child, unlike increment that looks for 'not a right child'.
        
              auto parent = __x->parent;
      
              // Ascend to first parent ancestor that is not a left child
              // and thus is less than __x.
              while (__x == parent->left.get()) {
      
                 if (parent == ptree->root.get()) // The parent is the root -> there is no predecessor.
                     return current;             
                 
                  __x = parent;
                  parent = parent->parent;
              }
        
              __x = parent; // Set __x to first parent less than __x.
          }

          return __x;
      }

      Node *min(Node *__y)  
      {
         if (__y == nullptr) return __y;

         while(__y->left) 
            __y = __y->left.get();

         return __y;
      } 
 
      Node *max(Node *__y)  
      {
         if (__y == nullptr) return __y;

         while(__y->right) 
            __y = __y->right.get();

         return __y;
      }     


     public:
   
      using difference_type  = std::ptrdiff_t; 
      using value_type       = bstree<Key, Value>::value_type; 
      using reference        = value_type&; 
      using pointer          = value_type*;
          
      using iterator_category = std::bidirectional_iterator_tag; 
   
      iterator_inorder() : current{nullptr}, ptree{nullptr}, pos{position::at_end}
      {
      }

      explicit iterator_inorder(bstree<Key, Value>& tree) : ptree{&tree}
      { 
         if (ptree->root == nullptr) {

             pos = position::at_end; 
             current = nullptr;

         } else { 

           pos = position::at_beg;
           // Set current to node with smallest key.
           current = min(ptree->root.get());
         }
      } 
      
      // Ctor for return the iterator_inorder returned by end();  
      iterator_inorder(bstree<Key, Value>& tree, int dummy) : ptree{&tree}
      {
          pos = position::at_end; 
          
         if (ptree->root == nullptr) 
             current = nullptr;
         else 
           // Set current to node with largest key.
           current = max(ptree->root.get());
      }

      iterator_inorder(const iterator_inorder& lhs) : current{lhs.current}, ptree{lhs.ptree}, pos{lhs.pos}
      {
      }
        
      iterator_inorder& operator=(const iterator_inorder& lhs)
      {
          if (this == &lhs)
              return *this;

          current = lhs.current;
          ptree = lhs.ptree;
          pos = lhs.pos; 

          return *this;
      }
 
      iterator_inorder& operator++() noexcept 
      {
        switch (pos) {
      
           case position::at_end:
               break;
            
           case position::at_beg:
           case position::between:
           {
               auto next = increment();

               if (current == next) 
                   pos = position::at_end;
               else
                 current = next; 
           }
           break;
           default:
                 break;
      
         } 
         return *this;
      }
      
      iterator_inorder operator++(int) noexcept
      {
         iterator_inorder tmp(*this);
   
         operator++();
   
         return tmp;
      } 
       
      iterator_inorder& operator--() noexcept 
      {
         switch(pos) {
   
             case position::at_beg:
                break; 
            
             case position::at_end:
                 pos = position::between;
                 break;
   
             case position::between: 
             {     
               auto prev = decrement();
            
              if (prev == current) 
                 pos = position::at_beg;
              else
                  current = prev;
             } 
             break;
         
             default:
                 break;
         } 
         return *this;
      } 
      
      iterator_inorder operator--(int) noexcept
      {
         iterator_inorder tmp(*this);
   
         operator--();
   
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
   
      friend bool
      operator==(const iterator_inorder& __x, const iterator_inorder& __y) noexcept
      {
      
        if (__x.ptree == __y.ptree) {
        
           // If we are not in_between...check whether both iterators are at the end...
           if (__x.pos == position::at_end && __y.pos == position::at_end) { 
        
               return true;
        
           } else if (__x.pos == position::at_beg && __y.pos == position::at_beg) { // ...or at beg. 
        
               return true;
        
           } else if (__x.pos == __y.pos && __x.current == __y.current) { // else check whether pos and current are all equal.
               return true;
          }
        }
        
        return false;
      }

      friend bool
      operator!=(const iterator_inorder& __x, const iterator_inorder& __y) noexcept 
      {
         return !operator==(__x, __y); 
      }
   };
   
   iterator_inorder begin() noexcept
   {
       iterator_inorder iter{*this}; 
       return iter; 
   }
    
   iterator_inorder end() noexcept 
   {
       iterator_inorder iter{*this, 1};
       return iter;  
   }
   
   using reverse_iterator = std::reverse_iterator<iterator_inorder>;
   
   reverse_iterator rbegin() noexcept  
   {
      return std::make_reverse_iterator(this->end());
   }    

   reverse_iterator rend() noexcept
   {
      return std::make_reverse_iterator(this->begin());
   }    
};

// pprovided for symmetry
template<typename Key, typename Value>
inline bool operator==(const typename bstree<Key, Value>::stack_iterator_inorder::sentinel& sent, const typename bstree<Key, Value>::stack_iterator_inorder& iter)  noexcept
{
   return iter == sent;    
}    

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

   node_postOrderIterative(f, root);

   *this = copy_tree(lhs.root);

   return *this;
}


template<class Key, class Value> inline bstree<Key, Value>::bstree(std::initializer_list<value_type>& list)  noexcept : bstree()
{
   insert(list);
}

template<class Key, class Value> inline bstree<Key, Value>::bstree(const bstree<Key, Value>& lhs) noexcept : size{lhs.size}, root{nullptr}
{ 
   if (!lhs.root) return;
    
   *this = copy_tree(lhs.root);
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
  
  auto f = [] (std::unique_ptr<Node>& ptr) {
      ptr.reset();
  };
  
  node_postOrderIterative(f, root);
  
  *this = copy_tree(lhs.root);
 
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

template<class Key, class Value> template<typename Functor> void bstree<Key, Value>::inOrderTrace(Functor f, const std::unique_ptr<Node>& current, stack_tracer<Key>& tracer, int depth) const noexcept
{
   if (!current) {

      return;
   }

   tracer.push(current->key());

   inOrderTrace(f, current->left, tracer, depth + 1);
   
   tracer.print();
   
   f(current->__get_value()); 
    
   inOrderTrace(f, current->right, tracer, depth + 1);
   
   tracer.pop();
}

template<class Key, class Value> template<typename Functor> void bstree<Key, Value>::preOrderTrace(Functor f, const std::unique_ptr<Node>& current, stack_tracer<Key>& tracer, int depth) const noexcept
{
   if (!current) {

      return;
   }

   tracer.push(current->key());

   tracer.print();

   f(current->__get_value()); 

   preOrderTrace(f, current->left, tracer, depth + 1);
   
   tracer.print();
    
   preOrderTrace(f, current->right, tracer, depth + 1);
   
   tracer.pop();
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

/* 
  Traced version of method below
template<class Key, class Value>
template<typename Functor>
void bstree<Key, Value>::preOrderStackIterative(Functor f, const std::unique_ptr<Node>& lhs) const noexcept
{
    stack_tracer<Key> tracer; 

   if (!lhs) return;
  
    std::stack<const node_type *> stack; 
    stack.push(root.get()); 
  
    tracer.push(root->key());

    //
    //  Pop all items one by one, and do the following for every popped item:
    // 
    //   a) invoke f 
    //   b) push its right child 
    //   c) push its left child 
    //
    // Note: the right child is pushed first so that left is processed first 
     
    while (!stack.empty()) { 

        // Pop the top item from stack and print it 
        const node_type *node = stack.top(); 
        stack.pop(); 

        Key key = tracer.peek();

        std::cout << key << " is top of stack. Stack after pop() = ";
        tracer.pop();
        tracer.print();
       
        f(node->__get_value()); 

        // Push right and left non-null children of the popped node to stack 
        if (node->right) { 
            tracer.push(node->right->key());
            stack.push(node->right.get()); 
        }

        if (node->left) {
            tracer.push(node->left->key());
            stack.push(node->left.get()); 
        } 
        std::cout << "Stack after pushing children of " << key << " = ";
        tracer.print();
    } 
}
*/

template<class Key, class Value>
template<typename Functor>
void bstree<Key, Value>::preOrderStackIterative(Functor f, const std::unique_ptr<Node>& lhs) const noexcept
{

   if (!lhs) return;
  
    std::stack<const node_type *> stack; 
    stack.push(root.get()); 

    //
    //  Pop all items one by one, and do the following for every popped item:
    // 
    //   a) invoke f 
    //   b) push its right child 
    //   c) push its left child 
    //
    // Note: the right child is pushed first so that left is processed first 
     
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
 Successfully Tested
 */
template<class Key, class Value>
template<typename Functor>
void bstree<Key, Value>::preOrderIterative(Functor f, const std::unique_ptr<Node>& root_in) const noexcept
{

   if (!root_in) return;

   Node *__y = root_in.get();

   do {   
       
        f(__y->__get_value());  

        if (__y->left)          // Prefer left child
            __y = __y->left.get();
        else if (__y->right)       // otherwise, the right 
            __y = __y->right.get();
      
        else  { // else __y is a leaf

           // If leaf is a left child and it's parent has a right child, make that right child the pre-order successor.
           if (__y == __y->parent->left.get() && __y->parent->right)  { // TODO: Could parent be nullptr, say, if root_in is the only node in the input tree? 
               
                  __y = __y->parent->right.get();
             
           } else {// Leaf is a right child (or a left child whose parent does not have a right child).
                  // We must ascend the parent chain until we find a parent whose right child's key > prior->key()

             for(auto parent = __y->parent; 1; parent = parent->parent) {
        
                // When parent's key is > prior->key(), we are high enough in the parent chain to determine if the
                // parent's right child's key > prior->key(). If it is, this is the preorder successor for the leaf node prior. 

                // Note: we combine all three tests--right child of parent exits, parent key is > prior's,
                // and parent's right child's key > prior's--into one if-test. 
                if (parent->right && parent->key() > __y->key() && parent->right->key() > __y->key()) { 

                     __y = parent->right.get();
                     break; 
                } 
                if (parent == root_in.get()) {
                    __y = root_in.get(); // There is no pre-order successor because we ascended to the root,
                    break;              // and the root's right child is < prior->key().
                }
             } 
           } 
        }
    } while(__y != root_in.get()); 
}

template<class Key, class Value>
bstree<Key, Value> bstree<Key, Value>::copy_tree(const std::unique_ptr<Node>& root_in) const noexcept
{
   bstree<Key, Value> new_tree;
   
   if (!root_in) 
       return new_tree;
 
   Node *new_node_parent = nullptr; 
   
   Node *new_node = nullptr;
   
   Node *__y = root_in.get();
    
   do {   
       
        std::unique_ptr<Node> new_uptr = std::make_unique<Node>(__y->__vt);
        
        new_node = new_uptr.get();
 
        if (!__y->parent) {// __y was the root, so set parent of new_node to nullptr.
           
            new_tree.root = std::move(new_uptr);
            new_node_parent = new_tree.root.get();
 
        }  else if (new_node_parent->key() > new_uptr->key()) {
               
            new_node_parent->connectLeft(new_uptr); 
            new_node_parent = new_node_parent->left.get();
               
        } else {
               
            new_node_parent->connectRight(new_uptr); 
            new_node_parent = new_node_parent->right.get();
        }
        
        std::cout << new_tree << std::endl;
        
        if (__y->left)          // Prefer left child
            __y = __y->left.get();
        else if (__y->right)       // otherwise, the right 
            __y = __y->right.get();
      
        else  { // else __y is a leaf
 
           // If leaf is a left child and it's parent has a right child, make that right child the pre-order successor.
           if (__y == __y->parent->left.get() && __y->parent->right)  {
               
                  __y = __y->parent->right.get();
                  new_node_parent = new_node->parent;
                
           } else {// Leaf is a right child (or a left child whose parent does not have a right child).
                  // We must ascend the parent chain until we find a parent whose right child's key > prior->key()
             auto new_xxx = new_node->parent;  

             for(auto parent = __y->parent; 1; parent = parent->parent) {
        
                // When parent's key is > prior->key(), we are high enough in the parent chain to determine if the
                // parent's right child's key > prior->key(). If it is, this is the preorder successor for the leaf node prior. 
 
                // Note: we combine all three tests--right child of parent exits, parent key is > prior's,
                // and parent's right child's key > prior's--into one if-test. 
                if (parent->right && parent->key() > __y->key() && parent->right->key() > __y->key()) { 
 
                     __y = parent->right.get();

                     new_xxx = new_xxx->right.get(); //--
                     
                     new_node_parent = new_tree.find__(parent->key()); //TODO: Can I speed this up?   
                     break; 
                } 
                
                if (parent == root_in.get()) {
                    __y = root_in.get(); // There is no pre-order successor because we ascended to the root,
                    break;             // and the root's right child is < prior->key().
                }
                new_xxx = new_xxx->parent;   
             } 
           } 
        }
    } while(__y != root_in.get()); 
   
    return new_tree;
 }
 
/*
post order iterative implementations
 
1. post-order iterative pseudocode and discussions: <-------
 
  https://en.wikipedia.org/wiki/Tree_traversal
 
  Traversl implement in multiple prog. languages
  http://rosettacode.org/wiki/Tree_traversal

  Tree traversal without recursion: the tree as a state machine
  https://www.perlmonks.org/?node_id=600456

2. implementation using one stack:

  In C:

  https://www.geeksforgeeks.org/iterative-postorder-traversal-using-stack/

  In Python. Throroughly discussed:

  https://stackoverflow.com/questions/54635756/iterative-postorder-traversal-of-a-binary-tree-with-a-single-stack-how-to-appro       

  https://www.java67.com/2017/05/binary-tree-post-order-traversal-in-java-without-recursion.html

3. implementation using two stacks:

  In Java:

  https://www.geeksforgeeks.org/iterative-postorder-traversal/
  https://algorithmsandme.com/iterative-postorder-traversal/

  In C++:

  https://www.techiedelight.com/postorder-tree-traversal-iterative-recursive/
*/


/*
 * Pseudocode from https://en.wikipedia.org/wiki/Tree_traversal
 */
template<class Key, class Value>
template<typename Functor>
void bstree<Key, Value>::postOrderIterative(Functor f, const std::unique_ptr<Node>& root_in) const
{
  //TODO: Rewrite without stack.
  const Node *pnode = root_in.get();

  std::stack<const Node *> stack; 

  const Node *lastNodeVisited{nullptr};

  while (!stack.empty() || pnode) {

    if (pnode) {

      stack.push(pnode);
      pnode = pnode->left.get();

    } else {

      const Node *peekNode = stack.top();

      if (peekNode->right && lastNodeVisited != peekNode->right.get())

          pnode = peekNode->right.get();

      else {

        f(peekNode->__get_value());
            
        lastNodeVisited = stack.top();
        stack.pop();
 
        pnode = nullptr;
     }
   } 
 }
}

/*
This method visits the Node

From https://www.geeksforgeeks.org/postorder-successor-node-binary-tree/:

An efficient solution is based on below observations.

   If given node is root, then postorder successor is NULL, since the root is the last node print in a postorder traversal
   If given node is a right child of its parent, or the right child of parent is NULL, then parent is postorder successor.
   If given node is left child of parent and right child of parent is not NULL, then postorder successor is the leftmost node of parent’s right subtree
 */
template<class Key, class Value>
template<typename Functor>
void bstree<Key, Value>::node_postOrderIterative(Functor f, std::unique_ptr<Node>& root_in) noexcept
{
   if (!root_in) return;

   Node *__y = root_in.get();

   // Go to min node in tree
   while (__y->left)
       __y = __y->left.get();

  while (__y != nullptr) {

    std::unique_ptr<Node>& __ref = get_unique_ptr(__y);  
    
    /*
     * We must this if-test here before f(__ref) is invoke because f(__ref) might invoke __ref.reset(),
     * and then __y will no longer be a a valid pointer.
     */
    bool __y_was_root = __y == root_in.get(); 
    
    f(__ref);

    if (__y_was_root)  // done
       break;

   // If given node is a right child of its parent or the parent's right is empty (implying the node is the left child and only child of its parent),
   // then the parent is post-order successor. 
   auto parent = __y->parent; 

   if (!parent->right || __y == parent->right.get()) 
       __y = parent; 
   
   else {
   
      // In all other cases, find the left-most child in the right substree of parent. 
      auto pnode = parent->right.get(); 
   
      while (pnode->left)    // get min node in subtree. 
          pnode = pnode->left.get(); 

       __y = pnode;
   }          
 }
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

// TODO: What if not found? Is this even a concern?
template<class Key, class Value> std::unique_ptr<typename bstree<Key, Value>::Node>& bstree<Key, Value>::find(Key key, std::unique_ptr<Node>& current) const noexcept
{
  if (!current || current->key() == key)
     return current;
  if (key < current->key())
     return find(key, current->left);
  else return find(key, current->right);
}

template<class Key, class Value> typename bstree<Key, Value>::Node *bstree<Key, Value>::find__(Key key, const std::unique_ptr<typename bstree<Key, Value>::Node>& current) const noexcept
{
  if (!current) return nullptr; // not found

  if (current->key() == key)
     return current.get();
  if (key < current->key())
     return find__(key, current->left);
  else return find__(key, current->right);
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

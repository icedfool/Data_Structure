// ===================================================================
//
// We provide the Point class and the implementation of several
// QuadTree member functions for printing.  
//
// IMPORTANT: You should modify this file to add all of the necessary
// functionality for the QuadTree class and its helper classes: Node,
// DepthIterator, and BreadthIterator.
//
// ===================================================================

#ifndef quad_tree_h_
#define quad_tree_h_

#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>

template<class number_type, class label_type>
class QuadTree_Iterator;
// ==============================================================
// ==============================================================
// A tiny templated class to store 2D coordinates.  This class works
// with number_type = int, float, double, unsigned char (a 1 byte=8
// bit integer), short (a 2 byte=16 bit integer).

template <class number_type>
class Point {
public:
  Point(const number_type& x_, const number_type& y_) : x(x_),y(y_) {}
  // REPRESENTATION
  number_type x;
  number_type y;
};

// node class
template <class number_type, class label_type>
class Node {
public:
  Node(): parent(nullptr) {
    for(int i = 0; i< 4;i++ ){
      children[i] = NULL;
    }
  }

  Node(const Point<number_type>& p, const label_type l):pt(p), label(l),parent(nullptr){
    for(int i = 0; i< 4;i++ ){
      children[i] = NULL;
    }
  }
  // pointers
  Node<number_type,label_type>* children[4];
  Node<number_type,label_type>* parent;
  Point<number_type> pt;
  label_type label;
};

// Quad tree class
template<class number_type,class label_type>
class QuadTree{
public:
  friend class QuadTree_Iterator<number_type,label_type>;
  friend class Breadth_first_iterator<number_type,label_type>;
  QuadTree(): size_(0) {}
  ~QuadTree(){clear(root_);}
  typedef QuadTree_Iterator<number_type,label_type> iterator;
  typedef Breadth_first_iterator<number_type,label_type> bf_iterator;
  void plot(int max_x, int max_y, bool draw_lines = true) const;
  void plot(Node<number_type,label_type> *p, std::vector<std::string> &board,
          int x_min, int x_max, int y_min, int y_max, bool draw_lines = true) const;
  void print_sideways() const;
  //other function
  int size();
  int height();
  int Compare(const Node<number_type,label_type>* node, const Point<number_type>& pos);
  void insert(Point<number_type> p,label_type c);
  iterator find(number_type num_a,number_type num_b);
  iterator begin(){return iterator(root_);}
  iterator end(){return iterator(NULL);}
  bf_iterator bf_begin(){return bf_iterator(root_);}
  bf_iterator bf_end(){return bf_iterator(NULL);}
private:
  iterator find(number_type num_a,number_type num_b,Node<number_type,label_type>* node);
  void insert(Point<number_type> p,label_type c,Node<number_type,label_type>* node);
  void clear(Node<number_type,label_type>* &p);
  int get_height(Node<number_type,label_type>* node);
  void print_sideways(Node<number_type,label_type>* p, const std::string &indent) const;
  unsigned int size_;
  Node<number_type,label_type>* root_;
};

// depth-first iterator class
template<class number_type, class label_type>
class QuadTree_Iterator{
public:
  friend class QuadTree<number_type,label_type>;
  QuadTree_Iterator():ptr(NULL) {}
  QuadTree_Iterator(Node<number_type,label_type>* p) : ptr(p) {}
  QuadTree_Iterator(const QuadTree_Iterator& old) : ptr(old.ptr) {}
  ~tree_iterator() {}

  const Point<number_type>& operator*() const { return ptr->pt; }
  bool operator== (const QuadTree_Iterator& rgt) { return ptr == rgt.ptr; }
  bool operator!= (const QuadTree_Iterator& rgt) { return ptr != rgt.ptr; }

  label_type getLabel(){return ptr->label;}
  int getDepth(){
    int depth;
    if (this->ptr->parent!= NULL){
      this->ptr = ptr->parent;
      depth += 1;
    }
    return depth;
  }
  void data_process(Node<number_type,label_type>* n,
                    std::vector<Node<number_type,label_type>*>& sorted_element);
  QuadTree_Iterator<number_type,label_type> & operator++(){
    Node<number_type,label_type>* current = this->ptr;
    Node<number_type,label_type>* root;
    while (this->ptr->parent != NULL){
      ptr = ptr->parent;
    }
    root = this->ptr;
    data_process(root,sorted_element);
    Node* return_iterator = std::find(sorted_element.begin(),sorted_element.end(),current);
    return_iterator++;
    return return_iterator;
  }
  QuadTree_Iterator<number_type,label_type> & operator++(int){
    QuadTree_Iterator<number_type,label_type> temp(*this);
    ++(*this);
    return temp;
  }
private:
  Node<number_type,label_type>* ptr;
  std::vector<Node<number_type,label_type>*> sorted_element; 
};

template<class number_type, class label_type>
void QuadTree_Iterator<number_type,label_type>::data_process
(Node<number_type,label_type>* n,std::vector<Node<number_type,label_type>*>& sorted_element){
  if( n == NULL){
    return;
  }
  else{
    sorted_element.push_back(n);
    data_process(n->children[0]);
    data_process(n->children[1]);
    data_process(n->children[2]);
    data_process(n->children[3]);
  }
}

// breadth-first iterator class
template<class number_type, class label_type>
class Breadth_first_iterator{
public:
  friend class QuadTree<number_type,label_type>;
  Breadth_first_iterator():ptr(NULL) {}
  Breadth_first_iterator(Node<number_type,label_type>* p): ptr(p) {}
  Breadth_first_iterator(const Depth_first_iterator& old) : ptr(old.ptr) {}

  const Point<number_type>& operator*() const { return ptr->pt; }
  bool operator== (const Breadth_first_iterator& rgt) { return ptr == rgt.ptr; }
  bool operator!= (const Breadth_first_iterator& rgt) { return ptr != rgt.ptr; }

  Breadth_first_iterator<number_type,label_type> & operator++(){
    Node<number_type,label_type>* current = this->ptr;
    while (this->ptr->parent != NULL){
      ptr = ptr->parent;
    }
    Node<number_type,label_type>* root = ptr;
    if(current->pt.x == root->pt.x && current->pt.y == root->pt.y){
      element.push_back(current);
      return element[0];
    }
    else if (current->children[0] == NULL){
      element.pop_front();
      return element[0];
    }
    else{
      element.pop_front();
      for (int i = 0; i < 4; i++){
        element.push_back(current->children[i]);
      }
      return element[0];
    }
  }

  Breadth_first_iterator<number_type,label_type> & operator++(int){
    Breadth_first_iterator<number_type,label_type> temp(*this);
    ++(*this);
    return temp;
  }
  int getDepth(){
      int depth;
      if (this->ptr->parent!= NULL){
        this->ptr = ptr->parent;
        depth += 1;
      }
      return depth;
    }
  label_type getLabel(){return ptr->label;}
private:
  Node<number_type,label_type>* ptr;
  std::deque<Node<number_type,label_type>*> element;
};


// a helper function to print Points to STL output stream
template <class number_type>
std::ostream& operator<<(std::ostream &ostr, const Point<number_type> &pt) {
  ostr << "(" << pt.x << "," << pt.y << ")";
  return ostr;
}

// ==============================================================
// PROVIDED CODE : QUAD TREE MEMBER FUNCTIONS FOR PRINTING
// ==============================================================

// Adjust the function prototypes below as necessary to add them as 
// member functions of your QuadTree class.

// NOTE: the plot function only works for quad trees with non negative
// integer coordinates and char labels

// NOTE2: the plot function assumes that no two points have the same x
// coordinate or the same y coordinate.

// plot driver function
// takes in the maximum x and y coordinates for these data points
// the optional argument draw_lines defaults to true
template<class number_type,class label_type>
void QuadTree<number_type,label_type>::plot(int max_x, int max_y, bool draw_lines) const {
  // allocate blank space for the center of the board
  std::vector<std::string> board(max_y+1,std::string(max_x+1,' '));
  // edit the board to add the point labels and draw vertical and
  // horizontal subdivisions
  plot(root_,board,0,max_x,0,max_y,draw_lines);
  // print the top border of the plot
  std::cout << "+" << std::string(max_x+1,'-') << "+" << std::endl;
  for (int i = 0; i <= max_y; i++) {
    // print each row of the board between vertical border bars
    std::cout << "|" << board[i] << "|" << std::endl;
  }
  // print the top border of the plot
  std::cout << "+" << std::string(max_x+1,'-') << "+" << std::endl;
}

// actual recursive function for plotting
template<class number_type,class label_type>
void QuadTree<number_type,label_type>::plot(Node<number_type,label_type> *p, 
                std::vector<std::string> &board,int x_min, int x_max, int y_min, int y_max, bool draw_lines) const {
  // base case, draw nothing if this node is NULL
  if (p == NULL) return;
  // check that the dimensions range of this node make sense
  assert (x_min >= 0 && x_min <= x_max);
  assert (y_min >= 0 && y_min <= y_max);
  assert (int(board.size()) >= y_max);
  assert (int(board[0].size()) >= x_max);
  // verify that the point stored at this node fits on the board
  assert (p->pt.y >= 0 && p->pt.y < int(board.size()));
  assert (p->pt.x >= 0 && p->pt.x < int(board[0].size()));
  // draw the vertical and horizontal bars extending across the
  // range of this node
  if (draw_lines) {
    for (int x = x_min; x <= x_max; x++) {
      board[p->pt.y][x] = '-';
    }
    for (int y = y_min; y <= y_max; y++) {
      board[y][p->pt.x] = '|';
    }
  }
  // draw this label
  board[p->pt.y][p->pt.x] = p->label;
  // recurse on the 4 children
  plot(p->children[0],board,x_min ,p->pt.x-1,y_min ,p->pt.y-1,draw_lines);
  plot(p->children[1],board,p->pt.x+1,x_max ,y_min ,p->pt.y-1,draw_lines);
  plot(p->children[2],board,x_min ,p->pt.x-1,p->pt.y+1,y_max ,draw_lines);
  plot(p->children[3],board,p->pt.x+1,x_max ,p->pt.y+1,y_max ,draw_lines);
}


// ==============================================================

// prints all of the tree data with a pre-order (node first, then
// children) traversal of the tree structure

// driver function
template<class number_type,class label_type>
void QuadTree<number_type,label_type>::print_sideways()
     const { print_sideways(root_,""); }

// actual recursive function
template<class number_type,class label_type>
void QuadTree<number_type,label_type>::print_sideways
        (Node<number_type,label_type>* p, const std::string &indent) const {
  // base case
  if (p == NULL) return;
  // print out this node
  std::cout << indent << p->label << " (" << p->pt.x << "," << p->pt.y << ")" << std::endl;
  // recurse on each of the children trees
  // increasing the indentation
  print_sideways(p->children[0],indent+"  ");
  print_sideways(p->children[1],indent+"  ");
  print_sideways(p->children[2],indent+"  ");
  print_sideways(p->children[3],indent+"  ");
}

// ==============================================================
// ==============================================================

// a helper function to compare with the x,y of current node to the postion
template<class number_type,class label_type>
int QuadTree<number_type,label_type>::Compare(const Node<number_type,label_type>* node, const Point<number_type>& pos){
	if (pos.x <node->pt.x && pos.y< node->pt.y)  return 0;
	if (pos.x>node->pt.x && pos.y <= node->pt.y) return 1;
	if (pos.x<node->pt.x  && pos.y > node->pt.y) return 2;
	if (pos.x > node->pt.x && pos.y>node->pt.y)  return 3;
	return -1;
}


//=====
template<class number_type,class label_type>
void QuadTree<number_type,label_type>::clear(Node<number_type,label_type>* &p){
	if (p == NULL) return;
	if (p->children[0]) clear(p->children[0]);
	if (p->children[1]) clear(p->children[1]);
	if (p->children[2]) clear(p->children[2]);
	if (p->children[3]) clear(p->children[3]);
	delete p;
	p = NULL;
}

template<class number_type,class label_type>
void QuadTree<number_type,label_type>::insert(Point<number_type> p,label_type c){
  insert(p,c,root_);
}

template<class number_type,class label_type>
void QuadTree<number_type,label_type>::insert(Point<number_type> p,label_type c,Node<number_type,label_type>* ptr_node){
  if(ptr_node == NULL){
    ptr_node = new Node<number_type,label_type>(p,c);
    this->size_++;
  }
  else{
    int situation_num = Compare(ptr_node,p);
    if(situation_num == 0){
      insert(p,c,ptr_node->children[0]);
      ptr_node->children[0]->parent = ptr_node;
    }
    else if(situation_num == 1){
      insert(p,c,ptr_node->children[1]);
      ptr_node->children[1]->parent = ptr_node;
    }
    else if(situation_num == 2){
      insert(p,c,ptr_node->children[2]);
      ptr_node->children[2]->parent = ptr_node;
    }
    else if(situation_num == 3){
      insert(p,c,ptr_node->children[3]);
      ptr_node->children[3]->parent = ptr_node;
    }
  }
}

template<class number_type,class label_type>
typename QuadTree<number_type,label_type>::iterator 
QuadTree<number_type,label_type>::find(number_type a, number_type b){
  return find(a,b,root_);
}

template<class number_type,class label_type>
typename QuadTree<number_type,label_type>::iterator 
QuadTree<number_type,label_type>::find(number_type a, number_type b,Node<number_type,label_type>* p){
  if(!p){return end();}
  if(a == p->pt.x && b == p->pt.y){return iterator* p;}
  else{
    int situation_num = Compare(p,Point<number_type>(a,b));
    if (situation_num == 0){
      find(a,b,p->children[0]);
    }
    else if (situation_num == 1){
      find(a,b,p->children[1]);
    }
    else if (situation_num == 2){
      find(a,b,p->children[2]);
    }
    else if (situation_num == 3){
      find(a,b,p->children[3]);
    }
  }
}


template<class number_type,class label_type>
int QuadTree<number_type,label_type>::size(){
  return this->size_;
}

template<class number_type,class label_type>
int QuadTree<number_type,label_type>::height(){
  if (this->root_ == NULL){
    return -1;
  }
  else{
    return get_height(root_);
  }
}

template<class number_type,class label_type>
int QuadTree<number_type,label_type>::get_height(Node<number_type,label_type>* node){
  if(node == NULL){
    return 0;
  }
  int max_0 = get_height(node->children[0]);
  int max_1 = get_height(node->children[1]);
  int max_2 = get_height(node->children[2]);
  int max_3 = get_height(node->children[3]);
  int total_max = std::max({max_0,max_1,max_2,max_3});
  return total_max + 1;
}


#endif

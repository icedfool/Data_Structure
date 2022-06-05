#ifndef _GRID_H_
#define _GRID_H_
#include<iostream>
#include<iomanip>


template <class T> class Grid;
//Node class
template <class T>
class Node{
public:
    // value and pointers
    T value_;
    Node<T>* left_;
    Node<T>* right_;
    Node<T>* up_;
    Node<T>* down_;
    //default constructor
    Node(int value) : value_(value), left_(nullptr),right_(nullptr), up_(nullptr),\
    down_(nullptr) {}
    Node() : value_(0), left_(nullptr),right_(nullptr), up_(nullptr),\
    down_(nullptr) {}
};

//iterator class
template <class T> 
class GridIterator {
private:
    // REPRESENTATION
    Node<T>* ptr_; // ptr to node in the grid
    bool goLeft = false;
    bool goRight = false;
    std::string mode;
public:
    // the grid class needs access to the private ptr_ member variable
    friend class Grid<T>;
    GridIterator(Node<T>* p= NULL):ptr_(p) {}

    T& operator*() {return ptr_->value_; }
    GridIterator<T>& left(){ptr_ = ptr_->left_;return *this;}
    GridIterator<T>& right(){ptr_ = ptr_->right_;return *this;}
    GridIterator<T>& up(){ptr_ = ptr_->up_;return *this;}
    GridIterator<T>& down(){ptr_ = ptr_->down_;return *this;}
    
    GridIterator<T> operator++() { // pre-increment, e.g., ++iter
        if (this->mode == "snake"){
            if(this->goLeft == true){
                if(this->ptr_->left_ == nullptr){
                    this->ptr_ = this->ptr_->down_;
                    this->goLeft = false;
                    this->goRight = true;
                    return *this;
                }
                this->ptr_ = this->ptr_->left_;
                return *this;
            }
            else if (this->goRight == true){
                if (this->ptr_->right_ == nullptr){
                    this->ptr_ = this->ptr_->down_;
                    this->goRight = false;
                    this->goLeft = true;
                    return *this;
                }
                this->ptr_ = this->ptr_->right_;
                return *this;
            }
        }
        else if(this->mode == "spiral"){
            return *this;
        }
        else{
            std::cerr<<"Now having trouble with iterators. Please check or response to the \
            author."<<std::endl;
            exit(0);
        }
    }

    GridIterator<T> operator++(int) { // post-increment, e.g., iter++
        GridIterator<T> temp(*this);
        if (this->mode == "snake"){
            if(this->goLeft == true){
                if(this->ptr_->left_ == nullptr){
                    this->ptr_ = this->ptr_->down_;
                    this->goLeft = false;
                    this->goRight = true;
                    return temp;
                }
                this->ptr_ = this->ptr_->left_;
                return temp;
            }
            else if (this->goRight == true){
                if (this->ptr_->right_ == nullptr){
                    this->ptr_ = this->ptr_->down_;
                    this->goRight = false;
                    this->goLeft = true;
                    return temp;
                }
                this->ptr_ = this->ptr_->right_;
                return temp;
            }
        }
        else if(this->mode == "spiral"){
            return temp;
        }
        else{
            std::cerr<<"Now having trouble with iterators. Please check or response to the \
            author."<<std::endl;
            exit(0);
        }
    }

    bool operator!=(const GridIterator<T>& r) const {
        return ptr_ != r.ptr_; 
    }

    bool operator==(const GridIterator<T>& r) const {
        return ptr_ == r.ptr_;
    }
};

//Grid class
template <class T>
class Grid{
private:
    //helper function
    void destroy_grid();
    // member values
    int m_width;
    int m_height;
    int m_size;
    // four pointers points to the nodes after this.
    Node<T>* upper_left_;
    Node<T>* upper_right_;
    Node<T>* lower_left_;
    Node<T>* lower_right_;
public:
    // default constructor, copy constructor and destructor
    Grid(): m_height(0), m_size(0), m_width(0),upper_left_(nullptr),\
    upper_right_(nullptr),lower_left_(nullptr),lower_right_(nullptr) {}
    Grid(int width,int height, T value = T());
    ~Grid(){ destroy_grid(); }

    //iterator
    typedef GridIterator<T> iterator;
    
    // accessor
    int getWidth() const{return m_width;}
    int getHeight() const{return m_height;}
    int getSize()const{return m_size;}

    // modifier
    void set(int row,int column, T value);
    void setWidth(int width){m_width = width;}
    void setHeight(int height){m_height = height;}
    void setSize(int size){m_size = size;}

    iterator begin_upper_left(){return iterator(upper_left_);}
    iterator begin_upper_right(){return iterator(upper_right_);}
    iterator begin_lower_left(){return iterator(lower_left_);}
    iterator begin_lower_right(){return iterator(lower_right_);}
    iterator begin_snake(){
        iterator ret(upper_left_);
        ret.mode = "snake";
        ret.goRight = true;
        return ret;
    }
    iterator end_snake(){
        iterator ret(NULL);
        ret.mode = "sanke";
        return ret;
    }
    iterator begin_spiral(){
        iterator ret(upper_left_);
        ret.mode = "spiral";
        return ret;
    }
    iterator end_spiral(){
        iterator ret(NULL);
        ret.mode = "spiral";
        return ret;
    }
    iterator end(){
        iterator ret(NULL);
        return ret;
    }
    // other function
    void print();
    T get(int column,int row);
    void join(Grid& newGrid);
    void stack(Grid& newGrid);
    void reset(T value);
    void clear(){destroy_grid();}
    void lift(iterator itr, Grid<T>& newGrid);
    void chop(iterator itr, Grid<T>& newGrid);
};

// template <class T> class Grid;


template <class T>
Grid<T>::Grid(int width,int height,T value){
    m_width = width;
    m_height = height;
    m_size = m_width*m_height;
    upper_left_ = new Node<T>(value);
    //create a moving node to track current column
    Node<T>* columnMarker = upper_left_;
    //create a moving node to track current column
    Node<T>* rowMarker = upper_left_;

    // this for loop can create the first row of the grid
    // first create a new node, then linked back to the previous 
    // node, and then move the columnMarker.
    for(int i = 1; i < width; i++){
        columnMarker->right_ = new Node<T>(value);
        columnMarker->right_->left_ = columnMarker;
        columnMarker = columnMarker->right_;
        // if the marker point at the last node of first row,
        // link this node to upper_right_ pointer.
        if(i == width-1){
            upper_right_ = columnMarker;
        } 
    }
    for(int i = 0; i < height-1; i++){
        // create a new node in next row, linked back, change the 
        // current rowMarker and columnMarker.
        rowMarker->down_ = new Node<T>(value);
        rowMarker->down_->up_ = rowMarker;
        rowMarker = rowMarker->down_;
        columnMarker = rowMarker;
        // if the current i = (height-2),then this is the last row,
        // since row marker is always the first node of the row,
        // then mark this node as lower_left_ .
        if(i == height-2){
            lower_left_ = rowMarker;
        }
        //creating the rest of the row
        for(int j = 0; j < width -1; j++){
            columnMarker->right_ = new Node<T>(value);
            columnMarker->right_->left_ = columnMarker;
            columnMarker->right_->up_ = columnMarker->up_->right_;
            columnMarker->right_->up_->down_ = columnMarker->right_;
            columnMarker = columnMarker->right_;
            // if the current j = (width-2) and i = (height-2) ,
            // then this is the last node of the grid, mark this 
            // node as the lower_right_ .
            if(j == width-2 && i == height - 2){
                lower_right_ = columnMarker;
            }
        }
    }
    //two special cases of the grid if the height/width is 1
    if(m_width == 1){
        upper_right_ = upper_left_;
        lower_right_ = lower_left_;
    }
    if (m_height == 1){
        lower_left_ = upper_left_;
        lower_right_ = upper_right_;
    }
}

// a helper function call be called by destructor
// and the clear function. the purpose of this function
// is to delete every node in the linked grid.
template <class T>
void Grid<T>::destroy_grid(){
    Node<T>* column = upper_left_;
    Node<T>* row = upper_left_;
    for (int i = 0; i < this->getHeight(); i++){
        row = row->down_;
        for (int j = 0; j < this->getWidth(); j++){
            if(j == this->getWidth()-1){
                delete column;
                column = row;
                continue;
            }
            column = column->right_;
            delete column->left_;
        }
    }
    // after deleted all the nodes remember to 
    // set all member varibles to 0.
    this->setHeight(0);
    this->setWidth(0);
    this->setSize(0);
}

template <class T>
void Grid<T>::print(){
    //create a temp node to point the first node of the grid
    Node<T>* temp = upper_left_;
    //create a rowMarker node to point the first node of every line
    Node<T>* rowMarker = upper_left_;
    //while current node has sth.,print that and move to next node
    while (rowMarker != nullptr){
        while (temp != nullptr){
            std::cout<<" "<<std::setw(3)<<temp->value_;
            temp = temp->right_;        
        }
        std::cout<<std::endl;
        temp = rowMarker->down_;
        rowMarker = temp;
    }
}

// take in the column and the row of the postion,
// and set the value of the current node.
template <class T>
void Grid<T>::set(int column,int row, T value){
    Node<T>* rowMarker = upper_left_;
    Node<T>* columnMarker = upper_left_;
    // loop till it get to the right column
    for(int i = 0; i <column; i++ ){
        columnMarker = columnMarker->right_;
    }
    rowMarker = columnMarker;
    // loop till it get to the right row
    for (int i = 0; i < row; i++){
        rowMarker = rowMarker->down_;
    }
    //set value
    rowMarker->value_ = value;
}

// take in the column and the row of the postion,
// and return the value of the current node.
// the logic is similar to the set function.
template <class T>
T Grid<T>::get(int column,int row){
    Node<T>* rowMarker = upper_left_;
    Node<T>* columnMarker = upper_left_;
    for(int i = 0; i <column; i++ ){
        columnMarker = columnMarker->right_;
    }
    rowMarker = columnMarker;
    for (int i = 0; i < row; i++){
        rowMarker = rowMarker->down_;
    }
    return rowMarker->value_;
}

// a function that is used to join two grid together.
// the new grid will be joined to the right side of the 
// former one.
template <class T>
void Grid<T>::join(Grid<T>& newGrid){
    // two help pointers
    Node<T>* rowMarker = this->upper_right_;
    Node<T>* columnMarker = newGrid.upper_left_;
    // since the old grid is linked, we only need to
    // link the last row of the new grid and the 
    // first row of the old grid. 
    for (int i = 0; i < newGrid.getHeight(); i++){
        columnMarker->left_ = rowMarker;
        rowMarker->right_ = columnMarker;
        rowMarker = rowMarker->down_;
        columnMarker = columnMarker->down_;
    }
    // change the grid mark pointers and the member varibles 
    // of the grid
    this->upper_right_ = newGrid.upper_right_;
    this->lower_right_ = newGrid.lower_right_;
    m_size += newGrid.getSize();
    m_width += newGrid.getWidth();
    //change the content of the new Grid to 0
    newGrid.upper_left_ = nullptr;
    newGrid.upper_right_ = nullptr;
    newGrid.lower_left_ = nullptr;
    newGrid.lower_right_ = nullptr;
    newGrid.setSize(0);
    newGrid.setHeight(0);
    newGrid.setWidth(0);
}

// a function that is used to join two grid together.
// the new grid will be joined to the up side of the 
// former one.
template <class T>
void Grid<T>::stack(Grid<T>& newGrid){
    // two help pointers
    Node<T>* columnMarker = newGrid.lower_left_;
    Node<T>* rowMarker = this->upper_left_;
    // since the old grid is linked, we only need to
    // link the last row of the new grid and the 
    // first row of the old grid. 
    for (int i = 0; i < newGrid.getWidth(); i++){
        columnMarker->down_ = rowMarker;
        rowMarker->up_ = columnMarker;
        columnMarker = columnMarker->right_;
        rowMarker = rowMarker->right_;
    }
    // change the grid mark pointers and the member varibles 
    // of the grid.
    this->upper_left_ = newGrid.upper_left_;
    this->upper_right_ = newGrid.upper_right_;
    m_height += newGrid.getHeight();
    m_size += newGrid.getSize();
    //change the content of the new Grid to 0
    newGrid.upper_left_ = nullptr;
    newGrid.upper_right_ = nullptr;
    newGrid.lower_left_ = nullptr;
    newGrid.lower_right_ = nullptr;
    newGrid.setSize(0);
    newGrid.setHeight(0);
    newGrid.setWidth(0);
}

// a function that is used to reset values of the grid.
template<class T>
void Grid<T>::reset(T value){
    Node<T>* rowMarker = upper_left_;
    Node<T>* columnMarker = upper_left_;
    for(int i = 0; i < this->getHeight(); i++){
        for(int j = 0; j < this->getWidth(); j++){
            columnMarker->value_ = value;
            columnMarker = columnMarker->right_;
        }
        rowMarker = rowMarker->down_;
        columnMarker = rowMarker;
    }
}

template<class T>
void Grid<T>::lift(iterator itr, Grid<T>& newGrid){
    // create two counter to count the width and height 
    // of the new node
    int counterHeight = 0;
    // create a rowMarker to reach the boundary 
    Node<T>* rowMarker = itr.ptr_;
    while (rowMarker->left_ != nullptr){
        rowMarker = rowMarker->left_;
    }
    // make a copy of the rowMarker and then use this 
    // copy to find four new boundary
    Node<T>* nodeMarker = rowMarker;
    nodeMarker = nodeMarker->up_;
    this->upper_left_ = rowMarker;
    newGrid.lower_left_ = nodeMarker;

    // find the lower_right of the newGrid and use 
    // two more step to set the upper_right_ for this 
    // grid and unlink nodes
    while (nodeMarker->right_ != nullptr){
        nodeMarker->down_->up_ = nullptr;
        nodeMarker->down_ = nullptr;
        nodeMarker = nodeMarker->right_;
    }
    newGrid.lower_right_ = nodeMarker;
    nodeMarker = nodeMarker->down_;
    this->upper_right_ = nodeMarker;
    nodeMarker = nodeMarker->up_;
    // unlink current two nodes
    nodeMarker->down_->up_ = nullptr;
    nodeMarker->down_ = nullptr;
    //find the upper_right of the newGrid
    while (nodeMarker->up_ != nullptr){
        nodeMarker = nodeMarker->up_;
        counterHeight += 1;
    }
    //the counting will miss one line, so add back here
    counterHeight += 1;
    newGrid.upper_right_ = nodeMarker;
    //find the upper_left_ of the newGrid
    while (nodeMarker->left_ != nullptr){
        nodeMarker = nodeMarker->left_;
    }
    newGrid.upper_left_ = nodeMarker;
    //set member varibles for newGrid 
    newGrid.m_height = counterHeight;
    newGrid.m_width = this->m_width;
    newGrid.m_size = newGrid.m_width*newGrid.m_height;
    //set new member varibles for current grid
    this->m_height = m_height-counterHeight;
    this->m_size = m_width*m_height;
}

template<class T>
void Grid<T>::chop(iterator itr, Grid<T>& newGrid){
    // create two counter to count the width and height 
    // of the new node
    int counterWidth = 0;
    // create a columnMarker to reach the boundary 
    Node<T>* columnMarker = itr.ptr_;
    while (columnMarker->up_ != nullptr){
        columnMarker = columnMarker->up_;
    }
    // make a copy of the columnMarker and then use this 
    // copy to find four new boundary
    Node<T>* nodeMarker = columnMarker;
    nodeMarker = nodeMarker->left_;
    newGrid.upper_left_ = columnMarker;
    this->upper_right_ = nodeMarker;
    // find the lower_right of the newGrid and use 
    // two more step to set the lower_left_ for this 
    // grid, and unlink nodes
    while (nodeMarker->down_ != nullptr){
        nodeMarker->right_->left_ = nullptr;
        nodeMarker->right_ = nullptr;
        nodeMarker = nodeMarker->down_;
    }
    this->lower_right_ = nodeMarker;
    nodeMarker = nodeMarker->right_;
    newGrid.lower_left_ = nodeMarker;
    nodeMarker = nodeMarker->left_;
    //unlink current two nodes
    nodeMarker->right_->left_ = nullptr;
    nodeMarker->right_ = nullptr;
    //find the lower_left_ of the newGrid
    while (nodeMarker->left_ != nullptr){
        nodeMarker = nodeMarker->left_;
        counterWidth += 1;
    }//the counting will miss one line, so add back here
    counterWidth += 1;
    this->lower_left_ = nodeMarker;
    //find the upper_left_ of the newGrid
    while (nodeMarker->up_ != nullptr){
        nodeMarker = nodeMarker->up_;
    }
    this->upper_left_ = nodeMarker;
    // find the upper_right_ and lower_right of the new grid
    // first find upper_right
    Node<T>* nodeMarker2 = columnMarker;
    while(nodeMarker2->right_ != nullptr){
        nodeMarker2 = nodeMarker2->right_;
    }
    newGrid.upper_right_ = nodeMarker2;
    //then find lower_right_
    while(nodeMarker2->down_ != nullptr){
        nodeMarker2 = nodeMarker2->down_;
    }
    newGrid.lower_right_ = nodeMarker2;
    //set member varibles for newGrid 
    newGrid.m_width = this->m_width - counterWidth;
    newGrid.m_height = this->m_height;
    newGrid.m_size = newGrid.m_width*newGrid.m_height;
    //set new member varibles for current grid
    this->m_height = this->m_height;
    this->m_width = counterWidth;
    this->m_size = this->m_width*this->m_height;
}
#endif
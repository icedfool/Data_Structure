#include "tetris.h"
#include <iostream>

//constructor that take in a int type of width,
// and set various variables by this w.
Tetris::Tetris(int w){
    width = w;
    data = new char*[width];
    heights = new int[width];
    for (int i = 0; i < width; i++){
        heights[i] = 0;
    }
    for (int i = 0; i < width; i++){
        data[i] = nullptr;
    }
    square = 0;
}

//copy constrcutor ,first copy the value of width,
//then copy the heights and data
//=========================================================================
Tetris::Tetris(const Tetris& T1){
    this->width = T1.width;
    this->data = new char*[this->width];
    this->heights = new int[this->width];
    this->square = T1.square;
    //copy the heights data from T1;
    for (int i = 0; i < width; i++){
        heights[i] = T1.heights[i];
    }
    //copy the data array from T1;
    for (int i = 0; i < width; i++){
        char* temp_data_column = new char[heights[i]];
        for (int j = 0; j < heights[i]; j++){
            temp_data_column[j] =  T1.data[i][j];
        }
        this->data[i] = temp_data_column;
    }
}

//operator override of =
Tetris& Tetris::operator=(const Tetris& T1){
    if(this != &T1){
        for(int i =0; i< width; i++){
            delete[] data[i];
        }
        delete[] heights;
        delete[] data;
        this->width = T1.width;
        this->data = new char*[width];
        this->heights = new int[width];
        this->square = T1.square;
        //copy the heights data from T1;
        for (int i = 0; i < width; i++){
            heights[i] = T1.heights[i];
        }
        //copy the data array from T1;
        for (int i = 0; i < width; i++){
            char* temp_data_column = new char[heights[i]];
            for (int j = 0; j < heights[i]; j++){
                temp_data_column[j] =  T1.data[i][j];
            }
            this->data[i] = temp_data_column;
        }
    }
    return *this;
}
//==========================================================================
//this function is to add bricks to the data, it requires the char type of brick type,
//a int type of degree it turned, and a int type of begin postion the brick gonna place.
void Tetris::add_piece(char type,int degree,int begin_position){
    square += 4;
    int height = 0;
    if (type == 'O'){
        //when the position is valid
        if (begin_position < (width -1)){
            //get the higher hight
            if (heights[begin_position] > heights[begin_position+1]){
                height = heights[begin_position];
            }
            else{
                height = heights[begin_position + 1];
            }

            //use a for loop to add data
            for (int i = 0; i < 2; i++){
                char*temp = new char[height+2];
                //set all the char of this new array as none in the case when 
                //new char are placed
                for (int k = 0; k < height+2; k++){
                    temp[k] = ' ';
                }
                //copy stuff
                for (int j = 0; j < heights[begin_position+i]; j++){
                    if(data[begin_position+i]!= nullptr){
                        temp[j] = data[begin_position+i][j];//segmentation fault line
                    }
                }
                //add new data
                temp[height] = 'O';
                temp[height+1] = 'O';
                //delete old one and linked to new one
                delete[] data[begin_position+i];
                data[begin_position+i] = temp;
            }

            //change the heigts of the graph
            heights[begin_position] = height+2;
            heights[begin_position+1] = height+2;
            
        }
        else{
            std::cerr<<"Invaild postion, please check."<<std::endl;
            exit(0);
        }
        
    }
    else if (type == 'I'){
        //first detect if the shape is 'I' or '----'
        if (degree == 0 || degree == 180){
            //detect begin postion is vaild
            if(begin_position > width){
                std::cerr<<"Invaild postion, please check."<<std::endl;
                exit(0);
            }
            //vaild postion, start processing
            else{
                //since we have checked the begin positon which is vaild,
                // then we just need to add four 'I' at this line.
                height = heights[begin_position];

                //add data
                //create a temp array
                char*temp = new char[height+4];
                //set all the character in this new array as none char just in case
                for (int k = 0; k < height+4; k++){
                    temp[k] = ' ';
                }
                //copy stuff
                for (int l = 0; l < height; l++){
                    if (data[begin_position] != nullptr){
                        temp[l] = data[begin_position][l];
                    }
                }
                //add new content and remove the old array, then linked the new array
                temp[height] = 'I';
                temp[height+1] = 'I';
                temp[height+2] = 'I';
                temp[height+3] = 'I';
                delete[] data[begin_position];
                data[begin_position] = temp;
                // change height
                heights[begin_position] = height+4;
            }
        }
        else if (degree == 90 || degree == 270){
            //detect if begin postion is vaild
            if(begin_position > (width-4)){
                std::cerr<<"Invaild postion, please check."<<std::endl;
                exit(0);
            }
            else{
                // get the max height for this four lines
                height = heights[begin_position];
                for (int i = 1; i < 4; i++){
                    if (heights[begin_position+i] > height){
                        height = heights[begin_position+i];
                    }
                    else{
                        continue;
                    }
                }
                // add data
                for(int i = 0; i< 4; i++){
                    //change size
                    char*temp = new char[height+1];
                    //set all the char of this new array as none in the case when 
                    //new char are placed
                    for (int k = 0; k < height+1; k++){
                        temp[k] = ' ';
                    }
                    //copy stuff
                    for (int j = 0; j < heights[begin_position+i]; j++){
                        if(data[begin_position+i]!= NULL){
                            temp[j] = data[begin_position+i][j];
                        }
                    }
                    temp[height] = 'I';
                    delete[] data[begin_position+i];
                    data[begin_position+i] = temp;
                }
                //change the heigts of the graph
                heights[begin_position] = height+1;
                heights[begin_position+1] = height+1;
                heights[begin_position+2] = height+1;
                heights[begin_position+3] = height+1;
            }
        }
        else{
            std::cerr<<"Invaild input degree, please check."<<std::endl;
            exit(0); 
        }
        
    }
    else if (type == 'T'){
        //pass till I have more time
    }
    else if (type == 'Z'){
        //pass till I have more time
    }
    else if (type == 'S'){
        //pass till I have more time
    }
    else if (type == 'L'){
        //pass till I have more time
    }
    else if (type == 'J'){
        //pass till I have more time
    }
    else{
        std::cerr<<"Can't recognize character, please enter a new one. "<<std::endl;
        exit(0);
    }
}

//get the biggest value of heights
int Tetris::get_max_height() const{
    int height = heights[0];
    for(int i = 1; i< width; i++){
        if(heights[i]> height){
            height = heights[i];
        }
    } 
    return height;
}

Tetris::~Tetris(){
    delete[] heights;
    for (int i = 0; i < width; i++){
        delete[] data[i];
    }
    delete[] data;
}

//clear the content in the tetris class
void Tetris::clear(){
    for (int i = 0; i < width; i++){
        delete[] data[i];
        data[i] = new char[0];
        heights[i] = 0;
    }
    square = 0;
};

//use to remove a line that is full. take no arguments.
int Tetris::remove_full_rows(){
    int height_min = heights[0];//the time that the loop going to be looped
    int temp_counter = 0;//temp vatible to detect if the row is full
    int lines_removed = 0;//record the number of line that is removed 
    int temp_counter_detect = 0;//temp detector for blank array
    // get minimum height in heights, and then we just need to go over the 
    // line that is beneth this height.
    for (int i = 1; i < width; i++){
        if (height_min > heights[i]){
            height_min = heights[i];
        }
    }
    // here we can have the min_height of heights,which can help reduce the times of
    // looping 

    //use a for loop to find the row which is full
    for (int i = height_min-1; i >= 0; i--){
        for (int j = 0; j < width; j++){
            if (data[j][i] != ' '){
                temp_counter += 1;
            }
        }
        if(temp_counter == width){
            // as long as we find the line that is full, start to create a new array,
            // and do the copy, delete char, delete old array and linked to new one 
            // stuff.
            
            // i  IS　THE CURRENT INDEX OF THE LINE THAT NEED TO BE DELETED
            // k  IS  THE CURRENT COLLUM

            //go over all the rows.
            lines_removed += 1;
            for (int k = 0; k < width; k++){
                // initialize a temp array with none characters.
                char* temp = new char[heights[k]-1];
                for (int l = 0; l < heights[k]-1; l++){
                    temp[l] = ' ';
                }

                //copy stuff, and ignore the mark index i.
                for (int m = 0; m < i; m++){
                    temp[m] = data[k][m];
                }

                //if there s no more charcter on the head, do delete old,link and detect stuff
                if(i >= heights[k]-1){
                    delete[] data[k];
                    data[k] = temp;
                    heights[k] -= 1;
                    continue;
                }
                else{
                    for (int n = i+1; n < heights[k]; n++){
                        temp[n-1] = data[k][n];
                    }
                }
                //delete the old one and linked new one to data.
                delete[] data[k];
                data[k] = temp;
                heights[k] -= 1;
            }
        }
        temp_counter = 0;
    }
    //this is to detect if the row is blank. if it is, then
    //delete the data it has,link a nullptr to the data, and 
    //set height of this line to 0.
    for(int t = 0; t<width; t++){
        temp_counter_detect = 0;
        for(int u = 0; u < heights[t]; u++){
            if(data[t][u] == ' '){
                temp_counter_detect += 1;
            }
        }
        if(temp_counter_detect == heights[t]){
            delete[] data[t];
            data[t] = new char[0];
            heights[t] = 0;
        }
        temp_counter_detect = 0;
    }
    square -= (lines_removed*width);
    return lines_removed;
} 

// add a blank column to the left of the graph.take no arguments.
void Tetris::add_left_column(){
    //create new arrays,both for height and data, and both are
    //1 size bigger than the former one.
    char** temp_data = new char*[width+1];
    int* temp_height = new int[width+1];
    //then do the copy stuff. and remember for this situation,
    //THE INDEX SHOULD BE !!! ADDING 1 !!!, BUT LOOP THE SAME TIMES.
    //copy height data
    for (int i = 1; i < width+1; i++){
        temp_height[i] = heights[i-1];
    }
    //copy data array, BE CAREFUL WITH THE INDEX.
    for (int i = 1; i < width+1; i++){
        char* temp_data_column = new char[heights[i-1]];
        for (int j = 0; j < heights[i-1]; j++){
            temp_data_column[j] = data[i-1][j];
        }
        temp_data[i] = temp_data_column;
    }
    //delete the old one and link the new one
    delete[] heights;
    for (int i = 0; i < width; i++){
        delete[] data[i];
    }
    delete[] data;
    data = temp_data;
    heights = temp_height;
    //set default value for heights[0] and data[0]
    heights[0] = 0;
    data[0] = new char[0];
    //dont forget width should +1
    width += 1;
}

//remove the colum at the left side of the tetris game. take no arguments.
void Tetris::remove_left_column(){
    //record the squares in the row that is removed
    int square_counter = 0;//temp counter to count the number of
    // squares that were removed
    for (int i = 0; i < heights[0]; i++){
        if(data[0][i] != ' '){
            square_counter += 1;
        }
    }
    square -= square_counter;
    //fisrt delete the value of the left column
    heights[0] = 0;
    delete[] data[0];
    data[0] = nullptr;
    //then create new arrays,both for height and data, and both are
    //1 size smaller than the former one.
    char** temp_data = new char*[width-1];
    int* temp_height = new int[width-1];
    //then do the copy stuff. and remember for this situation,
    //THE INDEX SHOULD BE !!! MINUSING 1 !!!, BUT LOOP THE SAME TIMES.
    //copy height data
    for (int i = 0; i < width-1; i++){
        temp_height[i] = heights[i+1];
    }
    //copy data array, BE CAREFUL WITH THE INDEX.
    for (int i = 0; i < width-1; i++){
        char* temp_data_column = new char[heights[i+1]];
        for(int j = 0; j < heights[i+1]; j++){
            temp_data_column[j] = data[i+1][j];
        }
        temp_data[i] = temp_data_column;
    }
    //delete the old and link the new one
    delete[] heights;
    for (int i = 0; i < width; i++){
        delete[] data[i];
    }
    delete[] data;
    heights = temp_height;
    data = temp_data;
    width -= 1;
}

//add a blank colum at the right side of the tetris game
void Tetris::add_right_column(){
    //width + 1
    width += 1;
    //create two array at the new size
    char** temp_data = new char*[width];
    int* temp_height = new int[width];
    //do the copy stuff
    for (int i = 0; i < width-1; i++){
        //copy heights from former heights
        temp_height[i] = heights[i];
        char* temp_data_column = new char[heights[i]];
        // copy the content from old data array
        for (int j = 0; j < heights[i]; j++){
            temp_data_column[j] = data[i][j];
        }
        temp_data[i] = temp_data_column;
    }
    //set data for the new column
    temp_height[width-1] = 0;
    temp_data[width-1] = new char[0];
    //delete the old one and link new one
    delete[] heights;
    heights = temp_height;
    for (int i = 0; i < width-1; i++){
        delete[] data[i];
    }
    delete[] data;
    data = temp_data;
};

//remove the colum at the right side of the tetris game. take no arguments.
void Tetris::remove_right_column(){
    //record the squares in the row that is removed
    int square_counter = 0;
    for (int i = 0; i < heights[width-1]; i++){
        if(data[width-1][i] != ' '){
            square_counter += 1;
        }
    }
    //delete the amount of blocks that is removed
    square -= square_counter;
    //delete the data of heights and data of the last column
    heights[width-1] = 0;
    delete[] data[width-1];
    data[width-1] = nullptr;
    //create the new array witch the size is 1 smaller than before
    char** temp_data = new char*[width-1];
    int* temp_height = new int[width-1];
    //do the copy stuff
    for (int i = 0; i < width-1; i++){
        //copy heights from former heights
        temp_height[i] = heights[i];
        char* temp_data_column = new char[heights[i]];
        // copy the content from old data array
        for (int j = 0; j < heights[i]; j++){
            temp_data_column[j] = data[i][j];
        }
        temp_data[i] = temp_data_column;
    }
    //delete the old data and link the new data
    for (int i = 0; i < width; i++){
        delete[] data[i];
    }
    delete[] data;
    delete[] heights;
    heights = temp_height;
    data = temp_data;
    width -= 1;
}; 
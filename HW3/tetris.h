#ifndef __TETRIS_H_
#define __TETRIS_H_

class Tetris{
    private:
        int width;
        char** data;
        int* heights;
        int square;
    public:
        //Constructor
        Tetris(int w);
        //copy constructor
        Tetris(const Tetris& T1);
        //destructor
        ~Tetris();
        //operator override
        Tetris& operator=(const Tetris& tetris_in);
        //Accessor
        int get_width() const{return width;}
        int count_squares() const{return square;}
        //other function
        int get_max_height() const;
        void add_piece(char type,int degree,int begin_position);
        void print() const;
        
        
        int remove_full_rows();
        void clear();
        
        void add_left_column();
        void remove_left_column();
        void remove_right_column(); 
        void add_right_column();
};

#endif
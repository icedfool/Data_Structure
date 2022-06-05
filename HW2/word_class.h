#ifndef __WORD_CLASS_H_
#define __WORD_CLASS_H_
#include <string>

class Word{
    public:

        //Defalt Constructor
        Word();
        
        //Constructor
        Word(std::string& in_string);
        
        //Accessor
        std::string get_word() const{return m_word_content;}
        int get_word_size() const{return m_word_size;}
        int get_score() const{return m_score;}
        bool get_pangram() const{return m_pangram;}

        //Modifier
        void set_word(std::string& in_word);
        void set_word_size(int& word_size);
        void set_score(int score);
        void set_pangram(bool pangram_status);

        //other function
        void calculate_score_and_detect_pangram(std::string& in_string);
    private:
        std::string m_word_content;
        int m_word_size;
        int m_score;
        bool m_pangram;
};
#endif
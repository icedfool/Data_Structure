#ifndef __BEE_STATUS_H_
#define __BEE_STATUS_H_
#include <string>
#include <iostream>
#include <vector>

#include "word_class.h"
class Bee_Status{
    public:
        //Constructor
        Bee_Status();

        //Accessor
        int get_date() const{return m_date;}
        std::string get_month();
        int get_year() const {return m_year;}  
        std::vector<Word> get_words() const{return m_words;}
        std::string get_letters() const{return m_letters;}
        int get_score() const{return m_score;}
        int get_pangram() const{return m_pangram;}
        int get_max_size() const{return m_max_size;}
        //Modifier
        void set_date(int date);
        void set_month(std::string& month);
        void set_year(int year);  
        void set_words(std::vector<Word>& words);
        void set_letters(std::string letters);
        void set_score(int& score);
        void set_pangram(int& pangram);
        // void set_max_size();
        void set_max_size(int max_size);

        //other function
        //void getdata();
        
    private:
        //class member
        std::string m_letters;
        std::vector<Word> m_words;
        int m_date;
        int m_month;
        int m_year;
        int m_pangram;
        int m_score;
        int m_max_size;
};
#endif
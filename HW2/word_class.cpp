#include "word_class.h"
#include <string>
#include <algorithm>
#include <vector>

//default constructor
Word::Word(){
    m_pangram = false;
    m_score = 0;
    m_word_size = 0;
    m_word_content = "";
}

//constructor with string
Word::Word(std::string& in_string){
    m_pangram = false;
    m_score = 0;
    m_word_content = in_string;
    m_word_size = in_string.size();
    calculate_score_and_detect_pangram(in_string);
};

//several Modifier
void Word::set_word(std::string& in_word){
    m_word_content = in_word;
};

void Word::set_word_size(int& word_size){
    m_word_size = word_size;
};

void Word::set_score(int score){
    m_score = score;
};

void Word::set_pangram(bool pangram_status){
    m_pangram = pangram_status;
};

//by reading one string to calculate the score and detect
//whether this string is pangram or no. it will call set_score
//and set pangram.
void Word::calculate_score_and_detect_pangram(std::string& in_string){
    int temp = in_string.size();
    if (temp == 4){
        Word::set_score(1);
    }
    if (temp == 5){
        Word::set_score(5);
    }
    if (temp == 6){
        Word::set_score(6);
    }
    if (temp >= 7){
        std::vector<char> chars_of_string;
        for (int i = 0; i < temp; i++){
            chars_of_string.push_back(in_string[i]);
        }

        //make sure the char in this distinguish vector is unique
        sort(chars_of_string.begin(), chars_of_string.end());
        chars_of_string.erase(unique(chars_of_string.begin(), \
                              chars_of_string.end()), chars_of_string.end());

        //if this is a pangram word, then give it the bonus,else,pass.
        if (chars_of_string.size() == 7){
            set_pangram(true);
            set_score(temp+7);
        }
        else{
            set_score(temp);
        }
    }
};
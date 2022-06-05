#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

#include "bee_status.h"
using std::string;

Bee_Status::Bee_Status(){
     m_pangram = 0;
     m_score = 0;
     m_date = 0;
     m_letters = "";
     m_month = 0;
     m_year = 0;
};

//get_month has the similar design way compared with 
//set_month function, basicly a reverse
std::string Bee_Status::get_month(){
    if (m_month == 1){
        string temp = "January";
        return temp;
    }
    else if (m_month == 2){
        string temp = "February";
        return temp;
    }
    else if (m_month == 3){
        string temp = "March";
        return temp;
    }
    else if (m_month == 4){
        string temp = "April";
        return temp;
    }
    else if (m_month == 5){
        string temp = "May";
        return temp;
    }
    else if (m_month == 6){
        string temp = "June";
        return temp;
    }
    else if (m_month == 7){
        string temp = "July";
        return temp;
    }
    else if (m_month == 8){
        string temp = "August";
        return temp;
    }
    else if (m_month == 9){
        string temp = "September";
        return temp;
    }
    else if (m_month == 10){
        string temp = "October";
        return temp;
    }
    else if (m_month == 11){
        string temp = "November";
        return temp;
    }
    else if (m_month == 12){
        string temp = "December";
        return temp;
    }
    else{
        string temp = "";
        return temp;
    }
}

void Bee_Status::set_date(int date){
    m_date = date;
}

void Bee_Status::set_letters(std::string letters){
    m_letters = letters;
}

void Bee_Status::set_pangram(int& pangram){
    m_pangram = pangram;
}

void Bee_Status::set_score(int& score){
    m_score = score;
}

void Bee_Status::set_words(std::vector<Word>& words){
    m_words = words;
}

void Bee_Status::set_year(int year){
    m_year = year;
}

void Bee_Status::set_max_size(int max_size){
    m_max_size = max_size;
}
//convert month from string type to int type
//so that the compare stuff can be easier
void Bee_Status::set_month(std::string& month){
    if (month == "January"){
        m_month = 1;
    }
    else if (month == "February"){
        m_month = 2;
    }
    else if (month == "March"){
        m_month = 3;
    }
    else if (month == "April"){
        m_month = 4;
    }
    else if (month == "May"){
        m_month = 5;
    }
    else if (month == "June"){
        m_month = 6;
    }
    else if (month == "July"){
        m_month = 7;
    }
    else if (month == "August"){
        m_month = 8;
    }
    else if (month == "September"){
        m_month = 9;
    }
    else if (month == "October"){
        m_month = 10;
    }
    else if (month == "November"){
        m_month = 11;
    }
    else if (month == "December"){
        m_month = 12;
    }
}
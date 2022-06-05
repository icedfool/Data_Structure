#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<iomanip>

#include "word_class.h"
#include "word_class.cpp"
#include "bee_status.h"
#include "bee_status.cpp"

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::cerr;
using std::ifstream;
using std::ofstream;


//helper function for getting max size
int cmp(int&a , int&b){
    return a > b;
}

//sort helper function to detect which one is later.
bool is_late_than(Bee_Status& game1, Bee_Status& game2){
    if(game1.get_year() > game2.get_year()){
        return true;
    }
    else if(game1.get_year() == game2.get_year()){
        if(game1.get_month() > game2.get_month()){
            return true;
        }
        else if(game1.get_month() == game2.get_month()){
            //special case for date
            if(game1.get_date() < game2.get_date()){ //if have problem, change the sign.
                return true;
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

//sort helper function to detect which one has more score
bool has_more_score(Bee_Status& game1, Bee_Status& game2){
    if(game1.get_score() > game2.get_score()){
        return true;
    }
    else if(game1.get_score() == game2.get_score()){
        return is_late_than(game1,game2);
    }
    else{
        return false;
    }
}

//sort helper function to detect which one has more pangram words
bool has_more_pangram(Bee_Status& game1, Bee_Status& game2){
    if (game1.get_pangram() > game2.get_pangram()){
        return true;
    }
    else if(game1.get_pangram() == game2.get_pangram()){
        return has_more_score(game1,game2);
    }
    else{
        return false;
    }
}

int main(int argc, char* argv[]){
    ifstream infile(argv[1]);
    ofstream outfile(argv[2]);
    string command = argv[3];
    //make sure the infile can be open
    if (!infile) { 
        std::cerr << "ERROR: cannot open font file " << argv[1]<< std::endl; 
        exit(0);
    }
    
    //and for outfile
    if (!outfile){
        std::cerr << "ERROR: cannot open out file " << argv[2] << std::endl; 
        exit(0);
    }
    
    //make sure the argc and argv are right
    if (argc < 4){
        std::cerr<< "ERROR: Too few arguments."<<std::endl;
    }

    if (argc > 4){
        std::cerr<< "ERROR: Too many arguments."<<std::endl;
    }
    
    //buffer stuff
    string temp;
    string temp2;
    Bee_Status temp_class;
    vector<Word> temp_words;
    int temp_counter = 0;
    vector<int> temp_counter_vec;
    //vector that is used to store data
    vector<Bee_Status> games_data;

    
    //================================================process data
    infile>>temp;
    while (infile >> temp){
        // set day, month and year
        temp_class.set_month(temp);
        infile >> temp;
        temp_class.set_date(atoi(temp.c_str()));
        infile >> temp;
        temp_class.set_year(atoi(temp.c_str()));

        //set all the letters of one game to one single string
        for (int i = 0; i < 9; i++){
            while (infile >> temp){
                temp2 += temp;
                break;
            }
        }
        temp_class.set_letters(temp2);
        temp2.clear();

        //create a temp vector of Word class to store the words,
        //then use set_words to copy them to temp_class.
        infile >> temp;
        while (infile >> temp){
            if (temp[0] != '-' ){
                Word temp_word_class(temp);
                temp_words.push_back(temp_word_class);
            }

            //if next line is dash line,then return the data from one game 
            //to games_data, and then clear the buffer
            else{
                temp_class.set_words(temp_words);

                //use for loop tp get pangram number
                for (int i = 0; i < int(temp_class.get_words().size()); i++){
                    if (temp_class.get_words()[i].get_pangram() == true){
                        temp_counter += 1;
                    }
                }
                temp_class.set_pangram(temp_counter);
                temp_counter = 0;
                
                //use for loop to set score
                for (int i = 0; i < int(temp_class.get_words().size()); i++){
                    temp_counter += temp_class.get_words()[i].get_score();
                }
                temp_class.set_score(temp_counter);
                temp_counter = 0;

                for (int i = 0; i <int(temp_class.get_words().size()); i++){
                    temp_counter_vec.push_back(temp_class.get_words()[i].get_word_size());
                }
                std::sort(temp_counter_vec.begin(),temp_counter_vec.end(),cmp);
                //push_back words to temp_class
                temp_class.set_words(temp_words);
                temp_class.set_max_size(temp_counter_vec[0]);
                games_data.push_back(temp_class);
                temp_words.clear();
                temp.clear();
                temp2.clear();
                break;
            }
        }
    }
    
//similar stuff only this is to store the data for the final class
    temp_class.set_words(temp_words);

    //use for loop tp get pangram number
    for (int i = 0; i < int(temp_class.get_words().size()); i++){
        if (temp_class.get_words()[i].get_pangram() == true){
            temp_counter += 1;
        }
    }
    temp_class.set_pangram(temp_counter);
    temp_counter = 0;

    //use for loop to set score
    for (int i = 0; i < int(temp_class.get_words().size()); i++){
        temp_counter += temp_class.get_words()[i].get_score();
    }
    temp_class.set_score(temp_counter);
    temp_counter = 0;

    //use for loop to get the max size of the string
    for (int i = 0; i <int(temp_class.get_words().size()); i++){
        temp_counter_vec.push_back(temp_class.get_words()[i].get_word_size());
    }
    std::sort(temp_counter_vec.begin(),temp_counter_vec.end(),cmp);
    temp_class.set_max_size(temp_counter_vec[0]);

    //push_back words to temp_class
    temp_class.set_words(temp_words);
    //temp class all set, return them to game_data
    games_data.push_back(temp_class);

    //clear buffer
    temp_words.clear();
    temp.clear();
    temp2.clear();
    temp_counter_vec.clear();
    //================================================ end of process data

    //sort by condition
    std::sort(games_data.begin(),games_data.end(),has_more_pangram);

    //scenario 1 game_status:
    if (command == "--game_stats"){

        //to print line 1
        outfile<<std::left<<std::setw(20)<<"date";
        //remember to use buffer to store the length of a line
        temp_counter += 20;
        outfile<<std::left<<std::setw(18)<<"letters";
        temp_counter += 18;
        outfile<<std::left<<std::setw(7)<<"score";
        temp_counter += 7;
        outfile<<std::left<<std::setw(10)<<"pangrams";
        temp_counter += 10;
        outfile<<std::left<<std::setw(8)<<"#words";
        temp_counter += 8;
        //use the former buffer vector to find the maximum of the max_size
        for (int i = 0; i < int(games_data.size()); i++){
            temp_counter_vec.push_back(games_data[i].get_max_size());
        }
        std::sort(temp_counter_vec.begin(),temp_counter_vec.end(),cmp);
        for (int i = 4; i <= temp_counter_vec[0]; i++){
            outfile<<std::right<<std::setw(3)<<i;
            temp_counter += 3;
        }
        outfile<<std::endl;
        //========================================= end of line 1
        for (int i = 0; i < temp_counter; i++){
            outfile<<"-";
        }
        outfile<<std::endl;
        temp_counter = 0;
        int temp_counter2;
        //========================================= end of line 2
        
        //use for loop to print every line
        for (int i = 0; i < int(games_data.size()); i++){
            outfile<<std::left<<std::setw(10)<<games_data[i].get_month();
            outfile<<std::right<<std::setw(2)<<games_data[i].get_date()<<",";
            outfile<<" ";
            outfile<<std::left<<std::setw(5)<<games_data[i].get_year();
            outfile<<" ";
            for (int j = 0; j < int(games_data[i].get_letters().size()); j++){
                outfile<<std::left<<std::setw(2)<<games_data[i].get_letters()[j];
            }
            outfile<<std::right<<std::setw(5)<<games_data[i].get_score();
            outfile<<std::right<<std::setw(10)<<games_data[i].get_pangram();
            outfile<<std::right<<std::setw(8)<<games_data[i].get_words().size()<<"  ";
            
            //calculate how may string at this size show up
            temp_counter2 = 0;
            for (int k = 4; k <= int(temp_counter_vec[0]/* max string size */); k++){
                for (int l = 0; l < int(games_data[i].get_words().size()); l++){
                    if (int(games_data[i].get_words()[l].get_word_size()) == k){
                        temp_counter2 += 1;
                    }
                }
                if(temp_counter2 != 0){
                    outfile<<std::right<<std::setw(3)<<temp_counter2;
                }
                else{
                    outfile<<"   ";
                }
                temp_counter2 = 0;
            }
            outfile<<endl;
        }
    }
    //================================================= end of scenario 1 game_status

    //scenario 2 word_status
    if(command == "--word_stats"){
        //i don't think i'm able to write this so far... I just finished process data in
        // Thursday, so i guess next time I should work faster?
    }
    //================================================= end of scenario 2 word_status

    //scenario 3 letter_status
    if (command == "--letter_stats"){
        //first two lines
        outfile<<"letter   #center   avg center   #other   avg other"<<endl;
        outfile<<"--------------------------------------------------"<<endl;
        
        //buffer stuff
        int temp_counter_for_center = 0;
        int temp_counter_for_other = 0;
        float temp_counter_for_score_center = 0.0;
        float temp_counter_for_score_other = 0.0;
        vector<int> temp_vec_for_center;
        vector<int> temp_vec_for_other;

        //generate every line content,from a to z
        for (int i = 97; i <= 122; i++){
            outfile<<std::left<<std::setw(9)<<char(i);
            //for loop throgh every game
            for (int j = 0; j < int(games_data.size()); j++){
                //if get_letters[1](which must be the center letter) is equal to
                //current character, then +1 the temp counter.
                if (games_data[j].get_letters()[1] == char(i)){
                    temp_counter_for_center += 1;
                    temp_vec_for_center.push_back(j);
                }
                //if it isn't, then for loop the string, and if it match the current char,
                //+1 another temp counter.
                else{
                    for (int m = 3; m <= 8; m++){
                        if (games_data[j].get_letters()[m] == char(i)){
                            temp_counter_for_other += 1;
                            temp_vec_for_other.push_back(j);
                        }
                    }
                }
            }
            //calculate two scores
            for (int i = 0; i < int(temp_vec_for_center.size()); i++){
                temp_counter_for_score_center += games_data[int(temp_vec_for_center[i])].get_score();
            }
            temp_counter_for_score_center = (temp_counter_for_score_center/temp_vec_for_center.size());
            for (int i = 0; i < int(temp_vec_for_other.size()); i++){
                temp_counter_for_score_other += games_data[int(temp_vec_for_other[i])].get_score();
            }
            temp_counter_for_score_other = (temp_counter_for_score_other/temp_vec_for_other.size());

            //print the rest of a line
            if(temp_counter_for_center != 0){
                outfile<<std::right<<std::setw(7)<<temp_counter_for_center;
                outfile<<std::right<<std::setw(13)<<std::fixed<<std::setprecision(1)<<temp_counter_for_score_center;
            }
            else{
                outfile<<"       ";
                outfile<<"             ";
            }
            if (temp_counter_for_other != 0){
                outfile<<std::right<<std::setw(9)<<temp_counter_for_other;
                outfile<<std::right<<std::setw(12)<<std::fixed<<std::setprecision(1)<<temp_counter_for_score_other;
            }
            else if(temp_counter_for_other == 0){
                outfile<<"               ";
            }
            outfile<<endl;
            //claer buffer;
            temp_counter_for_center = 0;
            temp_counter_for_other = 0;
            temp_counter_for_score_center = 0;
            temp_counter_for_score_other = 0;
            temp_vec_for_center.clear();
            temp_vec_for_other.clear();
        }
    }
    //=========================================== end of scenraio 3

    //scenraio 4 customer_status
    // count the word begin with certain characters. for example, if the 
    // word is "bull", then the count of b plus 1. by using this fuction,
    // we are able to find in all the games which letter is able to format 
    // more words.
    if (command == "--custom_stats"){
        //first two lines
        outfile<<"Character                              Count"<<endl;
        outfile<<"--------------------------------------------"<<endl;
        for (int i = 97; i <= 122; i++){
            char temp_char = char(i);
            outfile<<temp_char;
            int temp_counter_for_string = 0;
            for (int j = 0; j < int(games_data.size()); j++){
                for (int k = 0; k < int(games_data[j].get_words().size()); k++){
                    if(games_data[j].get_words()[k].get_word()[0] == char(i)){
                        temp_counter_for_string += 1;
                    }
                }
            }
            if (temp_counter_for_string != 0){
                outfile<<std::right<<std::setw(43)<<temp_counter_for_string<<endl;
            }
            else{
                 outfile<<std::right<<std::setw(43)<<" "<<endl;
            }
            temp_counter_for_string = 0;
        }
    }
}
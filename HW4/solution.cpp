#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <list>

#include "diff.h"
#include "solution.h"


std::list<Diff> compute_diff(std::string command, 
                             std::list<std::string> string_list_a,  
                             std::list<std::string> string_list_b){
    //create two iterator point at two list's begin.
    std::string command_string = command.substr(0,12);
    typename std::list<std::string>::iterator iter_a = string_list_a.begin();
    typename std::list<std::string>::iterator iter_b = string_list_b.begin();
    int temp_counter_a = 0;//to store the current index of string list a
    int temp_counter_b = 0;//to store the current index of string list b
    std::list<Diff> return_diff_list;
    while(iter_a != string_list_a.end() && iter_b != string_list_b.end()){
        //if the content in both iterator is same, then keep looping till find sth.
        //different
        if(*iter_b == *iter_a){
            iter_a++;
            iter_b++;
            temp_counter_a++;
            temp_counter_b++;
            continue;
        }
        else if (*iter_a != *iter_b){
            std::list<std::string>::iterator iter_a_copy = iter_a;
            std::list<std::string>::iterator iter_b_copy = iter_b;
            //case 1, the content in line A appears earlier,
            if(*(++iter_b_copy) == *iter_a){
                Diff temp_diff(temp_counter_a,INSERT,(*iter_b));
                return_diff_list.push_back(temp_diff);
                //make sure the temp_counter is pointing the right index
                //and two iters are pointing at same element
                iter_b++;
                temp_counter_b++;
            }

            //case 2, the content in line A appears later(which means A delete this content)
            //so the diff should be current index,ERASE and the content in second string.
            else if (*(++iter_a_copy) == *iter_b){
                Diff temp_diff(temp_counter_a,ERASE,*iter_a);
                return_diff_list.push_back(temp_diff);
                //make sure the temp_counter is pointing the right index
                //and two iters are pointing at same element
                iter_a++;
                temp_counter_a++;
            }
            //case 3, probebaly is a replace situation, so the diff should be
            //current index,REPLACE and the content in second string.
            else{
                Diff temp_diff(temp_counter_a,REPLACE,(*iter_b));
                return_diff_list.push_back(temp_diff);
                //since these two iterators are pointing at different values and 
                //will not go into looping, we have to add both two counters and
                //iterators.
                iter_a++;
                iter_b++;
                temp_counter_a++;
                temp_counter_b++;
            }            
        }
    }
    //two exceptions, first is that the string_a is less then string_b, and another
    //one is string_b less than string_a, in these two cases should be handle seperately.
    if(iter_a == string_list_a.end() && iter_b != string_list_b.end()){
        while (iter_b != string_list_b.end()){
            Diff temp_diff(temp_counter_b,INSERT,(*iter_b));
            return_diff_list.push_back(temp_diff);
            temp_counter_b++;
            iter_b++;
        }
    }
    else if (iter_b == string_list_b.end() && iter_a != string_list_a.end()){
        while(iter_a != string_list_a.end()){
            Diff temp_diff(temp_counter_a,ERASE,(*iter_a));
            return_diff_list.push_back(temp_diff);
            temp_counter_a++;
            iter_a++;
        }
    }
    
    return return_diff_list;
};


std::list<Diff> invert_diff(std::list<std::string> string_list_a,  
                            std::list<Diff> diff_a){
    //create two iterator point at two list's begin.
    typename std::list<std::string>::iterator iter_a = string_list_a.begin();
    typename std::list<Diff>::iterator iter_diff_a = diff_a.begin();
    std::list<Diff> return_diff_list;// the diff list that is going to be returned
    int temp_counter = 0;//to store the current index 
    int temp_counter_string2 = 0;//the current index of the sample_string
    //while two iterator not reachs end, just keep going
    while(iter_diff_a != diff_a.end() && iter_a != string_list_a.end()){
        if((*iter_diff_a).getPosition() != temp_counter){
            iter_a++;
            temp_counter++;
            temp_counter_string2++;
        }
        else{
            if((*iter_diff_a).getType() == ERASE){
                std::string temp_string = *iter_a;
                //temp_counter_string2 is the current index
                Diff temp_diff(temp_counter_string2,INSERT,temp_string);
                return_diff_list.push_back(temp_diff);
                //make sure two index matchs and the content at this index matchs.
                iter_a++;
                temp_counter++;
                //increase the diff_iter to go over commands
                iter_diff_a++;
            }
            else if ((*iter_diff_a).getType() == INSERT){
                Diff temp_diff(temp_counter_string2,ERASE);
                return_diff_list.push_back(temp_diff);
                //make sure two index matchs
                temp_counter_string2++;
                //increase the diff_iter to go over commands
                iter_diff_a++;

            }
            else if ((*iter_diff_a).getType() == REPLACE){
                std::string temp_string = *iter_a;
                //temp_counter_string2 is the current index for sample_string
                Diff temp_diff(temp_counter_string2,REPLACE,temp_string);
                return_diff_list.push_back(temp_diff);
                //increase the diff_iter to go over commands
                iter_diff_a++;
            }
        }
    }
    return return_diff_list;
};

void apply_diff(std::list<std::string>& string_list_a,  
                std::list<Diff> diff_a){
    //create two iterator point at two list's begin.
    typename std::list<std::string>::iterator iter_a = string_list_a.begin();
    typename std::list<Diff>::iterator iter_diff_a = diff_a.begin();
    int temp_counter = 0;//to store the current index 
    //while two iterator not reachs end, just keep going
    while(iter_diff_a != diff_a.end() && iter_a != string_list_a.end()){
        //if current index do not match the diff_list, just keep going
        if((*iter_diff_a).getPosition() != temp_counter){
            iter_a++;
            temp_counter++;
        }
        else{
            //case 1,command is erase, then erase current content
            //and the iterator will point at the next value, so the
            //temp counter also need ++.
            if((*iter_diff_a).getType() == ERASE){
                iter_a = string_list_a.erase(iter_a);
                temp_counter++;
                //remember to ++ for the diff iter or it will not read
                // new command.
                iter_diff_a++;
            }
            //case 2,command is insert, then insert current content
            //and the iterator will point at the new insert value, so the
            //iter_a also need ++. however, the temp_counter still
            //point at the right index, so do not edit it.
            else if ((*iter_diff_a).getType() == INSERT){
                iter_a = string_list_a.insert(iter_a,(*iter_diff_a).getValue());
                //remember to ++ for the diff iter or it will not read
                // new command.
                iter_diff_a++;
                iter_a++;
            }
            //case 3,command is replace, then delete the old value 
            //and insert current content, both the iter_a and temp_counter
            //will not change
            else if ((*iter_diff_a).getType() == REPLACE){
                iter_a = string_list_a.erase(iter_a);
                string_list_a.insert(iter_a,(*iter_diff_a).getValue());
                temp_counter++;
                //remember to ++ for the diff iter or it will not read
                // new command.
                iter_diff_a++;
            }
        }
    }
};


//assert if the two string list is same.
bool assert_same(std::list<std::string> string_list_a,  
                 std::list<std::string> string_list_b){
    //create two iterator point at two list's begin.
    std::list<std::string>::iterator iter_a = string_list_a.begin();
    std::list<std::string>::iterator iter_b = string_list_b.begin();
    //return the bool value with certain conditions
    if(string_list_b.size() != string_list_a.size()){
        return false;
    }
    for (int i = 0; i < int(string_list_a.size()); i++){
        if(*iter_a != *iter_b){
            return false;
        }
    }
    return true;
};

//assert if the two diff list is same.
bool assert_same_diff(std::list<Diff> Diff_list_a,  
                      std::list<Diff> Diff_list_b){
    //create two iterator point at two list's begin.
    std::list<Diff>::iterator iter_a = Diff_list_a.begin();
    std::list<Diff>::iterator iter_b = Diff_list_b.begin();
    //return the bool value with certain conditions
    if(Diff_list_a.size()!= Diff_list_b.size()){
        return false;
    }
    for(int i = 0; i < int(Diff_list_a.size()); i++){
        if((*iter_a).getPosition() != (*iter_b).getPosition()){
            return false;
        }
        else{
            if((*iter_a).getValue() != (*iter_b).getValue()){
                return false;
            }
            else{
                if((*iter_a).getType() != (*iter_b).getType()){
                    return false;
                }
            }
        }
    }
    return true;
};
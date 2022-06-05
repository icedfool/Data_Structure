#ifndef _SOLUTION_H_
#define _SOLUTION_H_

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <list>

#include "diff.h"

std::list<Diff> compute_diff(std::string command, 
                             std::list<std::string> string_list_a,  
                             std::list<std::string> string_list_b);

std::list<Diff> invert_diff(std::list<std::string> string_list_a,  
                            std::list<Diff> diff_a);

void apply_diff(std::list<std::string>& string_list_a,  
                std::list<Diff> diff_a);

bool assert_same(std::list<std::string> string_list_a,  
                 std::list<std::string> string_list_b);

bool assert_same_diff(std::list<Diff> Diff_list_a,  
                      std::list<Diff> Diff_list_b);

#endif
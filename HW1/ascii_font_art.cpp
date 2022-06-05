#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <algorithm>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;
// ======================================================================================

// Helper function to read the provided font from a file.  The format
// of the font file is described in comments below.  The width,
// height, and bitmap_letters variables are set by this function.
void ReadFont(const std::string &font_file, 
	      int &width,
	      int &height,
	      std::vector<std::vector<std::string> > &bitmap_letters) {

  // open the font file for reading
  std::ifstream istr(font_file.c_str());
  if (!istr) { 
    std::cerr << "ERROR: cannot open font file " << font_file << std::endl; 
    exit(0);
  }

  // read in the width & height for every character in the file
  istr >> width >> height;
  assert (width >= 1);
  assert (height >= 1);

  // Create a vector to store all 256 ASCII characters of the
  // characters.  Each character is represented as a vector of
  // <height> strings that are each <width> wide.  Initially the
  // characters are unknown (represented with the '?' character).
  bitmap_letters = std::vector<std::vector<std::string> > 
    ( 256, std::vector<std::string> ( height, std::string(width, '?')));

  // read in all the characters
  // first is the ascii integer representation of the character
  int ascii;
  while (istr >> ascii) {
    assert (ascii >= 0 && ascii < 256);
    // next the character is printed in single quotes
    char c;
    istr >> c;
    assert (c == '\'');
    // use std::noskipws to make sure we can read the space character correctly
    istr >> std::noskipws >> c;
    // verify that the ascii code matches the character
    assert (c == (char)ascii);
    // switch back to std::skipws mode
    istr >> std::skipws >> c;
    assert (c == '\'');
    // read in the letter
    std::vector<std::string> bitmap;
    std::string tmp;
    for (int i = 0; i < height; i++) {
      istr >> tmp;
      assert ((int)tmp.size() == width);
      // make sure the letter uses only '#' and '.' characters
      for (unsigned int j = 0; j < tmp.size(); j++) { 
        assert (tmp[j] == '.' || tmp[j] == '#'); 
      }
      bitmap.push_back(tmp);
    }
    // overwrite the initially unknown letter in the vector
    bitmap_letters[ascii] = bitmap;
  }
}

// ======================================================================================

//display to terminal function:
void display_to_terminal(char**& argv,
                         vector<vector<string>>&bitmap_letters,\
                         vector<int>& ascii_of_str,
                         int& height){
    string input_string = argv[3];
    char fore_ground = argv[4][0];
    char back_ground = argv[5][0];

    //store the ascii of every char from the string
    for(int i = 0; i < int(input_string.size()); i++){
      ascii_of_str.push_back(int(input_string[i]));
    }
    
    //replace stuff
    for(int i = 0 ; i < int(bitmap_letters.size()); i++){
      for (int j = 0; j < int(bitmap_letters[i].size()); j++){
        replace(bitmap_letters[i][j].begin(),bitmap_letters[i][j].end(),'#',fore_ground);
        replace(bitmap_letters[i][j].begin(),bitmap_letters[i][j].end(),'.',back_ground);
      }
    }
    
    //print function
    for (int i = 0; i < height; i++){
      for (int j = 0; j < int(ascii_of_str.size()); j++){
        cout<<bitmap_letters[ascii_of_str[j]][i]<<back_ground;
      }
      cout<<endl;
    }
}

//display to file function
void display_to_file(char**&argv,
                     vector<vector<string>>&bitmap_letters,\
                     vector<int>& ascii_of_str,
                     int& height){
  string input_string = argv[3];
  char fore_ground = argv[4][0];
  char back_ground = argv[5][0];
  ofstream out_file(argv[7]);
    
  if (!out_file.good()) {
    std::cerr << "Can't open " << argv[7] << " to write.\n";
    exit(1);
  }

    //store the ascii of every char from the string
  for(int i = 0; i < int(input_string.size()); i++){
    ascii_of_str.push_back(int(input_string[i]));
  }
    
    //replace stuff
  for(int i = 0 ; i < int(bitmap_letters.size()); i++){
    for (int j = 0; j < int(bitmap_letters[i].size()); j++){
      replace(bitmap_letters[i][j].begin(),bitmap_letters[i][j].end(),'#',fore_ground);
      replace(bitmap_letters[i][j].begin(),bitmap_letters[i][j].end(),'.',back_ground);
    }
  }
    
    //write all these stuff to out_file
  for (int i = 0; i < height; i++){
    for (int j = 0; j < int(ascii_of_str.size()); j++){
      out_file<<bitmap_letters[ascii_of_str[j]][i]<<back_ground;
    }
    out_file<<endl;
  }
}

//turn bitmap into ascii letters function
void read_file(char**&argv,
               vector<vector<string>>&bitmap_letters){
  string infile_string = argv[3];
  ifstream infile(infile_string);
  vector<string> ascii_distinguish_vec;
  vector<string> input_string_in_line;
  vector<string> string_to_be_compare;
  string temp;
  string temp_2;

  if (!infile.good()) { 
    std::cerr << "ERROR: cannot open font file " << infile_string << std::endl; 
    exit(0);
  }
  //turn the bitmap into a vector of long string and prepare
  //to compare with input file.
  for(int i = 0 ; i < int(bitmap_letters.size()); i++){
    for (int j = 0; j < int(bitmap_letters[i].size()); j++){
      temp.append(bitmap_letters[i][j]);
      if((j+1) == int(bitmap_letters[i].size())){
        ascii_distinguish_vec.push_back(temp);
        temp.clear();
      }
    }
  }

  //turn the file into line format
  while (std::getline(infile,temp)){
    input_string_in_line.push_back(temp);
    temp.clear();
  }

  //get the fore_ground char and back_ground char
  string temp_string;
  temp_string.append(input_string_in_line[0]);
  char back_ground = temp_string[6];
  char fore_ground;
  for (int i = 0; i < int(temp_string.size()); i++){
    if (temp_string[i] != back_ground){
      fore_ground = temp_string[i];
      break;
    }
  }

  //separate the line so that we can get a long string form
  //of the char bitmap
  for (int i = 0; i < int(input_string_in_line.size()); i++){
    for (int j = 0; j < int((input_string_in_line[i].size()/7)); j++){
        temp_2 = input_string_in_line[i].string::substr(7*j,6);
        string_to_be_compare.push_back(temp_2);
        temp_2.clear();
    }
  }

  //use string_to_compare as a buffer and put the long string stuff
  //into input_string_in_line and prepare to compare
  input_string_in_line.clear();
  for (int i = 0; i < int((string_to_be_compare.size()/7)); i++){
    for (int j = 0; j < 7; j++){
      int temp1 = string_to_be_compare.size()/7;
      temp_2.append(string_to_be_compare[j*temp1+i]);
    }
    input_string_in_line.push_back(temp_2);
    temp_2.clear();
  }

  //clear buffer to save RAM
  string_to_be_compare.clear();

  //replace charater to match the compare vector
  for (int i = 0; i < int(input_string_in_line.size()); i++){
    replace(input_string_in_line[i].begin(),input_string_in_line[i].end(),fore_ground,'#');
    replace(input_string_in_line[i].begin(),input_string_in_line[i].end(),back_ground,'.');
  }

  //compare input_string_in_line and ascii_distinguish_vec, and we can get 
  //the ascii number(index) that we want.
  vector<int> ascii_nums;
  for (int i = 0; i < int(input_string_in_line.size()); i++){
    for (int j = 0; j < int(ascii_distinguish_vec.size()); j++){
      if (input_string_in_line[i] == ascii_distinguish_vec[j]){
        ascii_nums.push_back(j);
      }
    }
  }

  //print them!
  for (int i = 0; i < int(ascii_nums.size()); i++){
    cout<<char(ascii_nums[i]);
  }
}

void display_to_terminal_with_kerning(){}


int main(int argc, char* argv[]){
  if (argc < 4){
    std::cerr<<"Too few arguments. Please check."<<endl;
    exit(0);
  }
  if (argc > 8){
    std::cerr<<"Too many arguments. Please check."<<endl;
    exit(0);
  }
  
  string command = argv[1];
  string infile = argv[2];
  vector<vector<string>> bitmap_letters;
  vector<int> ascii_of_str;
  int width;
  int height;

  if (command == "display" && (argc < 6)){
    std::cerr<<"Too few arguments. Please check."<<endl;
    exit(0);
  }

  //generate vector of bitmap letters
  ReadFont(argv[2],width,height,bitmap_letters);
  
  // seriaro 1 display to terminal:
  if (command == "display" && argc == 6){
    display_to_terminal(argv,bitmap_letters,ascii_of_str,height);
  }

  //scenario 2 read:
  if (command == "read"){
    read_file(argv,bitmap_letters);
  }

  //scenario 3 print to file 
  if (command == "display" && argc == 8){
    display_to_file(argv,bitmap_letters,ascii_of_str,height);
  }

  /* // scenario 4 print with kerning
  if (command == "display" && argc == 7 && argv[6] == "kerning"){
    //currently not gonna writing it
  } */
}

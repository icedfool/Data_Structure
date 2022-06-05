#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<unordered_map>
#include<cassert>
#include<map>
#include<algorithm>
#include<cmath>

class Star_Data {
public:
  float x;
  float y;
  float apparent_magnitude;
  std::string which_constellation;
  std::string name;
};

class Line_Data {
public:
  std::string which_constellation;
  std::string name_a;
  std::string name_b;
};

class Constellation_Data {
public:
  std::string color;
  float min_x;
  float min_y;
  float max_x;
  float max_y;
};

float calculate_distance(float x1, float y1, float x2, float y2) {
  return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

bool pair_match(std::pair<std::string,float> a, std::pair<std::string,float> b) {
  if (a.second - b.second < 0.001) {
    return true;
  }
  return false;
}

std::string process_line_data(std::pair<std::string,float> a, 
                       std::pair<std::string,float> b,
                       std::vector<Star_Data>& star_data,
                       std::vector<Star_Data>& lib_star_data) {
  std::string name_a = a.first.substr(0,10);
  std::string name_b = a.first.substr(11,10);
  std::string change_name_a = b.first.substr(0,10);
  std::string change_name_b = b.first.substr(11,10);

  std::vector<Star_Data>::iterator it = star_data.begin();
  std::vector<Star_Data>::iterator it2 = lib_star_data.begin();
  for(;it != star_data.end(); it++) {
    if(it->name == name_a) {
      it->name = change_name_a;
      it->which_constellation = lib_star_data[0].which_constellation;
      for(;it2 != lib_star_data.end(); it2++) {
        if(it2->name == change_name_a) {
          it->apparent_magnitude = it2->apparent_magnitude;
        }
      }
    } 
  }
  std::string content = "assigning  "+name_a+"  "+change_name_a+"\n"+
                        "assigning  "+name_b+"  "+change_name_b+"\n";
  return content;
}
int main(int argc, char* argv[]){
  // ======================================================================
  // LOAD THE INPUT CONSTELLATION FILE DATA
  //

  // process the input library file
  std::vector<std::vector<Star_Data>> library_star_data;
  std::vector<std::vector<Line_Data>> library_line_data;
  
  for(int i = 1; i < argc; i++){
    std::string temp_arguemnt = argv[i];
    if (temp_arguemnt == "-l"){
      std::string temp;
      std::string library_filename = argv[i+1];
      std::ifstream library_file(library_filename);
      std::vector<Star_Data> temp_library_star_data;
      std::vector<Line_Data> temp_library_line_data;
      while(library_file >> temp){
        // read each line segment
        if (temp == "star") {
          Star_Data sd;
          library_file >> sd.x >> sd.y >> sd.apparent_magnitude >> sd.which_constellation >> sd.name;
          temp_library_star_data.push_back(sd);
        }

        // read each line segment
        else if (temp == "line") {
          Line_Data ld;
          library_file >> ld.which_constellation >> ld.name_a >> ld.name_b;
          temp_library_line_data.push_back(ld);
        }
      }
      library_star_data.push_back(temp_library_star_data);
      library_line_data.push_back(temp_library_line_data);
    }
  }
  std::string input_filename = argv[2];
  std::ifstream istr(input_filename);
  assert (istr.good());

  // place to store the data
  std::vector<Star_Data> star_data;
  std::vector<Line_Data> line_data;
  std::map<std::string,Constellation_Data> constellation_data;
  
  std::string token;
  while (istr >> token) {

    // read each star 
    if (token == "star") {
      Star_Data sd;
      istr >> sd.x >> sd.y >> sd.apparent_magnitude >> sd.which_constellation >> sd.name;
      star_data.push_back(sd);
    }

    // read each line segment
    else if (token == "line") {
      Line_Data ld;
      istr >> ld.which_constellation >> ld.name_a >> ld.name_b;
      line_data.push_back(ld);
    }
  }

  //use a unordered_map to store the values of the distance between stars
  std::unordered_map<std::string,float> distance_map;
  std::unordered_map<std::string,float> library_distance_map;

  //use two for loops to claculate the distance between stars and store them
  for (int i = 0; i < star_data.size(); i++){
    std::string name_a = star_data[i].name;
    for(int j = i+1; j < star_data.size(); j++){
      std::string name_b = star_data[j].name;
      float distance = calculate_distance(star_data[i].x, star_data[i].y, star_data[j].x, star_data[j].y);
      std::string name_temp = name_a + " " + name_b;
      distance_map[name_temp] = distance;
    }
  }

  //use a unordered_map to store the values of the distance between stars of the library file
  for (int i = 0; i < library_star_data.size(); i++){
    for (int j = 0; j < library_star_data[i].size(); j++){
      std::string name_a = library_star_data[i][j].name;
      for (int k = j+1; k < library_star_data[i].size(); k++){
        std::string name_b = library_star_data[i][k].name;
        float distance = calculate_distance(library_star_data[i][j].x, library_star_data[i][j].y, 
                                            library_star_data[i][k].x, library_star_data[i][k].y);
        std::string name_temp = name_a + " " + name_b;
        library_distance_map[name_temp] = distance;
      }
    }
  }
  
  // ======================================================================
  // compare the data of the distance map and the library distance map to find the
  // matching pairs, and then try to process the line data
  std::vector<std::string> output_lines;
  bool found = false;
  int found_lib_index = 0;
  for(int library_index = 0; library_index < library_distance_map.size(); library_index++){
    for(std::unordered_map<std::string,float>::iterator it = distance_map.begin(); it != distance_map.end(); it++){
      for(std::unordered_map<std::string,float>::iterator it2 = library_distance_map.begin(); it2 != library_distance_map.end(); it2++){
        if (pair_match((*it),(*it2))){
          found = true;
          found_lib_index = library_index;
          std::string temp = process_line_data((*it), (*it2), star_data, library_star_data[library_index]);
          output_lines.push_back(temp);
        }
      }
    }
  }

  if (found == true){
    std::cout<<"FOUND CONSTELLATION "<<library_star_data[0][0].which_constellation<<std::endl;
    for(int i = 0; i< output_lines.size(); i++){
      std::cout<<output_lines[i]<<std::endl;
    }
  }
  else{
    std::cout<<"No matching constellation found"<<std::endl;
  }
}
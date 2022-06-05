#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

//
// This is a standalone program for plotting constellation files with
// stars & line segments -- input file, constellation library file,
// and output file.  No changes should be necessary to this program.
//
//   g++ -g -Wall -Wextra plot_stars.cpp -o plot.out
//
//
// Here are some sample command lines
//
//   ./plot.out example_input.txt example_input.html
//   ./plot.out --draw_diagram --label_constellation Ursa_Minor.txt Ursa_Minor.html
//   ./plot.out --draw_diagram --label_constellation --label_stars example_output.txt example_output.html
//
// You can view the files produced by the program with any web modern
// browser (Chrome, Firefox, Safari, Edge).


// =================================================================
// Simple classes to hold the data from the constellation files

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


// =================================================================
// Helper functions to prepare the SVG file

// Setup a black screen sized 1000x1000 to draw on
void draw_begin(std::ostream &ostr) {
  ostr << "<?xml version=\"1.0\" standalone=\"no\"?>" << std::endl;
  ostr << "<svg width=\"1000\" height=\"1000\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;
  ostr << "<polygon points= \"0,0 0,1000 1000,1000 1000,0\" style=\"fill:#000000\" />" << std::endl;
}

// Draw the lines of the constellation with thin lines
void draw_lines(std::ostream &plot_str,
                std::vector<Star_Data> &star_data,
                std::vector<Line_Data> &line_data,
                const std::map<std::string,Constellation_Data> &constellation_data) {
  for (unsigned int i = 0; i < line_data.size(); i++) {
    std::string color = constellation_data.find(line_data[i].which_constellation)->second.color;
    int a = -1;
    int b = -1;
    // find the indices of these vertices within the star vector
    for (unsigned int j = 0; j < star_data.size(); j++) {
      if (star_data[j].name == line_data[i].name_a) a = j;
      if (star_data[j].name == line_data[i].name_b) b = j;
    }
    if (a == -1) continue;
    if (b == -1) continue;
    assert (a != -1);
    assert (b != -1);
    // draw the line
    plot_str << "<line x1=\"" << star_data[a].x << "\" y1=\"" << star_data[a].y
             << "\" x2=\""    << star_data[b].x << "\" y2=\"" << star_data[b].y
             << "\" stroke=\"#" << color << "\" stroke-width=\"1\" />" << std::endl;
  }
}

// Draw the stars of the contellations with circles
void draw_stars(std::ostream &plot_str, std::vector<Star_Data> &star_data,
                bool color_stars, bool label_stars,
                const std::map<std::string,Constellation_Data> &constellation_data) {
  for (unsigned int i = 0; i < star_data.size(); i++) {

    // all unknown / unreliable star magnitudes (value is -1) are drawn with radius 2
    float radius = 2;
    if (star_data[i].apparent_magnitude > 0) {
      // scale the radius by the apparent magnitude
      // note: smaller magnitudes are brighter stars (bigger circles)
      radius = 8 / (star_data[i].apparent_magnitude);
      if (radius > 10) radius = 10;
      if (radius < 1) radius = 1;
    }

    // stars will be draw white or using the color of the identified constellation
    std::string star_color = "ffffff";
    if (color_stars && star_data[i].name.substr(0,5) != "STAR_") {
      star_color = constellation_data.find(star_data[i].which_constellation)->second.color;
    }

    // draw the circle
    plot_str << "<circle cx = \"" << star_data[i].x
             << "\" cy = \"" << star_data[i].y
             << "\" r = \"" << radius
             << "\" style=\"fill:#"+star_color+"\" />" << std::endl;

    // if requested, also write the star's name
    if (label_stars && star_data[i].name.substr(0,5) != "STAR_") {
      std::string color = constellation_data.find(star_data[i].which_constellation)->second.color;
      plot_str << "<text x = \"" << star_data[i].x + 1
               << "\" y = \"" << star_data[i].y - 4
               << "\" font-size=\"15\"  fill=\"#" << color << "\">"  << star_data[i].name << "</text>" << std::endl;
    }
  }
}

// Write the name of the constellation beneath the stars in this constellation
void label_constellations(std::ostream &plot_str, 
                          const std::map<std::string,Constellation_Data> &constellation_data) {
  for (std::map<std::string,Constellation_Data>::const_iterator itr = constellation_data.begin();
       itr != constellation_data.end(); itr++) {
    
    std::string color = itr->second.color;
    plot_str << "<text x = \"" << (itr->second.min_x + itr->second.max_x)/2.0 - 20
             << "\" y = \"" << (itr->second.min_y + itr->second.max_y)/2.0 
             << "\" font-size=\"25\"  fill=\"#" << color << "\">"  << itr->first << "</text>" << std::endl;
  }
}

void draw_end(std::ostream &ostr) {
  ostr << "</svg>" << std::endl;
}


// =================================================================


// helper function to assign each constellation that is found in the file with a different color
std::string get_next_color() {
  
  std::vector<std::string> colors;
  colors.push_back("ff0000"); // red
  colors.push_back("00ff00"); // green
  colors.push_back("6666ff"); // blue
  colors.push_back("ffff00"); // yellow
  colors.push_back("00ffff"); // cyan
  colors.push_back("ff00ff"); // magenta
  colors.push_back("ff8800"); // orange
  colors.push_back("88ff00"); // 
  colors.push_back("00ff88"); //
  colors.push_back("0088ff"); // 
  colors.push_back("ff0088"); // 
  colors.push_back("8800ff"); // 
  // note: if there are more than 12 constellations, we'll draw two or
  // more constellations with the same color.

  static int which = -1;
  which++;
  which %= colors.size();
  return colors[which];
}


// =================================================================

int main(int argc, char* argv[]) {

  // ======================================================================
  // PARSE COMMAND LINE ARGUMENTS
  //
  // This program expects 2 arguments, the input filename and the
  // output filename
  assert (argc >= 3);

  std::string input_filename;
  std::string output_filename;

  bool draw_diagram = false;
  bool label_stars = false;
  bool color_stars = false;
  bool label_constellation = false;
  
  for (int i = 1; i < argc; i++) {
    if (argv[i] == std::string("--draw_diagram")) {
      draw_diagram = true;
    } else if (argv[i] == std::string("--label_stars")) {
      label_stars = true;
    } else if (argv[i] == std::string("--color_stars")) {
      color_stars = true;
    } else if (argv[i] == std::string("--label_constellation")) {
      label_constellation = true;
    } else if (input_filename == "") {
      input_filename = argv[i];
    } else {
      assert (output_filename == "");
      output_filename = argv[i];
    }
  }

  // ======================================================================
  // LOAD THE INPUT CONSTELLATION FILE DATA
  //
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
      if (sd.which_constellation != "Unknown") {

        std::map<std::string,Constellation_Data>::iterator itr = constellation_data.find(sd.which_constellation);
        if (itr == constellation_data.end()) {
          // create a new constellation object with a new color
          Constellation_Data &cd = constellation_data[sd.which_constellation];
          cd.min_x = sd.x;
          cd.min_y = sd.y;
          cd.max_x = sd.x;
          cd.max_y = sd.y;
          cd.color = get_next_color();
        } else {
          // keep track of the lower left corner of the constellation
          itr->second.min_x = std::min(itr->second.min_x,sd.x);
          itr->second.min_y = std::min(itr->second.min_y,sd.y);
          itr->second.max_x = std::max(itr->second.max_x,sd.x);
          itr->second.max_y = std::max(itr->second.max_y,sd.y);
        }
      }
      star_data.push_back(sd);
    }

    // read each line segment
    else if (token == "line") {
      Line_Data ld;
      istr >> ld.which_constellation >> ld.name_a >> ld.name_b;
      line_data.push_back(ld);
    }

    else {
      assert(0);
    }
  }

  // ============================================================
  // PLOT THE DATA
  std::ofstream plot_str(output_filename);
  assert (plot_str.good());
  draw_begin(plot_str);
  if (draw_diagram) {
    draw_lines(plot_str,star_data,line_data,constellation_data);
  }
  if (label_constellation) {
    label_constellations(plot_str,constellation_data);
  }
  draw_stars(plot_str,star_data,color_stars,label_stars,constellation_data);
  draw_end(plot_str);
}

// =================================================================

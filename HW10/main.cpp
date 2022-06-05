#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include <map>

#include "image.cpp"
#include "priority_queue.h"

// ===================================================================================================

// distance field method functions
double NaiveDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image);
double ImprovedDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image);
double FastMarchingMethod(Image<Color> &input, Image<DistancePixel> &distance_image);

// visualization style helper functions
Color Rainbow(double distance, double max_distance);
Color GreyBands(double distance, double max_distance, int num_bands);

// ===================================================================================================

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " input.ppm output.ppm distance_field_method visualization_style" << std::endl;
    exit(1);
  }

  // open the input image
  Image<Color> input;
  if (!input.Load(argv[1])) {
    std::cerr << "ERROR: Cannot open input file: " << argv[1] << std::endl;
    exit(1);
  }

  // a place to write the distance values
  Image<DistancePixel> distance_image;
  distance_image.Allocate(input.Width(),input.Height());

  // calculate the distance field (each function returns the maximum distance value)
  double max_distance = 0;
  if (std::string(argv[3]) == std::string("naive_method")) {
    max_distance = NaiveDistanceFieldMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("improved_method")) {
    max_distance = ImprovedDistanceFieldMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("pq_with_map")) {
    max_distance = FastMarchingMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("pq_with_hash_table")) {
    // EXTRA CREDIT: implement FastMarchingMethod with a hash table
  } else {
    std::cerr << "ERROR: Unknown distance field method: " << argv[3] << std::endl;
    exit(1);
  }

  // convert distance values to a visualization
  Image<Color> output;
  output.Allocate(input.Width(),input.Height());
  for (int i = 0; i < input.Width(); i++) {
    for (int j = 0; j < input.Height(); j++) {
      double v = distance_image.GetPixel(i,j).getValue();
      if (std::string(argv[4]) == std::string("greyscale")) {
	output.SetPixel(i,j,GreyBands(v,max_distance*1.01,1));
      } else if (std::string(argv[4]) == std::string("grey_bands")) {
	output.SetPixel(i,j,GreyBands(v,max_distance,4));
      } else if (std::string(argv[4]) == std::string("rainbow")) {
	output.SetPixel(i,j,Rainbow(v,max_distance));
      } else {
	// EXTRA CREDIT: create other visualizations 
	std::cerr << "ERROR: Unknown visualization style" << std::endl;
	exit(0);
      }
    }
  }
  // save output
  if (!output.Save(argv[2])) {
    std::cerr << "ERROR: Cannot save to output file: " << argv[2] << std::endl;
    exit(1);
  }
  return 0;
}

// ===================================================================================================

double NaiveDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
  int w = input.Width();
  int h = input.Height();
  // return the maximum distance value
  double answer = 0;
  // loop over the pixels in the input image
  for (int i = 0; i < w; i++)  {
    for (int j = 0; j < h; j++) {
      double closest = -1;      
      // loop over all other pixels in the input image
      for (int i2 = 0; i2 < w; i2++)  {
        for (int j2 = 0; j2 < h; j2++) {
          const Color& c = input.GetPixel(i2,j2);      
          // skip all pixels that are not black
          if (!c.isBlack()) continue;
          // calculate the distance between the two pixels
          double distance = sqrt((i-i2)*(i-i2) + (j-j2)*(j-j2));
          // store the closest distance to a black pixel
          if (closest < 0 || distance < closest) {
            closest = distance;
          }
        }
      }
      assert (closest >= 0);
      answer = std::max(answer,closest);
      // save the data to the distance image
      DistancePixel& p = distance_image.GetPixel(i,j);
      p.setValue(closest);
    }
  }
  return answer;
}


double ImprovedDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {

  int w = input.Width();
  int h = input.Height();

  std::vector<std::pair<int,int>> black_pixels;
  // store all the black pixels in a vector
  double answer = 0;
  for (int i = 0; i < w; i++){
    for (int j = 0; j < h; j++){
      const Color& c = input.GetPixel(i,j);
      if (!c.isBlack()) continue;
      black_pixels.push_back(std::make_pair(i,j));
    }
  }
  //use a double for loop to loop over all the pixels and compare 
  //the distance to the black pixels
  for (int i = 0; i < w; i++){
    for (int j = 0; j < h; j++){
      double closest = -1;
      for (int k = 0; k < int(black_pixels.size()); k++){
        int i2 = black_pixels[k].first;
        int j2 = black_pixels[k].second;
        double distance = sqrt((i-i2)*(i-i2) + (j-j2)*(j-j2));
        if (closest < 0 || distance < closest){
          closest = distance;
        }
      }
      assert (closest >= 0);
      answer = std::max(answer,closest);
      // save the data to the distance image
      DistancePixel& p = distance_image.GetPixel(i,j);
      p.setValue(closest);
    }
  }
  return answer;
}

// do not go over every pixel
double FastMarchingMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {

  int w = input.Width();
  int h = input.Height();

  std::vector<std::pair<int,int>> black_pixels;
  DistancePixel_PriorityQueue queue;
  // store all the black pixels in a vector
  double answer = 0;
  for (int i = 0; i < w; i++){
    for (int j = 0; j < h; j++){
      const Color& c = input.GetPixel(i,j);
      if (!c.isBlack()) continue;
      DistancePixel& p = distance_image.GetPixel(i,j);
      p.setValue(0);
      black_pixels.push_back(std::make_pair(i,j));
    }
  }

  // find all the pixels that are next to black pixels and add them to the queue,
  // and set their color to blue.
  for (int i = 0; i < int(black_pixels.size()); i++){
    std::pair<int,int> pair = black_pixels[i];
    for(int j = pair.first -1; j < pair.first+1; j++){
      for(int k = pair.second-1; k < pair.second+1; k++){
        //make sure the pixel is in the image
        if(j < 0 || j >= h || k < 0 || k >= w){
          continue;
        }
        //make sure the pixel is not the same pixel
        if(j == pair.first && k == pair.second){
          continue;
        }
        //make sure the pixel is not black
        if(input.GetPixel(j,k).isBlack()){
          continue;
        }
        //make sure the pixel hasn't been visited
        if(input.GetPixel(j,k).isBlue()){
          continue;
        }
        DistancePixel& p = distance_image.GetPixel(j,k);
        p.setX(j);
        p.setY(k);
        input.SetPixel(j,k,Color(0,0,255));
        double distance = 0;
        // calculate the distance to the black pixel,if orthogonal,the distance is 1,else
        // the distance is sqrt(2)
        if(pair.first == j || pair.second == k){
          distance = 1;
        }
        else{
          distance = sqrt(2);
        }
        if(distance + p.getValue() < answer){
          answer = distance + p.getValue();
        }
        p.setValue(distance);
        queue.push(&p);
      }
    }
  }

  // ============================================================================================
  
  // use a while loop to loop over the queue, and pop the front pixel, and set its color to blue.
  while (queue.empty() == false){
    const DistancePixel* p = queue.top();
    queue.pop();
    answer = p->getValue();
    int x = p->getX();
    int y = p->getY();
    DistancePixel& newP = distance_image.GetPixel(x,y);
    for(int i = x-1; i < x+1; i++){
      for(int j = y-1; j < y+1; j++){
        //make sure the pixel is in the image
        
        if(i < 0 || i > h || j < 0 || j > w){
          continue;
        }
        //make sure the pixel is not the same pixel
        if(i == x && j == y){
          continue;
        }
        //make sure the pixel is not black
        if(input.GetPixel(i,j).isBlack()){
          continue;
        }
        // if we have marked this pixel as blue, then we need to make sure this pixel already
        // has a shorter distance to the black pixels. And if not, we need to set some value to
        // this pixel.
        if(input.GetPixel(i,j).isBlue()){
          double distance = 0;
          // calculate the distance to the black pixel,if orthogonal,the distance is 1,else
          // the distance is sqrt(2)
          if(i == x && j == y){
            distance = 1;
          }
          else{
            distance = sqrt(2);
          }
          double distance_bewteen_pixels = p->getValue() + distance;
          if(distance_bewteen_pixels < newP.getValue()){
            newP.setValue(distance_bewteen_pixels);
            queue.update_position(&newP);
          }
        }
      }
    }
  }
  
  //============================================================================================= 
  return answer;
}
// ===================================================================================================

Color Rainbow(double distance, double max_distance) {
  Color answer;
  if (distance < 0.001) {
    // black
    answer.r = 0; answer.g = 0; answer.b = 0;
  } else if (distance < 0.2*max_distance) {
    // blue -> cyan
    double tmp = distance * 5.0 / max_distance;
    answer.r = 0;
    answer.g = tmp*255;
    answer.b = 255;
  } else if (distance < 0.4*max_distance) {
    // cyan -> green
    double tmp = (distance-0.2*max_distance) * 5.0 / max_distance;
    answer.r = 0;
    answer.g = 255;
    answer.b = (1-tmp*tmp)*255;
  } else if (distance < 0.6*max_distance) {
    // green -> yellow
    double tmp = (distance-0.4*max_distance) * 5.0 / max_distance;
    answer.r = sqrt(tmp)*255;
    answer.g = 255;
    answer.b = 0;
  } else if (distance < 0.8*max_distance) {
    // yellow -> red
    double tmp = (distance-0.6*max_distance) * 5.0 / max_distance;
    answer.r = 255;
    answer.g = (1-tmp*tmp)*255;
    answer.b = 0;
  } else if (distance < max_distance) {
    // red -> white
    double tmp = (distance-0.8*max_distance) * 5.0 / max_distance;
    answer.r = 255;
    answer.g = tmp*255;
    answer.b = tmp*255;
  } else {
    // white
    answer.r = answer.g = answer.b = 255;
  }  
  return answer;
}

Color GreyBands(double distance, double max_value, int num_bands) {
  Color answer;
  if (distance < 0.001) {
    // red
    answer.r = 255; answer.g = 0; answer.b = 0;
  } else {
    // shades of grey
    answer.r = answer.g = answer.b = int(num_bands*256*distance/double(max_value)) % 256;
  }  
  return answer;
}

// ===================================================================================================

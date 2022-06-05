#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#include "tile.h"
#include "location.h"
#include "board.h"

using namespace std;

void add_tiles(const vector<Tile*> &t, vector<vector<string> > &his, unsigned int rows,
							unsigned int columns, bool all_solutions, bool allow_rotations);
void add_tiles_sub(Board &b, const vector<Tile*> &t, vector<Location> &l,
									vector<vector<string> > &his, unsigned int loc, bool all_solutions,
									bool allow_rotations, int city_l, int road_l);
bool check_edge(const Board &b, Tile* t, int r, int c);
bool check_loc(const Board &b, Tile* t, int r, int c, int &city, int &road);
bool check_board(const Board &b, vector<vector<string> > &s);

int mod(int a, int b){
	// Return the mod of two integer
	assert(b != 0);
	return (a-a/b*b);
}

// this global variable is set in main.cpp and is adjustable from the command line
// (you are not allowed to make your own global variables)
int GLOBAL_TILE_SIZE = 11;

// ==========================================================================
// Helper function that is called when an error in the command line
// arguments is detected.
void usage(int argc, char *argv[]) {
  cerr << "USAGE: " << endl;
  cerr << "  " << argv[0] << " <filename>  -board_dimensions <h> <w>" << endl;
  cerr << "  " << argv[0] << " <filename>  -board_dimensions <h> <w>  -all_solutions" << endl;
  cerr << "  " << argv[0] << " <filename>  -board_dimensions <h> <w>  -allow_rotations" << endl;
  cerr << "  " << argv[0] << " <filename>  -all_solutions  -allow_rotations" << endl;
  cerr << "  " << argv[0] << " <filename>  -tile_size <odd # >= 11>" << endl;
  exit(1);
}

// ==========================================================================
void HandleCommandLineArguments(int argc, char *argv[], string &filename,
                                unsigned int &rows, unsigned int &columns,
																bool &all_solutions, bool &allow_rotations) {

  // Must at least put the filename on the command line
  if (argc < 2) {
    usage(argc,argv);
  }
  filename = argv[1];

  // Parse the optional arguments
  for (int i = 2; i < argc; i++) {
    if (argv[i] == string("-tile_size")) {
      i++;
      assert (i < argc);
      GLOBAL_TILE_SIZE = atoi(argv[i]);
      if (GLOBAL_TILE_SIZE < 11 || GLOBAL_TILE_SIZE % 2 == 0) {
        cerr << "ERROR: bad tile_size" << endl;
        usage(argc,argv);
      }
    } else if (argv[i] == string("-all_solutions")) {
      all_solutions = true;
    } else if (argv[i] == string("-board_dimensions")) {
      i++;
      assert (i < argc);
      rows = atoi(argv[i]);
      i++;
      assert (i < argc);
      columns = atoi(argv[i]);
      if (rows < 1 || columns < 1) {
        usage(argc,argv);
      }
    } else if (argv[i] == string("-allow_rotations")) {
      allow_rotations = true;
    } else {
      cerr << "ERROR: unknown argument '" << argv[i] << "'" << endl;
      usage(argc,argv);
    }
  }
}

// ==========================================================================
void ParseInputFile(int argc, char *argv[], const string &filename, vector<Tile*> &tiles) {

  // Open the file
  ifstream istr(filename.c_str());
  if (!istr) {
    cerr << "ERROR: cannot open file '" << filename << "'" << endl;
    usage(argc,argv);
  }
  assert (istr);

  // Read each line of the file
  string token, north, east, south, west;
  while (istr >> token >> north >> east >> south >> west) {
    assert (token == "tile");
    Tile *t = new Tile(north,east,south,west);
    tiles.push_back(t);
  }
}

// ==========================================================================
int main(int argc, char *argv[]) {
   string filename;
   unsigned int rows = 0;
   unsigned int columns = 0;
   bool all_solutions = false;
   bool allow_rotations = false;
   HandleCommandLineArguments(argc, argv, filename, rows, columns, all_solutions,
		 													allow_rotations);

   // Load in the tiles
   vector<Tile*> tiles;
   ParseInputFile(argc,argv,filename,tiles);
   // Set the demension if it's not given
   if (rows == 0 && columns == 0){
	   rows = tiles.size();
	   columns = tiles.size();
   }
   // Confirm the specified board is large enough
   if (rows < 1  ||  columns < 1  ||  rows * columns < tiles.size()) {
	   cerr << "ERROR: specified board is not large enough" << rows << "X"
		 			<< columns << "=" << rows*columns << " " << tiles.size() << endl;
	   usage(argc,argv);
   }
   // Create a vector for map history
   vector<vector<string> > his;
   // Optimize for board dimension
   if (rows + columns > tiles.size()+1){
	   if (rows > tiles.size())
		   rows = tiles.size();
	   if (columns > tiles.size())
		   columns = tiles.size();
	   unsigned int r, c;
	   if (rows > columns){
		   r = rows;
		   c = columns;
	   }
	   else{
		   r = columns;
		   c = rows;
	   }
	   // Instead of adding tiles to a larger board, add them to several small board
	   while (c >= (tiles.size()+1-r) && r >= (tiles.size()+1-r)){
		   add_tiles(tiles, his, r, tiles.size()+1-r, all_solutions, allow_rotations);
		   r--;
	   }
   }
   else
	   // Add tiles using recursion
	   add_tiles(tiles, his, rows, columns, all_solutions, allow_rotations);
   // Print number of solutions
   if (his.size() == 0)
		cout << "No Solutions." << endl;
   else if (all_solutions)
		cout << "Found  " << his.size() << " Solution(s)." << endl;
   // Delete the tiles
   for (unsigned int t = 0; t < tiles.size(); t++) {
	   delete tiles[t];
   }
}

// ==========================================================================
void add_tiles(const vector<Tile*> &t, vector<vector<string> > &his, unsigned int rows,
							unsigned int columns, bool all_solutions, bool allow_rotations){
	// Create a new board
	// P.S. Both row number and column number are increased by 2 in the board.
	// But any tiles won't be added to the edge of the board. The edge of the
	// board will always keep NULL. In this way, special case for edge is no
	// longer needed to be consider in further coding.
	Board b(rows+2,columns+2);
	// Create a vector for the tile location
	vector<Location> l;
	// Check the total number of cities and roads of all tiles
	int city_l = 0, road_l = 0;
	for (unsigned int n = 0; n < t.size(); ++n){
		city_l += t[n]->numCities();
		road_l += t[n]->numRoads();
	}
	// Find the the number of roads and cities left
	city_l -= 2*t[0]->numCities();
	road_l -= 2*t[0]->numRoads();
	// Check whether need rotations or not
	int rota = 1, rsize = rows, csize = columns;
	if (allow_rotations){
		rota = 4;
		if (t[0]->numCities() == 4 || t[0]->numRoads() == 4)
			rota = 1;
		// If rotate, the first tile will only be put at the north-west of the board
		rsize = ceil(rows/2.);
		if (rows == columns)
			csize = ceil(columns/2.);
	}
	for (int deg = 0; deg < rota; ++deg){
		for (int r = 1; r <= rsize; ++r){
			for (int c = 1; c <= csize; ++c){
				if (check_edge(b, t[0], r, c)){
					b.setTile(r, c, t[0]);
					l.push_back(Location(r-1,c-1,deg*90));
					// Add following tiles using recursion
					add_tiles_sub(b, t, l, his, 1, all_solutions, allow_rotations, city_l, road_l);
					// Change the board back after recursion. Make sure the next recursion
					//has the same board
					b.setNULL(r, c);
					l.pop_back();
				}
			}
		}
		if (rota == 4)
			t[0]->rotate();
	}
}

void add_tiles_sub(Board &b, const vector<Tile*> &t, vector<Location> &l,
									vector<vector<string> > &his, unsigned int loc, bool all_solutions,
									bool allow_rotations, int city_l, int road_l){
	// Check the end point
	if (loc < t.size() && (all_solutions || his.size() == 0)){
		// Check whether need rotations or not
		int rota = 1;
		if (allow_rotations && t[loc]->numCities() != 4 && t[loc]->numRoads() != 4)
			rota = 4;
		// Minus the number of city and road left in the vector
		city_l -= 2*t[loc]->numCities();
		road_l -= 2*t[loc]->numRoads();
		int cc = city_l, rr = road_l;
		// Try to add the current tile to every location
		for (int deg = 0; deg < rota; ++deg){
			for (int r = 1; r < b.numRows()-1; ++r){
				for (int c = 1; c < b.numColumns()-1; ++c){
					// Check whether the current location is an empty location
					if (b.getTile(r,c) == NULL){
						// Check whether the previous tile's edge match the current edge
						//If appropriate, add the tile to the board, and continue the recursion
						if (check_edge(b, t[loc], r, c)){
							if (check_loc(b, t[loc], r, c, city_l, road_l)){
								// Check whether there are enough cities and roads left
								if (city_l >= 0 && road_l >= 0){
									b.setTile(r, c, t[loc]);
									l.push_back(Location(r-1,c-1,deg*90));
									add_tiles_sub(b, t, l, his, loc+1, all_solutions, allow_rotations,
																city_l, road_l);
									// Change the board back after recursion. Make sure the next
									// recursion has the same board
									b.setNULL(r, c);
									l.pop_back();
								}
							}
							city_l = cc;
							road_l = rr;
						}
					}
				}
			}
			if (rota == 4)
				t[loc]->rotate();
		}
	}
	else{
		// Check whether the board is set up correctly
		bool correct = check_board(b, his);
		// Print the board if needed
		if (correct && (all_solutions || his.size() == 1)){
			cout << "Solution: ";
			for (unsigned int i = 0; i < l.size(); i++) {
				cout << l[i];
			}
			cout << endl;
			b.PrintSmall();
			cout << endl;
		}
	}
}

bool check_edge(const Board &b, Tile* t, int r, int c){
	// Check whether the tile can locate at the edge of the board (Optimization for run time)
	if (r == 1 && t->getNorth() != 'p')
		return false;
	else if (r == (b.numRows()-2) && t->getSouth() != 'p')
		return false;
	else if (c == 1 && (t->getWest() != 'p'))
		return false;
	else if (c == b.numColumns()-2 && t->getEast() != 'p')
		return false;
	else
		return true;
}

bool check_loc(const Board &b, Tile* t, int r, int c, int &city, int &road){
	// Check whether the location touch other tiles first, then check whether the
	// edges match each other. If the edge touches, add the number of cities and
	// road back
	bool touch = false, right_edge = true;
	if (b.getTile(r,c-1) != NULL){
		touch = true;
		if (b.getTile(r,c-1)->getEast() != t->getWest())
			right_edge = false;
		else if(b.getTile(r,c-1)->getEast() == 'c')
			city += 2;
		else if(b.getTile(r,c-1)->getEast() == 'r')
			road += 2;
	}
	if (b.getTile(r,c+1) != NULL){
		touch = true;
		if (b.getTile(r,c+1)->getWest() != t->getEast())
			right_edge = false;
		else if(b.getTile(r,c+1)->getWest() == 'c')
			city += 2;
		else if(b.getTile(r,c+1)->getWest() == 'r')
			road += 2;
	}
	if (b.getTile(r-1,c) != NULL){
		touch = true;
		if (b.getTile(r-1,c)->getSouth() != t->getNorth())
			right_edge = false;
		else if(b.getTile(r-1,c)->getSouth() == 'c')
			city += 2;
		else if(b.getTile(r-1,c)->getSouth() == 'r')
			road += 2;
	}
	if (b.getTile(r+1,c) != NULL){
		touch = true;
		if (b.getTile(r+1,c)->getNorth() != t->getSouth())
			right_edge = false;
		else if(b.getTile(r+1,c)->getNorth() == 'c')
			city += 2;
		else if(b.getTile(r+1,c)->getNorth() == 'r')
			road += 2;
	}
	return touch && right_edge;
}

bool check_board(const Board &b, vector<vector<string> > &his){
	// Check if the board has been correctly set up
	for (int r = 1; r < b.numRows()-1; ++r){
		for (int c = 1; c < b.numColumns()-1; ++c){
			if (b.getTile(r,c) != NULL){
				// Check whether the map has a correct edge
				if (b.getTile(r,c-1) == NULL && b.getTile(r,c)->getWest() != 'p')
					return false;
				if (b.getTile(r,c+1) == NULL && b.getTile(r,c)->getEast() != 'p')
					return false;
				if (b.getTile(r-1,c) == NULL && b.getTile(r,c)->getNorth() != 'p')
					return false;
				if (b.getTile(r+1,c) == NULL && b.getTile(r,c)->getSouth() != 'p')
					return false;
			}
		}
	}
	// Create a pattern for the board
	vector<string> pa(4,"");
	int r_beg, r_end, c_beg, c_end;
	b.begin_end(r_beg, r_end, c_beg, c_end);
	for (int r = r_beg; r < r_end; ++r){
		for (int c = c_beg; c < c_end; ++c){
			// Add the first letter of the type name to the pattern
			if (b.getTile(r, c) != NULL)
				pa[0] += b.getTile(r, c)->getNorth();
			// Add 0 if it's NULL
			else
				pa[0] += "0";
			// Add the first letter of the type name to the pattern
			if (b.getTile(r_end+r_beg-r-1, c_end+c_beg-c-1) != NULL)
				pa[2] += b.getTile(r_end+r_beg-r-1, c_end+c_beg-c-1)->getSouth();
			// Add 0 if it's NULL
			else
				pa[2] += "0";
		}
	}
	for (int c = c_beg; c < c_end; ++c){
		for (int r = r_beg; r < r_end; ++r){
			// Add the first letter of the type name to the pattern
			if (b.getTile(r, c_end+c_beg-c-1) != NULL)
				pa[1] += b.getTile(r, c_end+c_beg-c-1)->getEast();
			// Add 0 if it's NULL
			else
				pa[1] += "0";
			// Add the first letter of the type name to the pattern
			if (b.getTile(r_end+r_beg-r-1, c) != NULL)
				pa[3] += b.getTile(r_end+r_beg-r-1, c)->getWest();
			// Add 0 if it's NULL
			else
				pa[3] += "0";
		}
	}
	// Check whether the same pattern has been created previously
	for (unsigned int n = 0; n < his.size(); ++n){
		// Check with different rotation
		for (int deg = 0; deg < 4; ++deg){
			bool had = true;
			if (his[n][0] != pa[mod(deg, 4)])
				had = false;
			else if (his[n][1] != pa[mod(1+deg, 4)])
				had = false;
			else if (his[n][2] != pa[mod(2+deg, 4)])
				had = false;
			else if (his[n][3] != pa[mod(3+deg, 4)])
				had = false;
			// Return false if the pattern had before
			if (had)
				return false;
		}
	}
	his.push_back(pa);
	return true;
}
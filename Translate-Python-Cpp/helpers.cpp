/**
	helpers.cpp
	Purpose: helper functions which are useful when
	implementing a 2-dimensional histogram filter.
	This file is incomplete! Your job is to make the
	normalize and blur functions work. Feel free to 
	look at helper.py for working implementations 
	which are written in python.
*/

#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream> 
//#include "debugging_helpers.cpp"

using namespace std;

 typedef vector < vector <float> > float_global;
 typedef vector< vector <char> > char_global;



/**
    Creates a grid of zeros
    For example:
    zeros(2, 3) would return
    0.0  0.0  0.0
    0.0  0.0  0.0
    @param height - the height of the desired grid
    @param width - the width of the desired grid.
    @return a grid of zeros (floats)
*/
float_global zeros(int height, int width) {
  
 //refered to the alternate discussion mentioned in the c++ vectors lecture
 float_global grid_n(height, vector <float>(width,0));
 return grid_n;
}

/**
	TODO - implement this function
    Normalizes a grid of numbers. 
    @param grid - a two dimensional grid (vector of vectors of floats)
		   where each entry represents the unnormalized probability 
		   associated with that grid cell.
    @return - a new normalized two dimensional grid where the sum of 
    	   all probabilities is equal to one.
*/
float_global normalize(float_global grid) {
	// todo - your code here

    
	float sum=0.0;
  
	int row = grid.size();
    int col = grid[0].size();
  

	for(int i=0;i<row;i++)    //postfix
	{
		for(int j=0;j<col;j++)   
		sum=sum+grid[i][j];
	}
	
	for(int i=0;i<row;++i)   //prefix
	{
		for(int j=0;j<col;++j)
		grid[i][j]=grid[i][j]/sum;
	}

	return grid;
}




/**
	TODO - implement this function.
    Blurs (and normalizes) a grid of probabilities by spreading 
    probability from each cell over a 3x3 "window" of cells. This 
    function assumes a cyclic world where probability "spills 
    over" from the right edge to the left and bottom to top. 
    EXAMPLE - After blurring (with blurring=0.12) a localized 
    distribution like this:
    0.00  0.00  0.00 
    0.00  1.00  0.00
    0.00  0.00  0.00 
    would look like this:
	
	0.01  0.02	0.01
	0.02  0.88	0.02
	0.01  0.02  0.01
    @param grid - a two dimensional grid (vector of vectors of floats)
		   where each entry represents the unnormalized probability 
		   associated with that grid cell.
	@param blurring - a floating point number between 0.0 and 1.0 
		   which represents how much probability from one cell 
		   "spills over" to it's neighbors. If it's 0.0, then no
		   blurring occurs. 
    @return - a new normalized two dimensional grid where probability 
    	   has been blurred.
*/
float_global blur(float_global grid, float blurring) {

	// your code here
	int i,j,dx,dy;
	int new_i, new_j;
	float multiplier;
  
    vector<int> list = {-1,0,1};
	
	int row = grid.size();
	int col = grid[0].size();
  
    float_global grid_blur = zeros(row,col);
	
	float center_prob = 1.0-blurring;
	float corner_prob = blurring/12.0;
	float adjacent_prob = blurring/6.0;
     
	
    float_global window;
    vector<float> first_row;
    vector<float> second_row;
    vector<float> third_row;
    first_row.push_back(corner_prob);
    first_row.push_back(adjacent_prob);
    first_row.push_back(corner_prob);
    second_row.push_back(adjacent_prob);
    second_row.push_back(center_prob);
    second_row.push_back(adjacent_prob);
    third_row.push_back(corner_prob);
    third_row.push_back(adjacent_prob);
    third_row.push_back(corner_prob);
    window.push_back(first_row);
    window.push_back(second_row);
    window.push_back(third_row);
  
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			float value = grid[i][j];
          //Refered to https://www.cprogramming.com/c++11/c++11-ranged-for-loop.html
			for (auto dx:list) 
			{
				for (auto dy:list) 
				{
					multiplier = window[dx+1][dy+1];	
					new_i = (((i + dy) % row) + row) % row;
					new_j = (((j + dx) % col) + col) % col;
					grid_blur[new_i][new_j] += value*multiplier;
				}
			}
		}
	}
	
	return normalize(grid_blur);
}

/** -----------------------------------------------
#
#
#	You do not need to modify any code below here.
#
#
# ------------------------------------------------- */


/**
    Determines when two grids of floating point numbers 
    are "close enough" that they should be considered 
    equal. Useful for battling "floating point errors".
    @param g1 - a grid of floats
    
    @param g2 - a grid of floats
    @return - A boolean (True or False) indicating whether
    these grids are (True) or are not (False) equal.
*/
bool close_enough(float_global g1, float_global g2) {
	int i, j;
	float v1, v2;
	if (g1.size() != g2.size()) {
		return false;
	}

	if (g1[0].size() != g2[0].size()) {
		return false;
	}
	for (i=0; i<g1.size(); i++) {
		for (j=0; j<g1[0].size(); j++) {
			v1 = g1[i][j];
			v2 = g2[i][j];
			if (abs(v2-v1) > 0.0001 ) {
				return false;
			}
		}
	}
	return true;
}

bool close_enough(float v1, float v2) { 
	if (abs(v2-v1) > 0.0001 ) {
		return false;
	} 
	return true;
}

/**
    Helper function for reading in map data
    @param s - a string representing one line of map data.
    @return - A row of chars, each of which represents the
    color of a cell in a grid world.
*/
vector <char> read_line(string s) {
	vector <char> row;

	size_t pos = 0;
	string token;
	string delimiter = " ";
	char cell;

	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		s.erase(0, pos + delimiter.length());

		cell = token.at(0);
		row.push_back(cell);
	}

	return row;
}

/**
    Helper function for reading in map data
    @param file_name - The filename where the map is stored.
    @return - A grid of chars representing a map.
*/
char_global read_map(char file_name[]) {

	ifstream infile(file_name);
	char_global map;
	if (infile.is_open()) {

		char color;
		vector <char> row;
		
		string line;

		while (std::getline(infile, line)) {
			row = read_line(line);
			map.push_back(row);
		}
	}
	return map;
}



//int main() {
	
//	vector < vector < char > > map = read_map("maps/m1.txt");
//	show_grid(map);
//	return 0;
//}
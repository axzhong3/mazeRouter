/**
 *	SquareMaze class
 */

#include <vector>
#include <stack>
#include "easypng.h"
#include "dsets.h"
#include "animation.h"

using std::vector;

class SquareMaze
{
	public:
		// SquareMaze constructor
		SquareMaze();

		// SquareMaze copy constructor
		SquareMaze(SquareMaze const & other);
		
		// SquareMaze destructor
		~SquareMaze();

		// SquareMaze assignment operator		
		SquareMaze & operator=(SquareMaze const & other);

		// Build the unsolved maze
		void makeMaze(int width, int height, int layer);

		// See if one can travel in a specific direction at (x,y)
		bool canTravel(int x, int y, int z, int dir);

		// Set whether or not a wall exists
		void setWall(int x, int y, int dir, bool exists);

		// Solve the maze
		void solveMaze(int source[][2], int dest[][2], int wire_num);

		// Solution of all the directions taken
		vector< vector<int> > solution;

		// Draw the unsolved maze
		PNG * drawMaze(int source[][2], int dest[][2], int wire_num);

		// Draw the solved maze
		PNG * drawMazeWithSolution(int source[][2], int dest[][2], int wire_num);

		// Animate the solved path
		animation motionMazeWithSolution(int source[][2], int dest[][2], int wire_num);
	
	private:
		// Width of maze
		int _width;
		
		// Height of maze
		int _height;

		// Number of metal layers
		int _layer;

		// 2D array saving whether walls removed
		bool ** _walls;

		bool *** _visited;

		// Total wire length
		int _totalWiring;
		
		// DisjointSets storing whether squares connected
		DisjointSets _connection;
		
		// Drawing helper function: draw a specific pixel
		void drawPixel(RGBAPixel * ptr, int r, int g, int b);

		// Draw the pins to be connected		
		void drawPin(PNG * output, int x, int y);

		// Draw the Vias
		void drawVia(PNG * output, int x, int y, int curr_layer);

		// Jump the different layer
		int deltaLayer(int dir);

		// Clear helper function
		void clear();

		// Copy helper function
		void copy(SquareMaze const & other);

};

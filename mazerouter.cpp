/*
 *  testsquaremaze.cpp
 *  cs225 Maze MP
 *  Performs basic tests of SquareMaze.
 *  Written April 2007 by Jonathan Ray
 *
 *  mazerouter.cpp
 *  CS225 Project Portfolio: Multi-layer Maze Router 
 *  Rewritten in May 2012 by Andrew Zhong
 */

#include <iostream>
#include "dsets.h"
#include "maze.h"
#include "easypng.h"
#include "input.cpp"

#define MAXNUM_NUM_GROUPS 1000;
#define MAX_NUM_BOUNDARIES 1000;

using namespace std;

int main()
{ 
	int width, height, layers, wire_num, wall_num;
	int source[1000][2],dest[1000][2],wallTL[1000][2],wallBR[1000][2];
	
	// Setup from "input.txt"
	setup(width, height, layers, source, dest, wire_num, wallTL, wallBR, wall_num);
		
	// Initialize the maze and set boundaries
	SquareMaze m;
    m.makeMaze(width,height,layers);
	for(int num=0;num<wall_num;num++)
	{
		for(int i=wallTL[num][0];i<wallBR[num][0]+1;i++)
		{	
			m.setWall(i,wallTL[num][1]-1,1,1);
			m.setWall(i,wallBR[num][1],1,1);
		}
		for(int i=wallTL[num][1];i<wallBR[num][1]+1;i++)
		{
			m.setWall(wallTL[num][0]-1,i,0,1);
			m.setWall(wallBR[num][0],i,0,1);
		}
	}

	// Draw the original pins and boundaries
    PNG* unsolved = m.drawMaze(source,dest,wire_num);
    unsolved->writeToFile("unsolved.png");
    delete unsolved;
	
	// Solve the maze routing   
    m.solveMaze(source,dest,wire_num);
    cout << "    Wiring finished!" << endl;
	
	// Draw the solved wiring   
    PNG* solved = m.drawMazeWithSolution(source,dest,wire_num);
    solved->writeToFile("solved.png");
    delete solved;
    cout << "    Wiring in .png generated!" << endl;

	// Animate the solving process
	animation anim=m.motionMazeWithSolution(source,dest,wire_num);  
	anim.write("solved.gif");
	cout << "    Wiring in .gif generated!" << endl;  
   
    return 0;
}


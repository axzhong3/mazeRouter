/**
 * @file premade_graphs.cpp
 *
 * This file contains the implementation of the premade graphs.
 * Updated by Sean Massung for Spring 2012
 *
 * @author Sean Massung
 * @date Spring 2011
 *
 * build_graph.cpp
 * CS225 Project Portfolio: Multi-layer Maze Router
 * Adapted by Andrew Zhong in June 2012
 */

#include "build_graph.h"
#include <cmath>

Graph Build_graph::buildgraph(bool isWeighted, int n, int num_pins,int group[][MAX_NUM_PINS][2])
{
	Graph g(isWeighted);
	Vertex vertices[num_pins];
	for(int i=0;i<num_pins;i++)
		vertices[i]=g.insertVertex("TEMP");
	for(int i=0;i<num_pins;i++)
		for(int j=i+1;j<num_pins;j++)
		{
			g.insertEdge(vertices[i],vertices[j]);
			int diff=abs(group[n][i][0]-group[n][j][0]);
			diff+=abs(group[n][i][1]-group[n][j][1]);
			g.setEdgeWeight(vertices[i],vertices[j],diff);
		}
	return g;
}

/**
 * @file premade_graphs.h
 * 
 * This file builds three example graphs with the Graph library.
 * The examples have real weights between cities in miles.
 * Unfortunately, graphviz draws the nodes with a best-fit algorithm,
 *  so relative locations of cities are wrong.
 * These graphs are also used in *_test.cpp.
 * Updated by Sean Massung for Spring 2012
 *
 * @author Sean Massung
 * @date Spring 2011
 *
 * build_graph.h
 * CS225 Project Portfolio: Multi-layer Maze Router
 * Adapted by Andrew Zhong in June 2012
 */

#ifndef _BUILD_GRAPH_
#define _BUILD_GRAPH_

namespace Build_graph
{

	Graph buildgraph(bool isWeighted, int group_num, int num_pins,int group[][MAX_NUM_PINS][2]);
}

#endif

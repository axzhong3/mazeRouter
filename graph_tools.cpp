/**
 * @file graph_tools.cpp
 * This is where you will implement several functions that operate on graphs.
 * Be sure to thoroughly read the comments above each function, as they give
 *  hints and instructions on how to solve the problems.
 *  Written by Andrew Zhong in May 2012
 */

#include "graph_tools.h"

/**
 * Returns the shortest distance (in edges) between the Vertices
 *  start and end.
 * THIS FUNCTION IS GRADED.
 *
 * @param graph - the graph to search
 * @param start - the vertex to start the search from
 * @param end - the vertex to find a path to
 * @return the minimum number of edges between start and end
 *
 * @todo Label each edge "MINPATH" if it is part of the minimum path
 *
 * @note Remember this is the shortest path in terms of edges,
 *  not edge weights.
 * @note Again, you may use the STL stack and queue.
 * @note You may also use the STL's unordered_map, but it is possible
 *  to solve this problem without it.
 *
 * @hint In order to draw (and correctly count) the edges between two
 *  vertices, you'll have to remember each vertex's parent somehow.
 */
int GraphTools::findShortestPath(Graph & graph, Vertex start, Vertex end)
{	
	queue<Vertex> q;
	unordered_map<Vertex,Vertex> p;
	unordered_map<Vertex,int> d;
	unordered_map<Vertex,bool> label_v;
	
	// initialize labels of all the vertices	
	vector<Vertex> all_v=graph.getVertices();
	for(vector<Vertex>::iterator it=all_v.begin();it<all_v.end();it++)
		label_v[*it]=false;
	
	label_v[start]=false;
	q.push(start);
	p[start]=start;
	d[start]=0;
	while(!q.empty())
	{
		Vertex curr=q.front();
		q.pop();
		vector<Vertex> adj_v=graph.getAdjacent(curr);
		for(vector<Vertex>::iterator it=adj_v.begin();it<adj_v.end();it++)
		{
			if(label_v[*it]==false)
			{
				q.push(*it);
				d[*it]=d[curr]+1;
				p[*it]=curr;
				label_v[*it]=true;
				graph.setEdgeLabel(curr,*it,"DISCOVERY");
			}
			else if(graph.getEdgeLabel(curr,*it).empty())
			{
				graph.setEdgeLabel(curr,*it,"CROSS");
			}
			if(*it==end)
			{
				for(Vertex next=*it;next!=start;next=p[next])
				{
					graph.setEdgeLabel(next,p[next],"MINPATH");
				}
				return d[*it];
			}
		}
	}
    return -1;
}

/**
 * Finds the minimum edge weight in the Graph graph.
 * THIS FUNCTION IS GRADED.
 *
 * @param graph - the graph to search
 * @return the minimum weighted edge
 *
 * @todo Label the minimum edge as "MIN". It will appear blue when
 *  graph.savePNG() is called in minweight_test.
 *
 * @note You must do a traversal.
 * @note You may use the STL stack and queue.
 * @note You may assume the graph is connected.
 *
 * @hint Initially label vertices and edges as unvisited.
 */
int GraphTools::findMinWeight(Graph & graph)
{
	queue<Vertex> q;
	unordered_map<Vertex,bool> label_v;
	bool isFirst=true;
	Edge min_e;
	int dis;

	// initialize labels of all the vertices	
	vector<Vertex> all_v=graph.getVertices();
	for(vector<Vertex>::iterator it=all_v.begin();it<all_v.end();it++)
		label_v[*it]=false;

	Vertex start=graph.getStartingVertex();
	label_v[start]=false;
	q.push(start);
	
	while(!q.empty())
	{
		Vertex curr=q.front();
		q.pop();
		vector<Vertex> adj_v=graph.getAdjacent(curr);
		for(vector<Vertex>::iterator it=adj_v.begin();it<adj_v.end();it++)
		{
			if(label_v[*it]==false)
			{
				q.push(*it);
				label_v[*it]=true;
				graph.setEdgeLabel(curr,*it,"DISCOVERY");
				if(isFirst==true)
				{
					min_e=graph.getEdge(curr,*it);
					dis=graph.getEdgeWeight(curr,*it);
					isFirst=false;
				}
				else
				{
					if(dis>graph.getEdgeWeight(curr,*it))
					{
						min_e=graph.getEdge(curr,*it);
						dis=graph.getEdgeWeight(curr,*it);
					}
				}
			}
			else if(graph.getEdgeLabel(curr,*it).empty())
			{
				graph.setEdgeLabel(curr,*it,"CROSS");
				if(isFirst==true)
				{
					min_e=graph.getEdge(curr,*it);
					dis=graph.getEdgeWeight(curr,*it);
					isFirst=false;
				}
				else
				{
					if(dis>graph.getEdgeWeight(curr,*it))
					{
						min_e=graph.getEdge(curr,*it);
						dis=graph.getEdgeWeight(curr,*it);
					}
				}
			}
			
		}
	}
	graph.setEdgeLabel(min_e.source,min_e.dest,"MIN");
	return dis;
}


/**
 * Finds a minimal spanning tree on a graph.
 * THIS FUNCTION IS GRADED.
 *
 * @param graph - the graph to find the MST of
 *
 * @todo Label the edges of a minimal spanning tree as "MST"
 *  in the graph. They will appear blue when graph.savePNG() is called.
 *
 * @note Use your disjoint sets class from MP 7.1 to help you with
 *  Kruskal's algorithm. Copy the files into the libdsets folder.
 * @note You may call std::sort (http://www.cplusplus.com/reference/algorithm/sort/)
 *  instead of creating a priority queue.
 */
void GraphTools::findMST(Graph & graph,int source[][2],int dest[][2],int & wire_num,int n,int num_pins,int group[][MAX_NUM_PINS][2])
{
	vector<Edge> all_e=graph.getEdges();
	vector<Vertex> all_v=graph.getVertices();

	unordered_map<Vertex,int> all_V;
	int size=all_v.size();
	for(int i=0;i<size;i++)
		all_V[all_v[i]]=i;

	sort(all_e.begin(),all_e.end());
	
	DisjointSets vertices;
	vertices.addelements(size);
	int edge_count=0;
	for(vector<Edge>::iterator it=all_e.begin();(it<all_e.end()||edge_count<size-1);it++)
	{
		if(vertices.find(all_V[(*it).source])!=vertices.find(all_V[(*it).dest]))
		{
			vertices.setunion(all_V[(*it).source],all_V[(*it).dest]);
			graph.setEdgeLabel((*it).source,(*it).dest,"MST");
			int source_index=(*it).source;
			int dest_index=(*it).dest;
			source[wire_num][0]=group[n][source_index][0];
			source[wire_num][1]=group[n][source_index][1];
			dest[wire_num][0]=group[n][dest_index][0];
			dest[wire_num][1]=group[n][dest_index][1];
			wire_num++;
			edge_count++;		
		}
	}
}

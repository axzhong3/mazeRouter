/* 
 *  input.cpp
 *  CS225 Project Portfolio: Multi-layer Maze Router 
 *  Written in June 2012 by Andrew Zhong
 */

#define MAX_NUM_GROUP 50
#define MAX_NUM_PINS 20

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include "easypng.h"
#include "graph.h"
#include "build_graph.cpp"
#include "graph_tools.h"


using std::vector;
using std::string;
using std::cerr;
using std::cout;
using std::endl;

void setup(int & width, int & height, int & layers, int source[][2], int dest[][2], int & wire_num, int wallTL[][2], int wallBR[][2], int & wall_num)
{
	wall_num=0;
	wire_num=0;

	int group[MAX_NUM_GROUP][MAX_NUM_PINS][2];
	int num_pins[MAX_NUM_GROUP]={0};
	int num_group=0;
	void quicksort(int num_pins[], int index[], int left, int right);

	string title("default");
	Graph* graph;
	ifstream input;	
	input.open("input.txt");
	char buffer;
	if(input.is_open())
	{
		input.seekg(0,ios::end);
		long end=input.tellg();
		input.seekg(0,ios::beg);
		while(input.tellg()<end-1)
		{
			while(title.compare("Width")!=0)
				input>>title;
			while(input.peek()!=';')
				input>>width;
			while(title.compare("Height")!=0)
				input>>title;
			while(input.peek()!=';')
				input>>height;
			while(title.compare("Number_of_Layers")!=0)
				input>>title;
			while(input.peek()!=';')
				input>>layers;
			while(title.compare("Walls_Top_Left")!=0)
				input>>title;
			while(input.peek()!=';')
			{
				input>>wallTL[wall_num][0];
				input>>wallTL[wall_num][1];
				wall_num++;
			}
			while(title.compare("Walls_Bottom_Right")!=0)
				input>>title;
			int i=0;
			while(input.peek()!=';')
			{
				input>>wallBR[i][0];
				input>>wallBR[i][1];
				i++;
			}
			if(i!=wall_num)
				cout<<"Error: Input to boundaries is invalid"<<endl;

			while(title.compare("Pins")!=0)
				input>>title;
			while(input.peek()!='.')
			{
				while(input.peek()!=';')
				{
					input>>group[num_group][num_pins[num_group]][0];
					input>>group[num_group][num_pins[num_group]][1];
					num_pins[num_group]++;
				}
				num_group++;
				input>>buffer;
			}
			input.seekg(0,ios::end);
		}
		input.close();
	}
	int * index=new int [num_group];
	for(int i=0;i<num_group;i++)
		index[i]=i;
	quicksort(num_pins,index,0,num_group-1);
	for(int i=0;i<num_group;i++)
	{
		graph=new Graph(Build_graph::buildgraph(true,index[i],num_pins[index[i]],group));
		GraphTools::findMST(*graph,source,dest,wire_num,index[i],num_pins[index[i]],group);
		delete graph;
	}
	delete [] index;

	// Interface
	cout<<endl;
	cout<<"----------------------- MAZE ROUTER v2.0 -----------------------"<<endl;	
	cout<<"Input:" <<endl;
	cout<<"    "<<width<<" x "<<height<<" grid"<<endl;
	cout<<"    Total number of metal layers: "<<layers<<endl;
	cout<<"    Total number of groups of connected pins: "<<num_group<<endl;
	cout<<"    Minimum number of wires: "<<wire_num<<endl;
	cout<<endl;
	cout<<"Progress:"<<endl;
}

void quicksort(int num_pins[], int index[], int left, int right)
{
	int i=left;
	int j=right;
	int temp;
	int pivot=num_pins[index[(i+j)/2]];
	while(i<=j)
	{
		while(num_pins[index[i]]<pivot)
			i++;
		while(num_pins[index[j]]>pivot)
			j--;
		if(i<=j)
		{
			temp=index[i];
			index[i]=index[j];
			index[j]=temp;
			i++;
			j--;
		}
	};
	if(left<j)
		quicksort(num_pins,index,left,j);
	if(i<right)
		quicksort(num_pins,index,i,right);
}

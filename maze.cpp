/**
 *	SquareMaze class implementation
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <queue>
#include "maze.h"

using namespace std;

SquareMaze::SquareMaze():_width(0),_height(0),_layer(0),_walls(NULL),_visited(NULL),_totalWiring(0)
{
}

SquareMaze::SquareMaze(SquareMaze const & other)
{
	if(this!=&other)
	{
		clear();
		copy(other);
	}
}

SquareMaze::~SquareMaze()
{
	clear();
}

SquareMaze & SquareMaze::operator=(SquareMaze const & other)
{
	SquareMaze * result=new SquareMaze(other);
	return *result;
}

void SquareMaze::makeMaze(int width, int height, int layer)
{
	_width=width;
	_height=height;
	_layer=layer;
	_totalWiring=0;

	// initialize the walls
	if(_walls!=NULL)
		clear();
	_walls=new bool* [_width];
	for(int i=0;i<_width;i++)
		_walls[i]=new bool [_height*2];
	for(int i=0;i<_width;i++)
		for(int j=0;j<_height*2;j++)
			_walls[i][j]=0;
	for(int i=0;i<_width;i++)
		_walls[i][_height*2-1]=1;
	for(int j=0;j<_height*2;j=j+2)
		_walls[_width-1][j]=1;

	_visited=new bool** [_width];
	for(int i=0;i<_width;i++)
	{
		_visited[i]=new bool* [_height];
		for(int j=0;j<_height;j++)
		{
			_visited[i][j]=new bool [_layer];
			for(int k=0;k<_layer;k++)
				_visited[i][j][k]=0;
		}
	}
}

bool SquareMaze::canTravel(int x, int y, int z, int dir)
{
	switch(dir)
	{
		case 0:
			if((x+1)>=_width)
				return 0;
			if(_walls[x][2*y]==1)
				return 0;
			return 1;
		case 1:
			if((y+1)>=_height)
				return 0;
			if(_walls[x][2*y+1]==1)
				return 0;
			return 1;
		case 2:
			if(x==0)
				return 0;
			if(_walls[x-1][2*y]==1)
				return 0;
			return 1;
		case 3:
			if(y==0)
				return 0;
			if(_walls[x][2*y-1]==1)
				return 0;
			return 1;
		case 4:
			if(z==_layer-1)
				return 0;
			else
				return 1;
		case 5:
			if(z==0)
				return 0;
			else
				return 1;
		default:
			return 0;
	}
}

void SquareMaze::setWall(int x, int y, int dir, bool exists)
{
	if(x<0|| x>=_width || y<0|| y>=_height)
		return;
	if(dir==0)
		_walls[x][2*y]=exists;
	else if(dir==1)
		_walls[x][2*y+1]=exists;
}

void SquareMaze::solveMaze(int source[][2], int dest[][2], int wire_num)
{
	int deltax[6]={1,0,-1,0,0,0};
	int deltay[6]={0,1,0,-1,0,0};
	int deltaz[6]={0,0,0,0,1,-1};
	int *** d=new int ** [_width];
	int *** p=new int ** [_width];
	bool *** visited=new bool ** [_width];
	for(int i=0;i<_width;i++)
	{
		d[i]=new int * [_height];
		p[i]=new int * [_height];
		visited[i]=new bool * [_height];
		for(int j=0;j<_height;j++)
		{
			d[i][j]=new int [_layer];
			p[i][j]=new int [_layer];
			visited[i][j]=new bool [_layer];
		}
	}

	for(int wire=0;wire<wire_num;wire++)
	{
		
		queue<int> qx;
		queue<int> qy;
		queue<int> qz;
		int source_x=source[wire][0];
		int source_y=source[wire][1];
		int source_z=0;			
		int dest_x=dest[wire][0];
		int dest_y=dest[wire][1];
		int dest_z=0;
		bool finished=0;
		for(int i=0;i<_width;i++)
			for(int j=0;j<_height;j++)
				for(int k=0;k<_layer;k++)
				{
					d[i][j][k]=0;
					p[i][j][k]=0;
					visited[i][j][k]=0;
				}

		cout<<"    Wire # "<<wire+1;
		cout<<" from ("<<source_x<<","<<source_y<<")";
		cout<<" to ("<<dest_x<<","<<dest_y<<");    ";

		int x=source_x;
		int y=source_y;
		int z=source_z;
		
		visited[x][y][0]=1;
		_visited[x][y][0]=1;
		qx.push(x);
		qy.push(y);
		qz.push(z);
		while(!(qx.empty()) && finished==0)
		{
			x=qx.front();
			y=qy.front();
			z=qz.front();
			qx.pop();
			qy.pop();
			qz.pop();
			for(int i=0;i<6;i++)
			{
				if(canTravel(x,y,z,i))
				{
					int curr_x=x+deltax[i];
					int curr_y=y+deltay[i];
					int curr_z=z+deltaz[i];
					if(visited[curr_x][curr_y][curr_z]==0 && (_visited[curr_x][curr_y][curr_z]==0 || (curr_x==source_x && curr_y==source_y) || (curr_x==dest_x && curr_y==dest_y))) 
					{
						qx.push(curr_x);
						qy.push(curr_y);
						qz.push(curr_z);
						visited[curr_x][curr_y][curr_z]=1;
						p[curr_x][curr_y][curr_z]=i;
						d[curr_x][curr_y][curr_z]=d[x][y][z]+1;
						if(curr_x==dest_x && curr_y==dest_y && curr_z==0)
						{
							finished=1;
							_totalWiring+=d[curr_x][curr_y][curr_z];
							cout<<"    Wire Length: "<<d[curr_x][curr_y][curr_z]<<endl;
					
							vector<int> temp;
							vector<int> onewire;
							while((dest_x!=source_x) || (dest_y!=source_y) || (dest_z!=0))
							{
								_visited[dest_x][dest_y][dest_z]=1;
								int direction=p[dest_x][dest_y][dest_z];
								temp.push_back(direction);
								dest_x-=deltax[direction];
								dest_y-=deltay[direction];
								dest_z-=deltaz[direction];
							}
							vector<int>::reverse_iterator rit;
							for(rit=temp.rbegin();rit<temp.rend();rit++)
								onewire.push_back(*rit);
							solution.push_back(onewire);
							
						}
					}	
				}
			}
		}
	}
	for(int k=0;k<_width;k++)
	{
		for(int l=0;l<_height;l++)
		{
			delete [] p[k][l];
			delete [] d[k][l];
			delete [] visited[k][l];
		}
		delete [] p[k];
		delete [] d[k];
		delete [] visited[k];
	}
	delete [] p;
	delete [] d;
	delete [] visited;
	cout<<"    Total wiring length = "<<_totalWiring<<endl;
	cout<<endl;
}

PNG * SquareMaze::drawMaze(int source[][2], int dest[][2], int wire_num)
{
	PNG * output= new PNG(_width*10+1,_height*10+1);
	for(int h=0;h<_height*10+1;h++)
		drawPixel((*output)(0,h),0,0,0);
	for(int w=0;w<_width*10+1;w++)
		drawPixel((*output)(w,0),0,0,0);
	for(int wire=0;wire<wire_num;wire++)
	{
		drawPin(output,source[wire][0],source[wire][1]);
		drawPin(output,dest[wire][0],dest[wire][1]);
	}
	for(int i=0;i<_width;i++)
		for(int j=0;j<_height;j++)
		{
			if(_walls[i][2*j]==1)
				for(int k=0;k<11;k++)
					drawPixel((*output)((i+1)*10,j*10+k),0,0,0);
			if(_walls[i][2*j+1]==1)
				for(int k=0;k<11;k++)
					drawPixel((*output)(i*10+k,(j+1)*10),0,0,0);
		}

	return output;
}



PNG * SquareMaze::drawMazeWithSolution(int source[][2], int dest[][2], int wire_num)
{
	PNG * output=drawMaze(source,dest,wire_num);
	int	color[3][3]={{26,0,153},{255,153,204},{0,153,255}};
	int curr_layer;
	for(int i=0;i<solution.size();i++)
	{
		int x=source[i][0];
		int y=source[i][1];
		curr_layer=0;
		bool high=false;
		for(int j=0;j<solution[i].size();j++)
		{
			switch(solution[i][j])
			{
				case 0:
					if(!high)
						for(int w=3;w<8;w++)	
							for(int k=0;k<11;k++)						
								drawPixel((*output)(3+10*x+k,w+10*y),color[curr_layer][0],color[curr_layer][1],color[curr_layer][2]);
					if(j==0)
						drawPin(output,x,y);
					x++;
					break;
				case 1:
					if(!high)
						for(int w=3;w<8;w++)
							for(int k=0;k<11;k++)
								drawPixel((*output)(w+10*x,3+10*y+k),color[curr_layer][0],color[curr_layer][1],color[curr_layer][2]);
					if(j==0)
						drawPin(output,x,y);
					y++;
					break;
				case 2:
					if(!high)
						for(int w=3;w<8;w++)
							for(int k=0;k<11;k++)
								drawPixel((*output)(7+10*x-k,w+10*y),color[curr_layer][0],color[curr_layer][1],color[curr_layer][2]);
					if(j==0)
						drawPin(output,x,y);
					x--;
					break;
				case 3:
					if(!high)
						for(int w=3;w<8;w++)
							for(int k=0;k<11;k++)
								drawPixel((*output)(w+10*x,7+10*y-k),color[curr_layer][0],color[curr_layer][1],color[curr_layer][2]);
					if(j==0)
						drawPin(output,x,y);
					y--;					
					break;	
				case 4:
					curr_layer++;
					drawVia(output,x,y,curr_layer-1);
					if(j==0)
						drawPin(output,x,y);
					break;
				case 5:
					curr_layer--;
					drawVia(output,x,y,curr_layer);
					if(j==0)
						drawPin(output,x,y);
					break;
					
			}
			if(x==dest[i][0] && y==dest[i][1])
				drawPin(output,x,y);
			
		}	
	}
	return output;
}

animation SquareMaze::motionMazeWithSolution(int source[][2],int dest[][2],int wire_num)
{
	animation anim;
	PNG * output=drawMaze(source,dest,wire_num);
	int	color[3][3]={{26,0,153},{255,153,204},{0,153,255}};
	int curr_layer;
	for(int i=0;i<solution.size();i++)
	{
		int x=source[i][0];
		int y=source[i][1];
		curr_layer=0;
		bool high=false;
		for(int j=0;j<solution[i].size();j++)
		{
			switch(solution[i][j])
			{
				case 0:
					if(!high)
						for(int w=3;w<8;w++)	
							for(int k=0;k<11;k++)						
								drawPixel((*output)(3+10*x+k,w+10*y),color[curr_layer][0],color[curr_layer][1],color[curr_layer][2]);
					if(j==0)
						drawPin(output,x,y);
					x++;
					break;
				case 1:
					if(!high)
						for(int w=3;w<8;w++)
							for(int k=0;k<11;k++)
								drawPixel((*output)(w+10*x,3+10*y+k),color[curr_layer][0],color[curr_layer][1],color[curr_layer][2]);
					if(j==0)
						drawPin(output,x,y);
					y++;
					break;
				case 2:
					if(!high)
						for(int w=3;w<8;w++)
							for(int k=0;k<11;k++)
								drawPixel((*output)(7+10*x-k,w+10*y),color[curr_layer][0],color[curr_layer][1],color[curr_layer][2]);
					if(j==0)
						drawPin(output,x,y);
					x--;
					break;
				case 3:
					if(!high)
						for(int w=3;w<8;w++)
							for(int k=0;k<11;k++)
								drawPixel((*output)(w+10*x,7+10*y-k),color[curr_layer][0],color[curr_layer][1],color[curr_layer][2]);
					if(j==0)
						drawPin(output,x,y);
					y--;					
					break;	
				case 4:
					curr_layer++;
					drawVia(output,x,y,curr_layer-1);
					if(j==0)
						drawPin(output,x,y);
					break;
				case 5:
					curr_layer--;
					drawVia(output,x,y,curr_layer);
					if(j==0)
						drawPin(output,x,y);
					break;
					
			}
			if(x==dest[i][0] && y==dest[i][1])
				drawPin(output,x,y);
			if (j%20==0)
			anim.addFrame(*output);	
		}	
		
	}
	anim.addFrame(*output);
	return anim;
}

void SquareMaze::drawPixel(RGBAPixel * ptr, int r, int g, int b)
{
	ptr->red=r;
	ptr->green=g;
	ptr->blue=b;
}

void SquareMaze::drawPin(PNG * output, int x, int y)
{
	for(int k=0;k<11;k++)
	{
		drawPixel((*output)((x+1)*10,y*10+k),0,0,0);
		drawPixel((*output)((x)*10,y*10+k),0,0,0);
		drawPixel((*output)(x*10+k,(y+1)*10),0,0,0);
		drawPixel((*output)(x*10+k,(y)*10),0,0,0);
	}
	for(int i=3;i<8;i++)
		for(int j=3;j<8;j++)
			drawPixel((*output)(x*10+i,y*10+j),0,0,0);		
}

void SquareMaze::drawVia(PNG * output, int x, int y, int curr_layer)
{
	int	color[2][3]={{255,153,204},{0,153,255}};
	for(int i=0;i<11;i++)
		for(int j=0;j<11;j++)
			drawPixel((*output)(x*10+i,y*10+j),color[curr_layer][0],color[curr_layer][1],color[curr_layer][2]);
	for(int i=3;i<8;i++)
		for(int j=3;j<8;j++)
			drawPixel((*output)(x*10+i,y*10+j),0,0,0);	
}

int SquareMaze::deltaLayer(int dir)
{
	if(dir==4)
		return 1;
	if(dir==5)
		return -1;
	return 0;
}

void SquareMaze::clear()
{
	_width=0;
	_height=0;
	_layer=0;
	_totalWiring=0;
	if(_walls!=NULL)
	{
		for(int i=0;i<_width;i++)
			if(_walls[i]!=NULL)
				delete [] _walls[i];
		delete [] _walls;
		_walls=NULL;
	}
	if(_visited!=NULL)
	{
		for(int i=0;i<_width;i++)
		{
			if(_visited[i]!=NULL)
			{
				for(int j=0;j<_height;j++)
				{
					if(_visited[i][j]!=NULL)
						delete [] _visited[i][j];
				}
				delete [] _visited[i];
			}
		}		
		delete [] _visited;
		_visited=NULL;
	}
}

void SquareMaze::copy(SquareMaze const & other)
{
	_width=other._width;
	_height=other._height;
	_layer=other._layer;
	_totalWiring=other._totalWiring;
	_walls=new bool* [_width];
	for(int i=0;i<_width;i++)
	{
		_walls[i]=new bool [_height*2];
		for(int j=0;j<_height*2;j++)
			_walls[i][j]=other._walls[i][j];
	}
	_visited=new bool** [_width];
	for(int i=0;i<_width;i++)
	{
		_visited[i]=new bool* [_height];
		for(int j=0;j<_height;j++)
		{
			_visited[i][j]=new bool [_layer];
			for(int k=0;k++;k<_layer)
				_visited[i][j][k]=other._visited[i][j][k];
		}	
	}
}

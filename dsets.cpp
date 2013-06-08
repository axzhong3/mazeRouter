/**
 *	Disjoint set class implementation
 */

#include "dsets.h"

// add n new unconnected root nodes at the end
void DisjointSets::addelements(int n)
{
	for(int i=0;i<n;i++)
		array.push_back(-1);
}

// find the index of the root
int DisjointSets::find(int elem_number)
{
	if(array[elem_number]<0)
		return elem_number;
	else
		return (array[elem_number]=find(array[elem_number]));
}

// union the two sets by size
void DisjointSets::setunion(int a, int b)
{
	int roota=find(a);
	int rootb=find(b);
	if(roota!=rootb)
	{
		if((-1)*array[roota]<(-1)*array[rootb])
		{
			array[rootb]+=array[roota];
			array[roota]=rootb;
		}
		else
		{
			array[roota]+=array[rootb];
			array[rootb]=roota;
		}
	}
}

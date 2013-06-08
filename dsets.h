/** 
 *	Disjoint set class
 */

#ifndef DSETS_H
#define DSETS_H

#include <vector>
using std::vector;


class DisjointSets
{
	public:

		// add n new unconnected root nodes at the end
		void addelements(int n);

		// find the index of the root
		int find(int elem_number);

		// union the two sets by size
		void setunion(int a, int b);
	
	private:

		// the array of nodes
		vector<int> array;
};

#endif

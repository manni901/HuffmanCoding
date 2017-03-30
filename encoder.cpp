#include<iostream>
#include<vector>
#include<utility>
#include<fstream>
#include "ads.hpp"
using namespace std;

#define LEN 1000000

node :: node(int f, int v, bool x)
	: frequency(f), value(v), isExternal(x)
{
	left = 0;
	right = 0;
}

int main(int argc, char* argv[])
{
	int i;
	ifstream is(argv[1]);
	vector<int> freqTable(LEN);
	vector<node*> V;
	int val;
	while(is >> val) freqTable[val]++;
	for(i=0;i<LEN;i++)
	{
		if(freqTable[i] != 0)
		{
			V.push_back(new node(freqTable[i], i, true));
		}
	}
	
	binaryHeap BHeap(V);
	while(BHeap.getSize() > 1)
	{
		node* min = BHeap.removeMin();
		node* min1 = BHeap.removeMin();
		node* n = new node(min.frequency + min1.frequency, 0, false);
		n->left = min;
		n->right = min1;
		BHeap.insert(n);
	}
	
			
	
}

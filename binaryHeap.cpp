#include "ads.hpp"

binaryHeap :: binaryHeap(vector<node* > dat): data(dat)
{
	size = data.size();
	heapify();
}

int binaryHeap :: getSize()
{
	return size;
}

void binaryHeap :: heapify()
{
	int len = size-1;
	int curr = !(len&1) ? (len>>1)-1 : len>>1;
	while(curr >= 0)
	{
		heapifyDown(curr);
		curr--;
	}
}

node* binaryHeap :: removeMin()
{
	node* ans = data[0];
	data[0] = data[size-1];
	size--;
	//data.pop_back();
	heapifyDown(0);
	return ans;
}

void binaryHeap :: insert(node* first)
{
	//data.push_back(first);
	data[size] = first;
	size++;
	heapifyUp();
}

void binaryHeap :: heapifyUp()
{
	int index = size - 1;
	while(index > 0)
	{
		int parent = !(index&1) ? (index>>1)-1 : index>>1;
		if(data[index]->frequency < data[parent]->frequency)
		{
			index = swap(index, parent);
		}
		else break;
	}
}

void binaryHeap :: heapifyDown(int index)
{
	int len = size-1;
	int len1 = !(len&1) ? (len>>1)-1 : len>>1;
	while(index <= len1)
	{
		int ss = index;
		int left = (index<<1) + 1;
		int right = ((index<<1) + 2) <= len ? (index<<1) + 2 : index;
		if(data[index]->frequency > data[left]->frequency)
		{
			ss = left;
		}
		if(data[ss]->frequency > data[right]->frequency)
		{
			ss = right;
		}
		if(ss == index) break;
		else index = swap(index,ss);
	}
}

int binaryHeap :: swap(int a, int b)
{
	node* temp = data[a];
	data[a] = data[b];
	data[b] = temp;
	return b;
}
			
	
	
		
		

#include "ads.hpp"

KHeap :: KHeap(int ord, int off): order(ord), offset(off)
{
	size = 0;
	while(off--) data.push_back(0);
	heapify();
}

int KHeap :: getSize()
{
	return size;
}

void KHeap :: heapify()
{
	int len = size-1;
	int curr = (len-1)/order;
	while(curr >= 0)
	{
		heapifyDown(curr);
		curr--;
	}
}

node* KHeap :: removeMin()
{
	node* ans = data[offset];
	data[offset] = data[size-1+offset];
	size--;
	data.pop_back();
	heapifyDown(0);
	return ans;
}

void KHeap :: insert(node* first)
{
	data.push_back(first);
	size++;
	heapifyUp();
}

void KHeap :: heapifyUp()
{
	int index = size - 1;
	while(index > 0)
	{
		int parent = (index-1)/order;
		if(data[index+offset]->frequency < data[parent+offset]->frequency)
		{
			index = swap(index, parent);
		}
		else break;
	}
}

void KHeap :: heapifyDown(int index)
{
	int len = size-1;
	int len1 = (len-1)/order;
	while(index <= len1)
	{
		int ss = index;
		int start = index*order;
		for(int i=1;i<=order;i++)
		{
			start++;
			if(start <= len && data[start+offset]->frequency < data[ss+offset]->frequency) ss = start;
		}
		if(ss == index) break;
		else index = swap(index,ss);
	}
}

int KHeap :: swap(int a, int b)
{
	node* temp = data[a+offset];
	data[a+offset] = data[b+offset];
	data[b+offset] = temp;
	return b;
}
			
	
	
		
		

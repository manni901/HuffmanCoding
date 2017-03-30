#include<iostream>
#include<vector>
#include "ads.hpp"
using namespace std;


class binaryHeap
{
	vector<node* > data;
	
	public:
	binaryHeap(vector<node* > dat): data(dat)
	{
		heapify();
	}
	
	int getSize()
	{
		return data.size();
	}
	
	void heapify()
	{
		int len = data.size();
		int curr = len/2 - 1;
		while(curr >= 0)
		{
			int left = curr*2 + 1;
			int right = (curr*2 + 2) < len ? curr*2 + 2 : curr;
			int min = data[left]->frequency < data[right]->frequency ? left : right;
			min = data[min]->frequency < data[curr]->frequency ? min : curr;
			curr = swap(min,curr);
			curr--;
		}
	}
	
	node* removeMin()
	{
		node* ans = data[0];
		data[0] = data.back();
		data.pop_back();
		heapifyDown();
		return ans;
	}
	
	void insert(node* first)
	{
		data.push_back(first);
		heapifyUp();
	}
	
	void heapifyUp()
	{
		int index = data.size() - 1;
		while(index > 0)
		{
			int parent = index%2 == 0 ? index/2-1 : index/2;
			if(data[index]->frequency < data[parent]->frequency)
			{
				index = swap(index, parent);
			}
			else break;
		}
	}
	
	void heapifyDown()
	{
		int index = 0;
		int len = data.size()/2;
		while(index < len)
		{
			int left = index*2+1;
			int right = (index*2 + 2) < len ? index*2 + 2 : index;
			if(data[index]->frequency > data[left]->frequency)
			{
				index = swap(index, left);
			}
			else if(data[index]->frequency > data[right]->frequency)
			{
				index = swap(index, right);
			}
			else break;
		}
	}
	
	int swap(int a, int b)
	{
		node* temp = data[a];
		data[a] = data[b];
		data[b] = temp;
		return b;
	}
};
			
	
	
		
		

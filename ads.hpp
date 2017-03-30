#include<iostream>
#include<vector>
using namespace std;
struct node
{
	int frequency;
	int value;
	bool isExternal;
	node *left;
	node *right;
	
	node(int,int,bool);
};

class heap
{
public:
    heap(){}
    virtual ~heap(){}
    virtual node* removeMin()=0;
    virtual void insert(node*)=0;
    virtual int getSize()=0;
};

class binaryHeap : public heap
{
	vector<node*> data;
	int size;
	public:
	binaryHeap(vector<node*>);
	int getSize();
	void heapify();
	node* removeMin();
	void insert(node*);
	void heapifyUp();
	void heapifyDown(int);
	int swap(int, int);
};
	

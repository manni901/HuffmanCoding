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

struct pairNode
{
	node* data;
	vector<pairNode*> child;
	
	pairNode(node*);
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
	int order;
	int offset;
	public:
	binaryHeap(vector<node*>, int, int);
	int getSize();
	void heapify();
	node* removeMin();
	void insert(node*);
	void heapifyUp();
	void heapifyDown(int);
	int swap(int, int);
};
	
class pairingHeap : public heap
{
	pairNode* root;
	int size;
	
	public:
	pairingHeap(vector<node*>);
	int getSize();
	void insert(node*);
	node* removeMin();
	pairNode* combine(pairNode*, pairNode*);
};

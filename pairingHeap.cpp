#include "ads.hpp"

pairNode :: pairNode(node* n) : data(n) {}

pairingHeap :: pairingHeap(vector<node*> V)
{
	root = 0;
	size = 0;
	for(int i=0;i<V.size();i++)
	{
		insert(V[i]);
	}
}

int pairingHeap :: getSize()
{
	return size;
}

void pairingHeap :: insert(node* dat)
{
	pairNode* n = new pairNode(dat);
	if(root == 0) root = n;
	else root = combine(root, n);
	size++;
}

node* pairingHeap :: removeMin()
{
	node* ans = root->data;
	pairNode* temp = root;
	if(size > 1)
	{
		int i;
		int ss = root->child.size();
		for(i=ss-1; i-1 >= 0 ;i-=2)
		{
			root->child[i-1] = combine(root->child[i], root->child[i-1]);
		}
		
		i = i==0 ? 1 : 2;
		for(i;i<ss;i+=2)
		{
			root->child[0] = combine(root->child[0], root->child[i]);
		}
	}
	root = size == 1 ? 0 : root->child[0];
	temp->child.clear();
	delete(temp);
	size--;
	return ans;
}

pairNode* pairingHeap :: combine(pairNode* a, pairNode* b)
{
	if(a->data->frequency < b->data->frequency)
	{
		a->child.push_back(b);
		return a;
	}
	else
	{
		b->child.push_back(a);
		return b;
	}
}

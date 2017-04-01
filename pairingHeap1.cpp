#include "ads.hpp"

pairNode :: pairNode(node* n) : data(n) 
{
	left = next = prev = 0;
}

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
	pairNode* temp = root->left;
	if(size > 1)
	{
		temp->prev = 0;	
		pairNode* arbit = 0;
		pairNode* pree = 0;
		while(temp->next != 0)
		{
			arbit = temp->next->next;
			temp = combine(temp, temp->next);
			temp->prev = pree;
			pree = temp;
			temp->next = arbit;
			if(arbit != 0) 
			{ 
				arbit->prev = temp;
				temp = arbit;
			}
		}
		
		while(temp->prev != 0)
		{
			arbit = temp->prev->prev;
			temp = combine(temp, temp->prev);
			temp->prev = arbit;
		}
	}	
	delete(root);
	root = temp;
	size--;
	return ans;
}

pairNode* pairingHeap :: combine(pairNode* a, pairNode* b)
{
	pairNode* temp = 0;
	if(a->data->frequency > b->data->frequency)
	{
		temp = a;
		a = b;
		b = temp;
	}
	a->next = a->prev = b->next = b->prev = 0;
	temp = a->left;
	a->left = b;
	b->prev = a;
	b->next = temp;
	if(temp != 0) temp->prev = b;
	return a;
}

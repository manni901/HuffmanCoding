struct node
{
	int frequency;
	int value;
	bool isExternal;
	node *left;
	node *right;
	
	node(int,int,bool);
};

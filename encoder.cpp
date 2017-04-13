#include "ads.hpp"
#include<utility>
#include<fstream>
#include<climits>
#include<stack>
#include<ctime>
#include<string>
using namespace std;

#define LEN 1000000           // Range of integers allowed
#define BUFLEN 10000000       // Buffer length for write to file
#define ENCO "encoded.bin"    // Encoded file name
#define CODE "code_table.txt" // Code table file name

/**
 * Constructor for node
*/
node :: node(int f, int v, bool x)
	: frequency(f), value(v), isExternal(x)
{
	left = 0;
	right = 0;
}

void fillFrequencyTable(char*, vector<int>&);
void genNodeArray(vector<int>&, vector<node*>&);
node* buildHuffmanTree(heap*, vector<node*>&);
void buildCodeTable(vector<string>&, node*);
void writeEncodedFile(char*, vector<string>&);

int min(int a, int b)
{
	return a < b ? a : b;
}

int main(int argc, char* argv[])
{
	clock_t start_time;
	start_time = clock();
	
	// Table to store frequency of each number in input file
	vector<int> freqTable(LEN,0);
	
	// Collection to hold individual trees for each number
	vector<node*> V; 

	// Read file and update frequency count
	fillFrequencyTable(argv[1], freqTable);

	// Build individual nodes for each number
	genNodeArray(freqTable, V); // 
	
	// Create three different heap objects (Binary Heap, 4-Way Heap and Pairing Heap
	heap* Heap[] = 
	{
		new KHeap(2,0), 
		new KHeap(4,3), 
		new pairingHeap()
	};
	
	node* root = 0;
	// Build huffman tree using each type of heap
	for(int l=9;l<10;l++)
	{
		for(int k=2;k<3;k++)
		{
			root = buildHuffmanTree(Heap[k], V);
		}
	}
	
	// Code table to store string bitcode for each number
	vector<string> codeTable(LEN);
	
	// Build code table from huffman tree and write to file
	buildCodeTable(codeTable, root);
	
	// Read input file and write encoded binary file based on the codetable
	writeEncodedFile(argv[1], codeTable);
	
	// Output total time in seconds
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
}

/**
 * Read input file in chunks of size BUFLEN.
 * For each parsed number, increase its corresponding count in freqTable by 1
 * Input: input filename, freqTable vector passed by reference
*/
void fillFrequencyTable(char* filename, vector<int>& freqTable)
{
	ifstream is(filename);
	int val=0;
	char *buf = new char[BUFLEN];
	
	is.seekg (0, is.end);
    int fLength = is.tellg();
    is.seekg (0, is.beg);
    
	while(is)
	{
		is.read(buf, BUFLEN);
		for(int x=0;x<min(BUFLEN,fLength);x++)
		{
			if(buf[x] >= '0' && buf[x] <= '9') val = val*10 + (buf[x] - '0');
			else if(buf[x] == '\n')
			{
				
				freqTable[val]++;
				val = 0;
			}
		}
		fLength -= BUFLEN;
	}
	is.close();
}

/**
 * Dynamically create objects of type node for each number in freqTable
 * These nodes serve as initial single node trees for huffman tree
*/ 
void genNodeArray(vector<int>& freqTable, vector<node*>& V)
{
	for(int i=0;i<LEN;i++)
	{
		if(freqTable[LEN-i-1] != 0)
		{
			V.push_back(new node(freqTable[LEN-i-1], LEN-i-1, true));
		}
	}
}


/**
 * Build huffman tree using the following workflow.
 * Remove two minimum frequency nodes from heap.
 * Create a new node with the two removed nodes as its left and right children.
 * Insert this node back in the heap.
 * Repeat till there is only one node left in the heap. Return this node as the root of huffman tree.
 * Input: pointer to Heap, and vector of initial single node trees.
*/
node* buildHuffmanTree(heap* H, vector<node*>& V)
{
	for(int m=0;m<V.size();m++)
	{
		H->insert(V[m]);
	}
	while(H->getSize() > 1)
	{
		node* min = H->removeMin();
		node* min1 = H->removeMin();
		node* n = new node(min->frequency + min1->frequency, 0, false);
		n->left = min;
		n->right = min1;
		H->insert(n);
	}
	return H->removeMin();
}

/**
 * Perform a DFS traversal over the huffman tree to generate bitcodes
 * for each external node and write code to file.
 * Input: codeTable passed by reference, and pointer to root node of huffman tree
*/
void buildCodeTable(vector<string>& codeTable, node* root)
{
	ofstream code(CODE);
	string outstr = "";
	
	stack<pair<node*,string > > qu;
	qu.push(make_pair(root,""));
	while(!qu.empty())
	{
		pair<node*,string> P = qu.top();
		qu.pop();
		if(!P.first->isExternal)
		{
			qu.push(make_pair(P.first->right,P.second+'1'));
			qu.push(make_pair(P.first->left,P.second+'0'));
		}
		else
		{
			int v = P.first->value;
			codeTable[v] = P.second;
			code << v << " " << P.second << "\n";
		}
	}
}

/**
 * Method to write encoded binary file.
 * Read input file in chunks. For each number get its corresponding bitcode string
 * Pack the bitcode in char variables and write to output stream
*/
void writeEncodedFile(char* input, vector<string>& codeTable)
{
	ifstream is(input);
	int val=0;
	char byteCode=0;
	char ans[100000];
	int windex = 0;
	int j = CHAR_BIT-1;
	char *buf = new char[BUFLEN];
	is.seekg (0, is.end);
    int fLength = is.tellg();
    is.seekg (0, is.beg);
    ofstream enco(ENCO, ios::out | ios::binary);
    
    bool flag = true;
    
	while(is)
	{
		is.read(buf, BUFLEN);
		for(int x=0;x<min(BUFLEN,fLength);x++)
		{
			if(buf[x] >= '0' && buf[x] <= '9') { flag = true; val = val*10 + (buf[x] - '0'); }
			else if(buf[x] == '\n' && flag == true)
			{
				flag = false;
				string newCode = codeTable[val];
				for(int i=0;i<newCode.length();i++)
				{
					byteCode |= (newCode[i] - '0') << (j--);
					if(j<0)
					{
						ans[windex++] = byteCode;
						byteCode=0;
						if(windex == 100000) { enco.write(&ans[0], windex); windex = 0;}
						j = CHAR_BIT-1;
					}
				}
				val = 0;
			}
		}
		fLength -= BUFLEN;
	}
	if(windex > 0) enco.write(&ans[0], windex);
	is.close();
	enco.close();
}

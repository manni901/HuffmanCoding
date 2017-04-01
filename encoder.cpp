#include "ads.hpp"
#include<utility>
#include<fstream>
#include<climits>
using namespace std;

#define LEN 1000000
#define ENCO "encoded1.bin"
#define CODE "code_table1.txt"

node :: node(int f, int v, bool x)
	: frequency(f), value(v), isExternal(x)
{
	left = 0;
	right = 0;
}

void fillFrequencyTable(char*, vector<int>&);
void genNodeArray(vector<int>&, vector<node*>&);
void buildCodeTable(vector<vector<char> >&, node*);
void writeCodeTable(vector<vector<char> >&, vector<int>&);
void writeEncodedFile(char*, vector<vector<char> >&);

int main(int argc, char* argv[])
{
	clock_t start_time;
	start_time = clock();
	int i;
	vector<int> freqTable(LEN,0);
	vector<node*> V;
	cout<<"Start\n";
	fillFrequencyTable(argv[1], freqTable);
	cout<<"Frequency Done\n";
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
	genNodeArray(freqTable, V);
	cout<<"NodeArray Built\n";
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";		
	heap* BHeap = new binaryHeap(V,2,0);
	//cout<<"Heap Built\n";
	//cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
	
	/*while(BHeap->getSize() != 0)
	{
		node* test = BHeap->removeMin();
		//cout<<test->value<<" "<<test->frequency<<"\n";
	}*/
	
	while(BHeap->getSize() > 1)
	{
		node* min = BHeap->removeMin();
		node* min1 = BHeap->removeMin();
		node* n = new node(min->frequency + min1->frequency, 0, false);
		n->left = min;
		n->right = min1;
		BHeap->insert(n);
	}
	node* root = BHeap->removeMin();
	cout<<"Huffman Tree Built\n";
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
	
	vector<vector<char> > codeTable(LEN);
	buildCodeTable(codeTable, root);
	cout<<"CodeTable Built\n";
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
	
	writeEncodedFile(argv[1], codeTable);
	cout<<"\nEncodedFileEnd";
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
	writeCodeTable(codeTable, freqTable);
	cout<<"\nTableFile Done\n";
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
}

void fillFrequencyTable(char* filename, vector<int>& freqTable)
{
	ifstream is(filename);
	int val;
	while(is >> val) freqTable[val]++;
	is.close();
}

void genNodeArray(vector<int>& freqTable, vector<node*>& V)
{
	for(int i=0;i<LEN;i++)
	{
		if(freqTable[i] != 0)
		{
			V.push_back(new node(freqTable[LEN-i-1], LEN-i-1, true));
		}
	}
}

void buildCodeTable(vector<vector<char> >& codeTable, node* root)
{
	vector<pair<node*,vector<char> > > qu;
	qu.push_back(make_pair(root,vector<char>()));
	while(!qu.empty())
	{
		pair<node*,vector<char> > P = qu.back();
		qu.pop_back();
		if(!P.first->isExternal)
		{
			P.second.push_back('1');
			qu.push_back(make_pair(P.first->right,P.second));
			P.second.pop_back(); P.second.push_back('0');
			qu.push_back(make_pair(P.first->left,P.second));
		}
		else
		{
			codeTable[P.first->value] = P.second;
		}
	}
}

void writeCodeTable(vector<vector<char> >& codeTable, vector<int>& freqTable)
{
	ofstream code(CODE);
	for(int i=0;i<LEN;i++)
	{
		if(freqTable[i] != 0)
		{
			code << i << " " << string(codeTable[i].begin(), codeTable[i].end()) << "\n";
		}
	}
	code.close();
}

void writeEncodedFile(char* input, vector<vector<char> >& codeTable)
{
	vector<char> bits;
	ifstream is(input);
	vector<char> encrypted;
	int val;
	char byteCode;
	int j = CHAR_BIT-1;
	while(is >> val)
	{
		vector<char> newCode = codeTable[val];
		for(int i=0;i<newCode.size();i++)
		{
			if(newCode[i] == '0') byteCode &= ~(1 << j);
			else byteCode |= 1 << j;
			j--;
			if(j<0)
			{
				encrypted.push_back(byteCode);
				j = CHAR_BIT-1;
			}
		}
	}
	is.close();
	
	ofstream enco(ENCO, ios::out | ios::binary);
	enco.write(&encrypted[0], encrypted.size());
	enco.close();
}

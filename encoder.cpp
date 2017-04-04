#include "ads.hpp"
#include<utility>
#include<fstream>
#include<climits>
#include<stack>
using namespace std;

#define LEN 1000000
#define BUFLEN 10000000
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
node* buildHuffmanTree(heap*, vector<node*>&);
void buildCodeTable(vector<string>&, node*);
void writeEncodedFile(char*, vector<string>&);

int max(int a, int b)
{
	return a < b ? a : b;
}

int main(int argc, char* argv[])
{
	cout<<sizeof(int)<<" "<<sizeof(node*)<<"\n";
	clock_t start_time;
	start_time = clock();
	int i;
	vector<int> freqTable(LEN,0);
	vector<node*> V;
	cout<<"Start\n";
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
	fillFrequencyTable(argv[1], freqTable);
	cout<<"Frequency Done\n";
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
	genNodeArray(freqTable, V);
	cout<<"NodeArray Built\n";
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";		
	heap* Heap[] = {new KHeap(2,0), new KHeap(4,3), new pairingHeap()};
	node* root = 0;
	for(int k=2;k<3;k++)
	{
		clock_t ST;
		ST = clock();
		for(int l=9;l<10;l++)
		{
			buildHuffmanTree(Heap[i], V);
		}
		cout<<clock() - ST<<"\n";
	}
	
	cout<<"Huffman Tree Built\n";
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
	
	vector<string> codeTable(LEN);
	buildCodeTable(codeTable, root);
	cout<<"CodeTable Built\n";
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
	
	writeEncodedFile(argv[1], codeTable);
	cout<<"Encoding Done\n";
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
}

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

node* buildHuffmanTree(heap* H, vector<node*>& V)
{
	for(int m=0;m<V.size();m++)
	{
		H->insert(V[m]);
	}
	while(BHeap[k]->getSize() > 1)
	{
		node* min = H->removeMin();
		node* min1 = H->removeMin();
		node* n = new node(min->frequency + min1->frequency, 0, false);
		n->left = min;
		n->right = min1;
		Heap->insert(n);
	}
	return BHeap[k]->removeMin();
}

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
    ofstream enco(ENCO, ios::out | ios::K);
	while(is)
	{
		is.read(buf, BUFLEN);
		for(int x=0;x<min(BUFLEN,fLength);x++)
		{
			if(buf[x] >= '0' && buf[x] <= '9') val = val*10 + (buf[x] - '0');
			else if(buf[x] == '\n')
			{
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

#include "ads.hpp"
#include<utility>
#include<fstream>
#include<climits>
#include<bitset>
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

struct codeEntry
{
	vector<char> code;
	vector<char> byte;
	int lastShift;
	
	codeEntry(){};
	codeEntry(vector<char> c, vector<char> b, int s) : code(c), byte(b), lastShift(s) {};
};

void fillFrequencyTable(char*, vector<int>&);
void genNodeArray(vector<int>&, vector<node*>&);
void buildCodeTable(vector<codeEntry>&, node*);
void writeCodeTable(vector<codeEntry>&, vector<int>&);
void writeEncodedFile(char*, vector<codeEntry>&);

int max(int a, int b)
{
	return a < b ? a : b;
}

vector<char> vectorBitShift(char& prev, vector<char> V, int shift, int lastShift)
{
	int len = V.size();
	for(int i=0;i<len;i++)
	{
		char temp = V[i];
		V[i] = ((unsigned char)V[i])>>shift;
		V[i] |= prev;
		prev = temp << (CHAR_BIT - shift);
	}
	if(shift + lastShift < CHAR_BIT) { prev = V.back(); V.pop_back(); } 
	return V;
}

void updateCodeEntry(codeEntry& entry, vector<char> bits)
{
	char byteCode = 0;
	int j = CHAR_BIT - 1;
	for(int i=0;i<bits.size();i++)
	{
		if(bits[i] == '0') byteCode &= ~(1 << j);
		else byteCode |= 1 << j;
		j--;
		if(j<0)
		{
			entry.byte.push_back(byteCode);
			byteCode = 0;
			j = CHAR_BIT-1;
		}
	}
	if(j != CHAR_BIT - 1) entry.byte.push_back(byteCode);
	entry.code = bits;
	entry.lastShift = CHAR_BIT - 1 - j;
}

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
	heap* BHeap[] = {new binaryHeap(2,0), new binaryHeap(4,0), new pairingHeap()};
	node* root = 0;
	
	//////////////////////// PERFORMANCE PROFILING ///////////////////////////
	for(int k=2;k<3;k++)
	{
		clock_t ST;
		ST = clock();
		for(int l=9;l<10;l++)
		{
			for(int m=0;m<V.size();m++)
			{
				BHeap[k]->insert(V[m]);
			}
			while(BHeap[k]->getSize() > 1)
			{
				node* min = BHeap[k]->removeMin();
				node* min1 = BHeap[k]->removeMin();
				node* n = new node(min->frequency + min1->frequency, 0, false);
				n->left = min;
				n->right = min1;
				BHeap[k]->insert(n);
			}
			root = BHeap[k]->removeMin();
		}
		cout<<clock() - ST<<"\n";
	}
	////////////////////////////////////////////////////////////////////////////
	
	//ofstream ff("timings.txt");
	/*while(BHeap[2]->getSize() > 1)
	{
		//ST = clock();
		node* min = BHeap[2]->removeMin();
		node* min1 = BHeap[2]->removeMin();
		//ff<<clock() - ST<<"\n";
		node* n = new node(min->frequency + min1->frequency, 0, false);
		n->left = min;
		n->right = min1;
		//ST = clock();
		BHeap[2]->insert(n);
		//ff<<clock() - ST<<"\n";
	}
	node* root = BHeap[2]->removeMin();*/
	cout<<"Huffman Tree Built\n";
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
	
	vector<codeEntry> codeTable(LEN);
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
		if(freqTable[LEN-i-1] != 0)
		{
			V.push_back(new node(freqTable[LEN-i-1], LEN-i-1, true));
		}
	}
}

void buildCodeTable(vector<codeEntry>& codeTable, node* root)
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
			updateCodeEntry(codeTable[P.first->value], P.second);
		}
	}
}

void writeCodeTable(vector<codeEntry>& codeTable, vector<int>& freqTable)
{
	ofstream code(CODE);
	for(int i=0;i<LEN;i++)
	{
		if(freqTable[i] != 0)
		{
			code << i << " " << string(codeTable[i].code.begin(), codeTable[i].code.end()) << "\n";
		}
	}
	code.close();
}

void writeEncodedFile(char* input, vector<codeEntry>& codeTable)
{
	vector<char> bits;
	ifstream is(input);
	vector<char> encrypted;
	int val=0;
	char byteCode;
	int j = CHAR_BIT-1;
	char *buf = new char[BUFLEN];
	is.seekg (0, is.end);
    int fLength = is.tellg();
    is.seekg (0, is.beg);
    int shift = 0;
    char prev = 0;
    ofstream enco(ENCO, ios::out | ios::binary);
	while(is)
	{
		is.read(buf, BUFLEN);
		bool flag = true;
		for(int x=0;x<min(BUFLEN,fLength);x++)
		{
			if(buf[x] >= '0' && buf[x] <= '9') { flag = true; val = (val<<3) + (val<<1) + (buf[x] - '0');}
			else if(flag)
			{
				flag = false;
				int newShift = codeTable[val].lastShift + shift;
				if(shift == 0)
				{
					int siz = codeTable[val].byte.size();
					if(codeTable[val].lastShift != 0)
					{
						prev = codeTable[val].byte[siz];
						siz--;
					}
					enco.write(&codeTable[val].byte[0], siz);
					for(int dd=0;dd<siz;dd++)cout<<bitset<CHAR_BIT>(codeTable[val].byte[dd]);
					cout<<"------------"<<bitset<CHAR_BIT>(prev)<<"\n";
				}
				else
				{
					vector<char> ans = vectorBitShift(prev, codeTable[val].byte, shift, codeTable[val].lastShift);
					enco.write(&ans[0], ans.size());
					for(int dd=0;dd<ans.size();dd++)cout<<bitset<CHAR_BIT>(ans[dd]);
					cout<<"------------"<<bitset<CHAR_BIT>(prev)<<"\n";
					
				}
				shift = (newShift) % CHAR_BIT;
				val = 0;
			}
		}
		fLength -= BUFLEN;
	}
	is.close();
	//ofstream enco(ENCO, ios::out | ios::binary);
	//enco.write(&encrypted[0], encrypted.size());
	enco.close();
}

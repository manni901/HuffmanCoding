#include<iostream>
#include<vector>
#include<climits>
#include<fstream>
#include<string>
#include<ctime>
using namespace std;

#define SIZ 10000000    // Buffer length for reading from encoded file
#define DECO "decoded.txt"

int min(int a, int b)
{
	return a < b ? a : b;
}

struct node
{
	node *left;
	node *right;
	string data;
	bool isExternal;
	
	node()
	{
		left = right = 0;
		data = "";
		isExternal = false;
	}
};

class trie
{
	node* root;
	
	void insert(string s, string val)
	{
		int len = s.length();
		node* temp = root;
		for(int i=0;i<len;i++)
		{
			if(s[i] == '1')
			{
				if(temp->right == 0)temp->right = new node;
				temp = temp->right;
			}
			else
			{
				if(temp->left == 0)temp->left = new node;
				temp = temp->left;
			}
		}
		temp->isExternal = true;
		temp->data = val;
	}
	
	public:
	trie()
	{
		root = new node;
	}
	
	void insertAll(vector<string> key, vector<string> val)
	{
		for(int i=0;i<(int)key.size();i++)
		{
			insert(key[i],val[i]);
		}
	}
	
	node* search(char* encoded, int s, node* temp, ofstream& outp)
	{
		if(temp == 0) temp = root;
		
		char bitVal;
		string ans = "";

		for(int x=0;x<s;x++)
		{
			bitVal = encoded[x];
			for (int i = CHAR_BIT-1; i >= 0; i--)
			{
				if(((bitVal >> i) & 1))
				{
					temp = temp->right;
				}
				else
				{
					temp = temp->left;
				}
				
				if(temp == 0)
				{
					cout<<"\nError in Decoding";
					return 0;
				}
				if(temp->isExternal)
				{
					ans += temp->data+"\n";
					if(ans.length() > 10000000)
					{
						outp<<ans;
						ans = "";
					}
					temp = root;
				}
			}
		}
		if(ans.length() > 0 ) outp<<ans;
		return temp;
	}
};
		
int main(int argc, char* argv[])
{
	clock_t start_time;
	start_time = clock();
	
	trie D;
	
	ifstream codeTable(argv[2]);
	vector<string> key;
	vector<string> val;
	string a,b;
	while(codeTable >> a >> b)
	{
		key.push_back(b);
		val.push_back(a);
	}
	codeTable.close();
	
	D.insertAll(key,val);

	
	ifstream is(argv[1], ios::binary);
	is.seekg(0, ios_base::end);
	int size=is.tellg();
	is.seekg(0, ios_base::beg);
	
	char *v = new char[SIZ];
	node* temp = 0;
	ofstream outp(DECO);
	while(is)
	{
		is.read(&v[0], SIZ);
		temp = D.search(v, min(SIZ,size), temp, outp);
		size -= SIZ;
	}
	is.close();
	outp.close();
	
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
}
	

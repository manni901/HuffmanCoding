#include<iostream>
#include<vector>
#include<climits>
#include<fstream>
#include<bitset>
using namespace std;

struct node
{
	node *left;
	node *right;
	char data;
	bool isExternal;
	
	node()
	{
		left = right = 0;
		data = ' ';
		isExternal = false;
	}
};

class trie
{
	node* root;
	
	int stringToInt(string val)
	{
		int ans = 0;
		for(int i=0;i<(int)val.length();i++) ans = ans*10 + (val[i] - '0');
		return ans;
	}
	
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
		temp->data = stringToInt(val);
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
	
	void search(vector<char> encoded)
	{
		ofstream outp("decoded.txt");
		node* temp = root;
		
		char bitVal;
		string ans = "";

		for(int x=0;x<(int)encoded.size();x++)
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
					return;
				}
				if(temp->isExternal)
				{
					ans += temp->data;
					if(ans.length() > 10000000)
					{
						outp<<ans;
						ans = "";
					}
					temp = root;
				}
			}
		}
		outp<<ans;
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
	
	D.insertAll(key,val);
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
	
	ifstream is(argv[1]);
	is.seekg(0, ios_base::end);
	size_t size=is.tellg();
	is.seekg(0, ios_base::beg);
	vector<char> v(size/sizeof(char));
	is.read(&v[0], size);
	is.close();
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
	
	D.search(v);
	cout<<v.size()<<"\n";
	cout<<"Time : "<< (float)(clock() - start_time)/1000000<<"\n";
}
	


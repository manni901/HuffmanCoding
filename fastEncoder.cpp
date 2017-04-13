#include<iostream>
#include<vector>
#include<cstdio>
using namespace std;

int main()
{
	//freopen("input.txt", "r", stdin);
	int T,N,K,P,val;
	cin>>T;
	vector<int> ingredient;
	vector<vector<int> > island; 
	while(T--)
	{
		cin>>N>>K;
		ingredient.resize(K+1,0);
		island.resize(N,vector<int>());
		ingredient[0] = 2;
		while(N--)
		{
			cin>>P;
			while(P--)
			{
				cin>>val;
				ingredient[val]++;
				if(ingredient[val] == 1) K--;
				island[N].push_back(val);
			}
		}
		if(K != 0) cout<<"sad\n";
		else
		{
			int flag = 0;
			for(vector<int> V : island)
			{
				bool important = false;
				for(int val : V)
				{
					if(ingredient[val] < 2) important = true;
				}
				if(!important) {flag = 1; cout<<"some\n"; break;}
			}
			if(flag == 0)cout<<"all\n"; 
		}
		ingredient.clear();
		island.clear();
	}
}

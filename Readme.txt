For decoding I have used a binary trie of variable key lengths to store the code table. Each node of the binary trie has the following structure:

struct node
{
	node *left;
	node *right;
	string data;
	bool isExternal;
};

All external nodes (the ones with code table values) have the isExternal flag as true. It is false by default.
Following is a brief description of how the trie is built

- node root = NULL
- node curr = root
- While not(EndOfCodeFile)
	- Read number, bitcode
	- For each bit in bitcode
		- if bit == 0
			- if (curr.left == NULL) curr.left = new node
			- curr = curr->left
		- if bit == 1
			- if (curr.right == NULL) curr.right = new node
			- curr = curr->right
	- Since we are at end of bitcode, we are at an enternal node
	- curr.data = number
	- curr.isExternal = true
The time complexity of trie construction is linear in the order of sum of length of bitcodes for all the numbers in the codetable. The space complexity is linear in order of total number of rows in the codetable.
 
The height of the binary trie is the length of the longest bit code across all the code table values. Lets call this value 'H'. Thus the time complexity for searching a single code is O(H).

The time complexity of the entire decoding is O(n), where n is the sum of length of all bit codes in the encoded file (the total number of bits in the encoded file). Following is a brief description of how the search proceeds.

- Set current node (curr) to root.
- While not(EndOfFile) 
	- Read next bit (b) from file input stream.
	- curr = (b==0) ? curr.left : curr.right
	- if curr.isExternal
		- output curr.data to file
		- curr = root
  	- goto step 2

As you can see we traverse the encoded file exactly once bit by bit, and for each bit we only move one level down the trie in constant time. Hence the total time is O(n)

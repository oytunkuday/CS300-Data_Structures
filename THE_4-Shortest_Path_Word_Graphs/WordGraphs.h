#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>
#include <stack>

using namespace std;


#ifndef WORDGRAPHS
#define WORDGRAPHS

struct vertex {
	string word;
	bool known = false;
	vector<vertex*> edges;
	int dist=INT_MAX;
	vertex* path=nullptr;
	vertex(const string & wordpara) :word(wordpara) {}
};
class WordGraphs
{
	public:
		WordGraphs(ifstream & in);
		~WordGraphs();
		bool isindict(const string& w1, const string& w2);
		void findshortestpath(const string& w1, const string& w2);

	private:
		string possiblechars[29]= { "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","-","‘","'" };
		queue<vertex*> bfsgraph;
		queue<vertex*> vertexestoreset;
		void operationfinder(const string& w1, const string& w2);
		int NUM_VERTICES;
		void resetEdges();
		unordered_map<string, vertex*> wordlist;
		bool checkifonediff(const string& word1, const string& word2) const;
		void unweightedBFS(vertex *s,vertex*goal);
		string insertion(const string& word, int pos, const string& ch);
		string substitution(const string& word, int pos, const string& ch);
		string deletion(const string& word, int pos);
};

#endif
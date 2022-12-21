#include "WordGraphs.h"

WordGraphs::WordGraphs(ifstream& in) {
	string word;
	while (in >> word) {
		if (wordlist.find(word) == wordlist.end()) {
			wordlist[word] = new vertex(word);
		}
	}
	//int count = 0;
	for (auto it : wordlist) {
		//cout << ++count << endl;
		string temp = it.first;
		int len = temp.length();
		vertex* tempvertex;
		for (int i = 0;i < 29;i++) {
			string newchar = possiblechars[i];
			string newword = temp;
			for (int j = 0;j <= len;j++) {
				if (j != len) {
					newword = deletion(temp, j);
					if (wordlist.find(newword) != wordlist.end()) {
						tempvertex = wordlist[newword];
						it.second->edges.push_back(tempvertex);
					}
					newword = temp;
					newword = substitution(temp, j, newchar);
					if (wordlist.find(newword) != wordlist.end() && newword != temp){
						tempvertex = wordlist[newword];
						it.second->edges.push_back(tempvertex);
					}
					newword = temp;

				}
				
				newword = insertion(temp, j, newchar);
				if (wordlist.find(newword) != wordlist.end()) {
					tempvertex = wordlist[newword];
					it.second->edges.push_back(tempvertex);
				}
				newword = temp;

			}

			/*for (auto& it2 : wordlist) {
				if (it.first != it2.first) {
					if (checkifonediff(it.first, it2.first)) {
						it.second->edges.push_back(it2.second);
						//cout << it.second->word << "   " << it2.second->word << endl;
					}
				}
			}
			*/
		}
	}
	NUM_VERTICES = wordlist.size();
}

WordGraphs::~WordGraphs() {

}

string WordGraphs::insertion(const string& word, int pos,const string & ch)
{
	string newword = word;
	if (pos < word.length())
	{
		return newword.insert(pos, ch);
	}
	else
		return (word + ch);
}

string WordGraphs::substitution(const string& word, int pos, const string& ch)
{
	string newword = word;
	char chr=ch[0];
	newword[pos] = chr;
	return newword;
}

string WordGraphs::deletion(const string& word, int pos)
{
	return (word.substr(0, pos) + word.substr(pos + 1));
}

bool WordGraphs::isindict(const string& word1, const string& word2) {
	if (wordlist.find(word1) == wordlist.end() || wordlist.find(word2) == wordlist.end()) {
		cout << "At least one of the words are not in the file" << endl;
		return false;
	}
	else {
		return true;
	}
}



void WordGraphs::operationfinder(const string& w1, const string& w2)
{
	int i=0,j=0;
	string old="",difference = "";
	bool flag = true;
	if (w1.length() > w2.length()) {//deletion
		while (w1[j] == w2[j]) {
			j++;
			if (j == w2.length()) {
				difference.push_back(w1[j]);
				cout << w2 << " (delete " << difference << " at position " << ++j << ")" << endl;
				flag = false;
				break;
			}
		}
		if (flag) {
			difference.push_back(w1[j]);
			cout << w2 << " (delete " << difference << " at position " << ++j << ")" << endl;
		}
	}
	else if (w1.length() == w2.length()) {//substitution
		while (w1[j] == w2[j])
			j++;
		difference.push_back(w2[j]);
		old.push_back(w1[j]);
		cout <<w2<< " (change "<<old<<" at position "<<++j<< " to "<< difference<<")" << endl;
	}
	else {//insertion
		while (w1[j] == w2[j]) {
			j++;
			if (j == w1.length()) {
				break;
			}
		}
		difference.push_back(w2[j]);
		cout << w2 << " (insert " << difference << " after position " << j << ")" << endl;
	}
}

bool WordGraphs::checkifonediff(const string& word1, const string& word2) const{
	int s1 = word1.length(), s2 = word2.length();
	int difference = s1 - s2;
	if (difference >1 || difference <-1) {
		return false;
	}
	else {
		int i = 0, j = 0;
		int count = 0;
		while (i < s1 && j < s2) {
			if (word1[i] != word2[j]) {
				if (count == 1)
					return false;
				count++;
				if (s1 > s2)
					i++;
				else if (s1 < s2)
					j++;
				else {
					i++;
					j++;
				}
			}
			else {
				i++;
				j++;
			}
		}
		if (i < s1 || j < s2) {
			count++;
		}
		return (count == 1);
	}
}

void WordGraphs::resetEdges()
{
	while (!vertexestoreset.empty()) {
		vertex* it;
		it = vertexestoreset.front();
		it->dist = INT_MAX;
		it->path = nullptr;
		it->known = false;
		vertexestoreset.pop();

	}
	while (!bfsgraph.empty())
		bfsgraph.pop();
}
void WordGraphs::unweightedBFS(vertex *s,vertex*goal)
{
	vertex *v, *w;
	bfsgraph.push(s);
	s->dist = 0;
	vertexestoreset.push(s);
	while (!bfsgraph.empty()){
		v = bfsgraph.front();
;		bfsgraph.pop();
		v->known = true;
		for (int j = 0;j < v->edges.size();j++) {
			w = v->edges[j];
			if (w->dist == INT_MAX) {
				w->dist = v->dist + 1;
				w->path = v;
				vertexestoreset.push(w);
				bfsgraph.push(w);
			}
			if (w == goal)
				return;
		}
	}
}

void WordGraphs::findshortestpath(const string& w1, const string& w2)
{
	vertex *v=wordlist[w1], *w=wordlist[w2];
	unweightedBFS(wordlist[w1], wordlist[w2]);
	if(w->path==nullptr)
		cout<<"There is no sequence of one letter transformations from " << w1 << " to " << w2 << endl;
	else {
		stack<vertex*> path;
		path.push(w);
		while (w->path != v) {
			w = w->path;
			path.push(w);
		}
		string word1,word2;
		word1 = w1;
		cout << word1 << endl;
		while (!path.empty()) {
			word2 = path.top()->word;
			operationfinder(word1, word2);
			word1 = word2;
			path.pop();
		}
	}
	resetEdges();
}
#include "WordGraphs.h"


using namespace std;

int main() {
	string filename = "words.txt";
	ifstream in(filename);
	if (in.fail()) {
		cout << "File was not found or something else failed. Exiting." << endl;
		return 1;
	}
	else {
		WordGraphs w(in);
		string w1, w2;
		while (true) {
			cin >> w1 >> w2;
			if (w1[0] == '*')
				break;
			else {
				if (w.isindict(w1, w2))
					if ((w1 == w2))
						cout << "Words are same!" << endl;
					else
						w.findshortestpath(w1, w2);
			}
		}
	}
	return 0;
}
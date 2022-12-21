#include <queue>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


#define QI(num) Q##num
#define qQI(num) queue<Process *> Q##num

using namespace std;


struct Process {
	string pid;
	queue<char> * processlist;
	Process* next;
	Process(string pidp, queue<char> * plengthp,Process* nextp=NULL) :pid(pidp), processlist(plengthp), next(nextp) {}
};

void deleteProcess(Process * head) {
	Process* temp = head;
	while (temp->next != NULL) {
		temp=temp->next;
		delete head;
		head = temp;
	}
}
void deleteArray(vector<queue<char>*> vectorofqueues) {
	for (int a = 0;a < vectorofqueues.size();a++)
		delete vectorofqueues[a];
}

Process* Addtolist(Process* head, Process* newnode) {
	if (head == NULL) {
		head = newnode;
	}
	else {
		Process* temptr;
		temptr = head;
		while (temptr->next != NULL) {
			temptr = temptr->next;
		}
		temptr->next = newnode;
	}
	return head;
}

string getDirection() {
	string destination;
	cout << "Please enter the process folder name: ";
	cin >> destination;
	string filename = "configuration.txt";
	cout << "When all processes are completed, you can find execution sequence in" << endl;
	cout << "\"" << destination << "/output.txt\".";
	destination = destination + "\\";
	return destination;
}

int main() {
	int numberofqueues, numberofprocesses, sizeofS;
	ifstream input;
	ofstream output;
	string destination = getDirection();
	output.open(destination+"output.txt");
	input.open((destination+"configuration.txt").c_str());
	Process * head = NULL;
	vector<queue<char>*> vectorofqueues;
	vector<queue<Process*>> vectorofpriorities;
	if (input.fail()){
		cout << "Error while opening the config file!" << endl;
	}
	else {
		input >> numberofqueues >> numberofprocesses >> sizeofS;
		for (int k = 1; k <= numberofprocesses;k++) {
			string processname = "p";
			string processfilename = processname + to_string(k)+ ".txt";
			processname[0] = processname[0] - ('a' - 'A');
			processname = processname + "C" + to_string(k);
			ifstream input2;
			input2.open(destination+processfilename.c_str());
			if (input2.fail()) {
				cout << "Error while opening process " << k << " file" << endl;
			}
			else {
				char charinputting;
				queue<char> * QI(k) = new queue<char>;
				while (input2 >> charinputting) {
					QI(k)->push(charinputting);
				}
				vectorofqueues.push_back(QI(k));
				input2.close();
				Process* newnode = new Process(processname, QI(k), NULL);
				head = Addtolist(head, newnode);
			}

		}
		for (int q = 0;q <= numberofqueues;q++) {
			if (q == 0) {
				queue<Process *> QX;
				vectorofpriorities.push_back(QX);
			}
			else {
				qQI(q);
				vectorofpriorities.push_back(QI(q));
			}
		}
		Process* temptr=head;
		while( temptr!=NULL) {
			vectorofpriorities[numberofqueues].push(temptr);
			temptr=temptr->next;
		}
		int count = 0;
		while (vectorofpriorities[0].size() != numberofprocesses) {
			if (count == sizeofS) {
				count = 0;
				for (int l = 0;l < numberofprocesses - vectorofpriorities[numberofqueues].size() - vectorofpriorities[0].size();) {
					for (int g = numberofqueues-1;g >0;g--) {
						while (vectorofpriorities[g].size() != 0) {
							Process* temp = vectorofpriorities[g].front();
							vectorofpriorities[g].pop();
							output << "B, " << temp->pid + ", Q" << numberofqueues << endl;
							l++;
							vectorofpriorities[numberofqueues].push(temp);
						}

					}
				}
			}
			else {
				bool keeprunning = true;
				for (int g = numberofqueues;g>0;g--) {
					if(vectorofpriorities[g].size() != 0 && keeprunning) {
						Process* temp = vectorofpriorities[g].front();
						char tempchar = temp->processlist->front();
						temp->processlist->pop();
						vectorofpriorities[g].pop();
						if (temp->processlist->front() == '-') {
							output << "E, " << temp->pid << ", QX";
							vectorofpriorities[0].push(temp);
							//if (vectorofpriorities[0].size()!=numberofprocesses) {
							output << endl;
							//}
							
						}
						else if (tempchar == '0') {
							output << "0, " << temp->pid << ", Q" << to_string(g) << endl;
							vectorofpriorities[g].push(temp);
						}
						else if(tempchar=='1') {//if '1'
							if (g == 1) {
								output << "1, " << temp->pid << ", Q" << to_string(g) << endl;
								vectorofpriorities[g].push(temp);
							}
							else {
								output << "1, " << temp->pid << ", Q" << to_string(g - 1) << endl;
								vectorofpriorities[g - 1].push(temp);
							}
						}
						keeprunning = false;
						count++;
					}
				}
			}
		}
	}
	input.close();
	output.close();
	deleteProcess(head);
	deleteArray(vectorofqueues);
	return 0;
}
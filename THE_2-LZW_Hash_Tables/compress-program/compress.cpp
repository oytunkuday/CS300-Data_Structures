#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "HashTable.h"

using namespace std;

/*
This piece of code reads one file "tale" character by character and
writes them to another file "yaz". So in the end "yaz" is a copy of
"tale".

TAKEN FROM HW2 PDF AND UPGRADED
*/


using namespace std;
int main()
{
	pairs ITEM_NOT_FOUND;
	HashTable<pairs>* dictionary = new HashTable<pairs>(ITEM_NOT_FOUND, 4096);//creating a new hash table, sending a pair that we created upside as the
	char ch;
	ifstream input;
	ofstream output;
	string compin = "compin",compout= "compout";
	output.open(compout);
	input.open(compin.c_str());
	string tempstring;
	pairs temppair;
	for (int i = 0;i < 256;i++) {
		unsigned chr = char(i);
		tempstring.push_back(chr);
		temppair.code = i;
		temppair.key = tempstring;//adding the first 256 ascii elements
		dictionary->insert(temppair);
		tempstring.pop_back();//after we add a character to string array, we pop_back() so that it is empty again and we dont expand the existing string.
	}
	int counter = 256;//we start at the 256th index since 0....255 will be occupied by the ASCII table characters converted into strings.
	while (!input.eof()) //eof returns true if next character 
	 //is eof 
	{
		input.get(ch); //get command reads next character 
		//unless you are at the end of the file
		tempstring.push_back(ch);
		temppair.key = tempstring;//assigning values to temp struct that we will send to hashtable
		temppair.code = counter;
		while (dictionary->isInTable(temppair) == true && !input.eof()) {//unless the new string already in the table, we keep adding a char at the end of it.
			
			input.get(ch);
			tempstring.push_back(ch);
			temppair.key = tempstring;//assigning values to our temp struct in order to send to hashtable
		}
		pairs prevtemppair(counter,tempstring.substr(0,tempstring.length()-1));
		output << dictionary->find(prevtemppair).code << " ";//outputting the code that we found from the hashtable using find.

		dictionary->insert(temppair);
		counter++;//incrementing the counter after we add a new element so that we catch up the code at the struct.
		tempstring = tempstring.substr(tempstring.length()-1,1);//we keep going by assuming the last character of the string will be the start of our next string to be stored in dictionary
	}
	output.close();
	input.close();
	delete dictionary;//freeing the memory that we allocated from heap using new operator.
	return 0;
}
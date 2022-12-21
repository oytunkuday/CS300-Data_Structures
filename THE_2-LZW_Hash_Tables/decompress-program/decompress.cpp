#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

/*
This piece of code reads one file "tale" character by character and
writes them to another file "yaz". So in the end "yaz" is a copy of
"tale".


TAKEN FROM THE HW2 PDF AND UPGRADED
*/


using namespace std;
int main()
{
	string* array = new string[4096];//creating an array of 4096 strings
	int sayi;
	ifstream input;
	ofstream output;
	string compin = "compout", compout = "decompout";
	output.open(compout);
	input.open(compin.c_str());
	string tempstring;
	for (int i = 0;i < 256;i++) {//taking first 256 characters of ASCII table
		unsigned chr = char(i);
		tempstring.push_back(chr);
		array[i] = tempstring;
		tempstring.pop_back();
	}
	bool first = true;
	string prevtext;
	int code;
	int counter=256;//starting counter as 256 since the first string after the ASCII will be 256 (index starts from 0.... and goes to ...255
	while (input >>sayi){
		code = sayi;
		if(array[sayi]!= "") {//in dictionary
			if (first) {//there is an exception for the first integer we are going to decompress. It doesnt have a previous text We use a bool value to get over this problem.
				
				first = false;
				tempstring= array[sayi];
				output << tempstring;
			}
			else{
				tempstring = array[sayi];
				output << tempstring;
				array[counter] = prevtext+tempstring.substr(0,1);
				counter++;//we increase the counter when we add a new element to array since the new index that the next element is going to be added will increase.
			}			
		}
		else if (array[sayi] == "") {//not in dictionary
			tempstring = prevtext+ prevtext.substr(0,1);
			array[sayi] = tempstring;
			output << tempstring;
			counter++; 
		}
		prevtext = tempstring;
		
	}
	output.close();
	input.close();
	delete[] array;//freeing the memory that was allocated from heap with new operator.
	return 0;
}
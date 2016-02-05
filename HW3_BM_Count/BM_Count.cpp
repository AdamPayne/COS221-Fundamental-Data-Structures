#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;

int max(int a, int b) { return (a > b) ? a : b; }

// setting the positions array (the BM constructor from the presentation)
void init(string, int[]);
// the actual search, which returns the number of occurences (the ++ operator from the presentation slightly modified)
int search(string, string);

int main(){
	ifstream test;
	string txt;
	vector<string> words;
	vector<int> frequency;
	test.open("test.txt", ios::in);
	// read one line from the file
	getline(test, txt);
	test.close();
	// transform the text to lowercase for easier matching and processing as a whole
	transform(txt.begin(), txt.end(), txt.begin(), tolower);
	// the last element shouldn't be an alphanumerical character otherwise the for loop would go in an infinite cycle and it would look ugly to be patched within it
	txt.push_back('.');
	// Table:
	cout << setw(30) << left << "Word" << setw(30) << "| Count" <<'\n' ;
	cout << setfill('-') << setw(60) << "" << setfill(' ') << '\n';
	for (string::iterator it = txt.begin(); it != txt.end(); it++){
		string word;
		while ((*it >= 'a' && *it <= 'z') || (*it >= '0' && *it <= '9')){
			// accumulate the characters of the world until a character which is not alphanumerical is reached
			word.push_back(*it);
			// if the last element of the text was an alphanumerical character it would become equal to txt.end() after this incrementation, but it would be also increment in the for loop so it would become different than the end and the loop won't end
			it++;
		}
		// test if the world is already in the list
		if (find(words.begin(), words.end(), word) == words.end() && !word.empty()){
			// add the new word to the list
			words.push_back(word);
			// compute the frequency and add it
			frequency.push_back(search(txt, word));
			// output the line in the table
			cout << setw(30) << words.back() << "| " << setw(28) << frequency.back() << '\n';
			cout << setfill('-') << setw(60) << "" << setfill(' ') << '\n';
		}
	}
	system("pause");
	return 0;
}

void init(string word, int positions[]){
	for (int i = 0; i < 256; i++) positions[i] = 0;
	for (int i = 0; word[i]; i++) positions[word[i]] = i;
}

int search(string txt, string word){
	int pos = 0, count=0, last=txt.length()-word.length();
	int positions[256];
	// initialize the positions array
	init(word, positions);
	while (pos < last){
		int j = word.length() - 1;
		// check if the processed block is surrounded by not alphanumerical characters so it is a word or a series of words and it is worth to process it, else increment pos with 1
		if ((txt[pos + j + 1] < '0' || txt[pos + j + 1] > 'z' || (txt[pos + j + 1] > '9' && txt[pos + j + 1] < 'a')) &&
			(txt[pos - 1] < '0' || txt[pos - 1] > 'z' || (txt[pos - 1] > '9' && txt[pos - 1] < 'a'))){
			while ((j >= 0) && word[j] == txt[pos + j])j--;
			if (j < 0){
				// go over to the next block
				pos += word.length();
				// increment the count
				count++;
			}
			else{
				pos += max(1, j - positions[txt[pos + j]]);
			}
		}
		else{
			pos++;
		}
	}
	return count;
}
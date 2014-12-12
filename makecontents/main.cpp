#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

struct entry {
	int indent;
	int pageNumber;
	std::string title;
};

using namespace std;

void printEntry(entry e, int bookLength) {
	int totalPad = ceil(log10(bookLength+1));
	int pnLength = ceil(log10(e.pageNumber+1));
	int pad = totalPad - pnLength;
	cout << e.pageNumber;
	for (int i = 0; i < pad; ++i) {
		cout << " ";
	}
	cout << "| ";
	for (int i = 0; i < e.indent; ++i) {
		cout << " ";
	}
	cout << e.title << endl;
	return;
}

int main (int argc, char * const argv[]) {
	if (argc < 2)
		return 0;
    vector<entry> entries;
	string line;
	ifstream myfile(argv[1]);
	getline(myfile, line);
	int numEntries = atoi(line.c_str());
	int highestPageNum = 0;
	for (int i = 0; i < numEntries; ++i) {
		entry newEntry;
		getline(myfile, line);
		newEntry.title = line;
		getline(myfile, line);
		newEntry.indent = atoi(line.c_str());
		getline(myfile, line);
		newEntry.pageNumber = atoi(line.c_str());
		if (newEntry.pageNumber > highestPageNum)
			highestPageNum = newEntry.pageNumber;
		entries.push_back(newEntry);
	}
	for (int i = 0; i < numEntries; ++i) {
		printEntry(entries[i], highestPageNum);
	}
    return 0;
}

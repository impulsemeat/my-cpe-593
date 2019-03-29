#include<iostream>
#include<cstdlib>
#include<string>
#include<fstream>
#include<vector>

using namespace std;

class Trie {
private:
	class Node {
	public:
		Node* next[26];
		bool isWord;
	};

	Node root;
public:
	Trie() {
		root.isWord = false;
		for (int i = 0; i < 26; i++)
			root.next[i] = nullptr;
	}
	~Trie() {}
	void insert(string word) {
		char *cstr = new char[word.size() + 1];
		word.copy(cstr, word.size() + 1);
		cstr[word.size()] = '\0';
		Node* p = &root;
		for (int i = 0; i < strlen(cstr); i++) {
			if (p->next[cstr[i] - 97] == nullptr)
				p->next[cstr[i] - 97] = new Node();
			p = p->next[cstr[i] - 97];
		}
		p->isWord = true;
		delete cstr;
	}

	bool contains(string word) {
		char *cstr = new char[word.size() + 1];
		word.copy(cstr, word.size() + 1);
		cstr[word.size()] = '\0';
		Node* p = &root;
		for (int i = 0; i < strlen(cstr); i++) {
			if (p->next[cstr[i] - 97] == nullptr)
				return false;
			p = p->next[cstr[i] - 97];
		}
		delete cstr;
		return p->isWord;
	}

	bool containsPrefix(string word) {
		char *cstr = new char[word.size() + 1];
		word.copy(cstr, word.size() + 1);
		cstr[word.size()] = '\0';
		Node* p = &root;
		for (int i = 0; i < strlen(cstr); i++) {
			if (p->next[cstr[i] - 97] == nullptr)
				return false;
			p = p->next[cstr[i] - 97];
		}
		delete cstr;
		return true;
	};
};

int main() {
	Trie MyDict;
	ifstream inFile("dict.txt", ios::in);
	if (!inFile) {
		cerr << "File could not be opened" << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
	while (!inFile.eof()) {
		string inWord;
		inFile >> inWord;
		MyDict.insert(inWord);
	}
	inFile.close();
	cout << "The dictionary is loaded." << endl;
	char choice;
	do
	{
		cout << "Please seclect follow options to operate the dictionary\n"
			<< "S for searching a word in the dicitonay\n"
			<< "P for finding a prefix\n"
			<< "Q for exit\n";
		cin >> choice;
		choice = toupper(choice);
		switch (choice)
		{
		case 'S': {
			cout << "please enter a word you would like to search without white space:\n";
			string entry;
			cin >> entry;
			if (MyDict.contains(entry))
				cout << "The word is in the dictionary\n" << endl;
			else
				cout << "The word is not found in the dictionary\n" << endl;
			break;
		}
		case 'P': {
			cout << "please enter a prefix you would like to search without white space:\n";
			string entry;
			cin >> entry;
			if (MyDict.containsPrefix(entry))
				cout << "The prefix is in the dictionary\n" << endl;
			else
				cout << "The prefix is not found in the dictionary\n" << endl;
			break;
		}
		case 'Q':
		{
			cout << "Exiting\n";
			break;
		}
		default:
		{
			cout << "Wrong Option";
			break;
		}
		}
	} while (choice != 'Q');
	system("PAUSE");
	return EXIT_SUCCESS;
}

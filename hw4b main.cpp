#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>

using namespace std;

class DoubleLinkedList2 {
private:
	class Node {
	public:
		int data;
		Node* next;
		Node* prev;
	};
	Node* head;
	Node* tail;
public:
	DoubleLinkedList2() : head(nullptr), tail(nullptr) {}
	~DoubleLinkedList2() {

	}
	void addEnd(int v) {
		Node* temp = new Node();
		temp->data = v;
		temp->next = nullptr;
		temp->prev = tail;
		if (tail == nullptr) {
			head = tail = temp;
		}
		else {
			tail->next = temp;
			tail = temp;
		}
	}
	void addStart(int v) {
		Node* temp = new Node();
		temp->data = v;
		temp->next = head;
		temp->prev = nullptr;
		if (head == nullptr) {
			head = tail = temp;
		}
		else {
			head->prev = temp;
			head = temp;
		}
	}

	void removeStart() {
		if (head == nullptr)
			return;
		if (head == tail) {
			delete head;
			head = tail = nullptr;
			return;
		}
		Node* temp = head;
		head = head->next;
		head->prev = nullptr;
		delete temp;
	}

	bool isEmpty() {
		return head == nullptr;
	}

	void removeEnd() {
		if (tail == nullptr)
			return;
		if (head == tail) {
			delete head;
			head = tail = nullptr;
			return;
		}
		Node* secondToLast = tail->prev;
		delete tail;
		secondToLast->next = nullptr;

	}
	friend ostream& operator <<(ostream& s, const DoubleLinkedList2& list) {
		for (Node* p = list.head; p != nullptr; p = p->next)
			s << p->data << ' ';
		return s;
	}
	class Iterator {
	private:
		Node* current;
	public:
		Iterator(const DoubleLinkedList2& orig) : current(orig.head) {}
		bool operator !() const { return current != nullptr; }
		Iterator& operator ++() {
			current = current->next;
			return *this;
		}
		int operator *()const { return current->data; }
	};
};
	enum instruction {
		ADD_FRONT,
		ADD_BACK,
		REMOVE_FRONT,
		REMOVE_BACK,
		OUTPUT,
		NONE
	};
	instruction stringToEnum(string s) {
		instruction result;
		if (s == "ADD_FRONT")
			result = ADD_FRONT;
		else if (s == "ADD_BACK")
			result = ADD_BACK;
		else if (s == "REMOVE_FRONT")
			result = REMOVE_FRONT;
		else if (s == "REMOVE_BACK")
			result = REMOVE_BACK;
		else if (s == "OUTPUT")
			result = OUTPUT;
		else
			result = NONE;
		return result;
	}

	int main()
	{
		DoubleLinkedList2 myList;
		ifstream inFile("HW4b.txt", ios::in);
		if (!inFile) {
			cerr << "File could not be opened" << endl;
			exit(EXIT_FAILURE);
		}
		while (inFile.peek() != EOF)
		{

			string theInstruction;
			instruction instruct;
			inFile >> theInstruction;
			instruct = stringToEnum(theInstruction);
			switch (instruct)
			{
			case ADD_FRONT:
			{
				int arr[3], i = 0;
				while (inFile.peek() != '\n') {
					if (inFile.peek() != ':')
						inFile >> arr[i++];
					else {
						char col;
						inFile >> col;
					}
				}
				for (arr[0]; arr[0] <= arr[2]; arr[0] = arr[0] + arr[1])
					myList.addStart(arr[0]);
				break;
			}
			case ADD_BACK:
			{
				int arr[3], i = 0;
				while (inFile.peek() != '\n') {
					if (inFile.peek() != ':')
						inFile >> arr[i++];
					else {
						char col;
						inFile >> col;
					}
				}
				for (arr[0]; arr[0] <= arr[2]; arr[0] = arr[0] + arr[1])
					myList.addEnd(arr[0]);
				break;
			}
			case REMOVE_FRONT:
			{
				int n;
				inFile >> n;
				for (int i = 0; i < n; i++)
					myList.removeStart();
				break;
			}
			case REMOVE_BACK:
			{
				int n;
				inFile >> n;
				for (int i = 0; i < n; i++)
					myList.removeEnd();
				break;
			}
			case OUTPUT:
			{
				for (DoubleLinkedList2::Iterator i = myList; !i; ++i)
					cout << *i << ' ';
				break;
			}
			default:
				break;
			}

		}
		inFile.close();
		system("PAUSE");
		return EXIT_SUCCESS;
	}
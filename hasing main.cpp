#include <cstdlib> 
#include <string>
//#include <vector>
#include <fstream>
#include <iostream>
#include <stdint.h>
using namespace std;

template <typename T>
class GrowArray
{
private:
	typedef T value_type;
	value_type* p;
	int used;
	int capacity;
	void grow() {
		value_type* old = p;
		capacity = 2 * capacity + 1;
		p = new value_type[capacity];
		if (old != nullptr) {
			for (int i = 0; i < used; i++)
				p[i] = old[i];
		}
		delete old;
	}
public:
	GrowArray() {
		p = nullptr;
		used = 0;
		capacity = 0;
	}
	GrowArray(int init_capa) {
		p = new value_type[init_capa];
		used = 0;
		capacity = init_capa;
	}
	~GrowArray() {
		delete[] p;
	}
	void addEnd(value_type v) {
		if (used >= capacity)
			grow();
		p[used++] = v;
	}

	void addStart(value_type v) {
		if (used >= capacity)
			grow();
		used++;
		for (int i = used; i > 0; i--)
			p[i] = p[i - 1];
		p[0] = v;
	}
	void removeEnd() {
		delete p[--used];
	}

	void removeStart() {
		for (int i = 0; i < used - 1; i++)
			p[i] = p[i + 1];
		delete p[--used];
	}
	void removeOne(int x) {
		if (x >= used) {
			cout << "Exceed the max used value" << used << endl;
			return;
		}
		value_type* temp = &p[x];
		for (int i = x; i < used - 1; i++)
			p[i] = p[i + 1];
		removeEnd();
		delete temp;
	}
	int getSize() {
		return used;
	}
	bool empty() {
		return (p == nullptr || used == 0 || capacity == 0);
	}
	value_type& operator[] (int i) const {
		if (i >= used)
		{
			cout << "Exceed the max used value" << used << endl;
			return p[0];
		}
		return p[i];
	}
	friend value_type getOne(const GrowArray &a, int i) {
		if (i >= a.used)
		{
			cout << "Exceed the max used value" << a.used << endl;
			return a[0];
		}
		return a[i];
	}
	friend int getSize(GrowArray* a) {
		return a->getSize();
	}
};
// SuperFastHash func from reference
#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif
uint32_t SuperFastHash(const char * data, int len) {
	uint32_t hash = len, tmp;
	int rem;

	if (len <= 0 || data == NULL) return 0;

	rem = len & 3;
	len >>= 2;

	/* Main loop */
	for (; len > 0; len--) {
		hash += get16bits(data);
		tmp = (get16bits(data + 2) << 11) ^ hash;
		hash = (hash << 16) ^ tmp;
		data += 2 * sizeof(uint16_t);
		hash += hash >> 11;
	}

	/* Handle end cases */
	switch (rem) {
	case 3: hash += get16bits(data);
		hash ^= hash << 16;
		hash ^= ((signed char)data[sizeof(uint16_t)]) << 18;
		hash += hash >> 11;
		break;
	case 2: hash += get16bits(data);
		hash ^= hash << 11;
		hash += hash >> 17;
		break;
	case 1: hash += (signed char)*data;
		hash ^= hash << 10;
		hash += hash >> 1;
	}

	/* Force "avalanching" of final 127 bits */
	hash ^= hash << 3;
	hash += hash >> 5;
	hash ^= hash << 4;
	hash += hash >> 17;
	hash ^= hash << 25;
	hash += hash >> 6;

	return hash;
}

template<size_t TABLE_SIZE>
int hashkey(const string& s)
{
	//place holder
	int n = s.length();
	//char str[n + 1];
	//strcpy(str, s.c_str());
	return SuperFastHash(s.c_str(), n);
}
typedef int(*hkey)(const string&);
template<size_t TABLE_SIZE, hkey hash>
class stringHash
{
private:
	GrowArray<string>* data[TABLE_SIZE];
	hkey hash;
	int collision, totalNum;
public:
	stringHash() : collision(0), totalNum(0) {
		for (GrowArray<string>* n : data)
			n = nullptr;
	}
	~stringHash() {
		for (GrowArray<string>* n : data)
			delete n;
	}
	void insert(const string& entry){
		int key = (*hash)(entry);
		if (!find(entry))
		{
			if (data[key] == nullptr)
				data[key] = new GrowArray<string>;
			else if (!data[key]->empty())
				collision++;
			data[key]->addEnd(entry);
			totalNum++;
			cout << "The word, " << entry << ", has been added successfully\n";
		}
		else 
			cout << "The word, " << entry << ", is already on the record\n";
	}
	void remove(const string& entry) {
		bool result = false;
		int key = (*hash)(entry);
		size_t i = 0;
		for (string s : *(data[key]))
		{
			if (s == entry)
			{
				result = true;
				break;
			}
			i++;
		}
		if (result)
		{
			data[key]->removeOne(i);
			if (!data[key]->empty())
				collision--;
			else
			{
				GrowArray<string> *temp;
				temp = data[key];
				data[key] = nullptr;
				delete temp;
			}
			totalNum--;
			cout << "The word, " << entry << ", is removed\n";
			return;
		}
		else
		{
			cout << "The word, " << entry << ", does not exist\n";
			return;
		}
	}
	bool find(const string& entry) {
		bool result = false;
		int key = (*hash)(entry);
		for ( int i = 0; i < getSize(data[key]); i++)
		{
			if (getOne(*data[key], i) == entry)
			{
				result = true;
				break;
			}
		}
		return result;
	}
	int size() const { return totalNum; }
	int collisions() const { return collision; }
	//
	//int const table_size = (int)TABLE_SIZE;
	void histogram() {
		if (totalNum == 0) {
			cout << "It is still empty\n";
			return;
		}
		cout << "insert    count\n" << endl;
		for (int i = 0; i < TABLE_SIZE; i++)
		{
			cout << i << '\t' << data[i]->getSize() << endl;
		}

	}
};


int main() {
	ifstream inFile("dict.txt", ios::in);
	if (!inFile) {
		cerr << "File could not be opened" << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
	size_t const dic_size = 653;
	stringHash<dic_size, hashkey<dic_size>> dictionary;
	int u = 0;
	while (!inFile.eof())
	{
		string entry;
		inFile >> entry;
		dictionary.insert(entry);
		u++;
	}
	cout << "File import completed\n";
	cout << "The collision found now is " << dictionary.collisions() << endl;
	cout << "The total number of words added from file is " << u << endl;
	cout << "The total number of words in the dictionary is " << dictionary.size() << endl;
	inFile.close();
	inFile.open("hw8.dat", ios::in);
	if (!inFile) {
		cerr << "File hw8.dat could not be opened" << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}else
		cout << endl << "Now, hw8.dat is open\n";
	while (!inFile.eof()) {
		string entry;
		inFile >> entry;
		if (dictionary.find(entry))
			cout << entry << " TRUE\n";
		else
			cout << entry << " FALSE\n";
	}
	inFile.close();
	cout << "\nhw8.dat is closed\n";
	dictionary.histogram();

	
	system("PAUSE");
	return EXIT_SUCCESS;
}
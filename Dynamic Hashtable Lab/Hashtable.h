
#include <vector>
#include <memory>

template <class T>
struct Bucket {
	T value;
	bool occupied;
	bool deleted;
	Bucket() : occupied(false), deleted(false) {}
};

template <class T>
class Hashtable {
public:
	Hashtable(int cap = 17, double loadFactor = 0.65);
	Hashtable(const Hashtable& other);
	Hashtable& operator=(const Hashtable& other);
	~Hashtable();

	void insert(const T& value);
	void remove(const T& value);
	bool contains(const T& value);
	int indexOf(const T& value);
	void clear();

	bool empty() const;
	int getSize() const;
	int getCapacity() const;
	double getLoadFactorThreshold() const;

	std::vector<Bucket<T>> table;
	int count;
	double loadFactorThreshold;


	int hash(const T& value, int attempt) const;
	void rehash();
	static bool isPrime(int num);
	static int nextPrime(int num);
};

template<class T>
Hashtable<T>::Hashtable(int cap, double loadFactor) : table(cap), loadFactorThreshold(loadFactor), count(0) {}

template<class T>
Hashtable<T>::Hashtable(const Hashtable& other) : table(other.table), loadFactorThreshold(other.loadFactorThreshold), count(other.count) {}

template<class T>
Hashtable<T>& Hashtable<T>::operator=(const Hashtable& other) {
	if (this != &other) {
		table = other.table;
		loadFactorThreshold = other.loadFactorThreshold;
		count = other.count;
	}
	return *this;
}

template<class T>
Hashtable<T>::~Hashtable() {}

template<class T>
int Hashtable<T>::hash(const T& value, int attempt) const {
	return (std::hash<T>()(value) + attempt * attempt) % table.size();
}

template<class T>
void Hashtable<T>::rehash() {
	int newCapacity = nextPrime(table.size() * 2);
	std::vector<Bucket<T>> newTable(newCapacity);
	
	for (auto& entry : table) {
		if (entry.occupied && !entry.deleted) {  
			int index = hash(entry.value, 0); 
			int i = 0;
			while (newTable[(index + i * i) % newCapacity].occupied) {
				i++;
			}
			newTable[(index + i * i) % newCapacity].value = entry.value;
			newTable[(index + i * i) % newCapacity].occupied = true;
		}
	}
	table = std::move(newTable);
}

template<class T>
bool Hashtable<T>::isPrime(int num) {
	if (num < 2) return false;
	if (num == 2 || num == 3) return true;
	if (num % 2 == 0) return false;
	for (int i = 3; i * i <= num; i += 2) {
		if (num % i == 0) return false;
	}
	return true;
}

template<class T>
int Hashtable<T>::nextPrime(int num) {
	while (!isPrime(num)) {
		num++;
	}
	return num;
}

template<class T>
void Hashtable<T>::insert(const T& value) {
	if (count >= loadFactorThreshold * table.size()) {
		rehash();  
	}
	int index = hash(value, 0);
	int i = 0;
	while (table[(index + i * i) % table.size()].occupied) {
		i++;
	}
	table[(index + i * i) % table.size()].value = value;
	table[(index + i * i) % table.size()].occupied = true;
	count++;
}

template<class T>
void Hashtable<T>::remove(const T& value) {
	int index = hash(value, 0);
	int i = 0;

	while (table[(index + i * i) % table.size()].occupied) {
		if (table[(index + i * i) % table.size()].value == value) {
			table[(index + i * i) % table.size()].deleted = true;
			count--;
			return;
		}
		i++;
	}
}

template<class T>
bool Hashtable<T>::contains(const T& value) {
	return indexOf(value) != -1;
}

template<class T>
int Hashtable<T>::indexOf(const T& value) {
	int index = hash(value, 0); 
	int i = 0;
	while (table[(index + i * i) % table.size()].occupied) { 
		if (table[(index + i * i) % table.size()].value == value &&
			!table[(index + i * i) % table.size()].deleted) { 
			return (index + i * i) % table.size();
		}
		i++;
	}
	return -1;
}

template<class T>
void Hashtable<T>::clear() {
	table.clear();
	table.resize(17);
	count = 0;
}

template<class T>
bool Hashtable<T>::empty() const {
	return count == 0;
}

template<class T>
int Hashtable<T>::getSize() const {
	return count;
}

template<class T>
int Hashtable<T>::getCapacity() const {
	return table.size();
}

template<class T>
double Hashtable<T>::getLoadFactorThreshold() const {
	return loadFactorThreshold;
}

template class Hashtable<int>;
template class Hashtable<double>;

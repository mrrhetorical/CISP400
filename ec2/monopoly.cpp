// monopoly.cpp
// Caleb Brock, CISP 400
// May 18, 2023

#include <iostream>
#include <stdexcept>

using namespace std;

// used to cache the amount of times a square is landed upon
template <typename K, typename V> class Map {
private:
	struct Bucket {
		K key;
		V value;
		Bucket* next = nullptr;
	};
	Bucket** buckets;
	int size;
	static const int max_size = 32;
	static int hash(K key) {
		return (key * 17) % max_size;
	}
public:
	Map() {
		size = 0;
		buckets = new Bucket*[max_size];
		for (int i = 0; i < max_size; i++) {
			buckets[i] = nullptr;
		}
	};
	V get(K key) {
		int index = hash(key);
		if (buckets[index] != nullptr) {
			if (buckets[index]->key == key) {
				return buckets[index]->value;
			} else {
				Bucket* ptr = buckets[index];
				while (ptr->next != nullptr) {
					ptr = ptr->next;
					if (ptr->key == key) {
						return ptr->value;
					}
				}
			}
		}
		throw invalid_argument("No such key exists in the map!");
	};
	void put(K key, V value) {
		Bucket* bucket = new Bucket {
			key = key,
			value = value
		};
		int index = hash(key);
		if (buckets[index] != nullptr) {
			Bucket* ptr = buckets[index];
			while (ptr->next != nullptr) {
				ptr = ptr->next;
			}
			ptr->next = bucket;
		} else {
			buckets[index] = bucket;
		}

		size++;
	};
	int getSize() {
		return size;
	}

};

int rollDie();

int main() {

	Map<int, int>* map = new Map<int, int>();
	map->put(1, 13);

	cout << map->get(1) << endl;
	cout << map->get(0) << endl;

	return 0;
}

int rollDie() {
	return (rand() % 6) + 1;
}
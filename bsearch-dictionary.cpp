
// Implementation of a dictionary using an array and binary search
// It will inherit from the ArrayDictionary

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bsearch-dictionary.h"

// Constructor
BinarySearchDictionary::BinarySearchDictionary()
{
	sorted = false;
}

bool
BinarySearchDictionary::addRecord( KeyType key, DataType record) {
	sorted = false;
	return ArrayDictionary::addRecord(key, record);
}

// Find a key in the dictionary and return corresponding record or NULL
	DataType
BinarySearchDictionary::findRecord( KeyType key)
{
	if (!sorted) {
		sort();
		sorted = true;
	}

	// Use binary search

	// Add your implementation here
	int min = 0;
	int max = currentNumber-1;
	int middle = 0;
	int store = -1; 
	while (max >= min) {
		middle = (min + max) / 2; 
		if (strcmp(key, array[middle].key) == 0) {
			store = 0;
			break;
		}
		if (strcmp(key, array[middle].key) < 0)
			max = middle - 1; 
		if (strcmp(key, array[middle].key) > 0)
			min = middle + 1; 
	}
	if (store == -1)	
		return NULL;
	return array[middle].data; 
}


// Sort array using heap sort
	void
BinarySearchDictionary::sort()
{
	//printf("Before\n");
	//for (int i = 0; i < currentNumber; i++) {
	//	printf("%s\n", array[i].key);
	//}
	for (int i = currentNumber / 2 - 1; i >= 0; i--) {
		heapify(currentNumber, i); 

	}
	for (int i = currentNumber - 1; i >= 0; i--) {
		ArrayDictionaryNode temp = array[0];
		array[0] = array[i];
		array[i] = temp;
		heapify(i, 0); 
	}


}


void BinarySearchDictionary :: heapify(int n, int i) {
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	if (left < n && strcmp(array[left].key, array[largest].key) > 0)
		largest = left;
	if (right < n && strcmp(array[right].key, array[largest].key) > 0)
		largest = right;
	if (largest != i) {
		ArrayDictionaryNode temp = array[i];
		array[i] = array[largest];
		array[largest] = temp;
		heapify(n, largest); 
	}

}


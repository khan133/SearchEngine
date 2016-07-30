
// Implementation of a dictionary using a hash table

#include <stdlib.h>
#include <string.h>
#include "hash-dictionary.h"

// Constructor
HashDictionary::HashDictionary()
{
  // Add your code here
	int i = 0;
	while (i < MaxBuckets) {
		buckets[i++] = NULL; 
}

}

int
HashDictionary::hash(KeyType key) {
  // Add your code here
  int sum = 0;
  int i = 0;
  int length = strlen(key); 
  while (i < length) {
  	sum += i * key[i];
	i++; 
  }
  return sum%MaxBuckets;
}

// Add a record to the dictionary. Returns false if key already exists
bool
HashDictionary::addRecord( KeyType key, DataType record)
{
  // Add your code here
  int num = hash(key);
  HashNode * curr = buckets[num];
  HashNode * temp = new HashNode();
  temp -> key = strdup(key);
  temp -> data = record; 
  bool added = true; 
  while (curr != NULL) {
  	if (strcmp(curr -> key, key) == 0) {
		curr -> data = record;
		added = false; 		
		curr = curr -> next;
		continue; 
	}
	curr = curr -> next; 
  }
	if (added == false)
		return false; 
	temp -> next = buckets[num];
	buckets[num] = temp; 
	nElements++; 
	return added; 
  
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
HashDictionary::findRecord( KeyType key)
{
	int num = hash(key);
	HashNode * curr = buckets[num];
	while (curr != NULL) {
		if (strcmp(curr -> key, key) == 0) {
			return curr -> data;
		}
		else
		curr = curr -> next;
	}
	return NULL;
}

// Removes one element from the table
bool
HashDictionary::removeElement(KeyType key)
{
  // Add your code here
	int num = hash(key); 
	HashNode * curr = buckets[num];
	HashNode * before = NULL;
	int count = 0; 
	while (curr != NULL) {
		if (strcmp(curr -> key, key) == 0) {
			if (count > 0) {
				before -> next = curr -> next; 
			}
			if (count== 0) 
				buckets[num] = curr -> next;
			nElements = nElements - 1; 
			delete before;
			delete curr;
			return true; 
		}

		before = curr;
		curr = curr -> next; 
	}

	return false;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
HashDictionary::keys(int * n)
{
	KeyType * a = (KeyType *) malloc(nElements * sizeof(KeyType));

	*n = nElements;
	int i = 0;
	for (int h = 0; h < MaxBuckets; h++) {
		HashNode * n = buckets[h];
		while (n!=NULL) {
			a[i] = n->key;
			i++;
			n = n->next;
		}
	}
	*n = nElements;
	return a;
}


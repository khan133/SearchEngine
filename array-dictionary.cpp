
// Implementation of a dictionary using an array and sequential search
// The array will be dynamically resized if necessary

#include <stdlib.h>
#include <string.h>

#include "array-dictionary.h"

// Constructor
ArrayDictionary::ArrayDictionary()
{
  // Add your code here
	currentNumber = 0;
	maxNumber = 500;
	array = new ArrayDictionaryNode[maxNumber];
	int i = 0;
}

// Add a record to the dictionary.Returns false if key already exists
bool
ArrayDictionary::addRecord( KeyType key, DataType record)
{
        // Add your code here
	int i = 0;
	while (i < currentNumber) {
		if (strcmp(array[i].key, key) == 0) 
		{
		array[i].data = (DataType*) record;
		return false;
		}
		i++; 
	}
	array[currentNumber].key = (char*) malloc(sizeof(char) * 50);
	strcpy(array[currentNumber].key,key);
	array[currentNumber].data = (DataType*) record;
	currentNumber = currentNumber + 1;
	return true;
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
ArrayDictionary::findRecord( KeyType key)
{
        // add your code here

	int i = 0;
	while (i < currentNumber) {
		KeyType newkey = array[i].key; 
		if (strcmp(newkey, key) == 0 ) {
			return array[i].data; 
		}
		else {
			i++; 
		}

	}
	return NULL;
}

// Removes one element from the table
bool
ArrayDictionary::removeElement(KeyType key)
{
        // Add your code here
	bool inTable = false;
	int i = 0;
	int count = 0;
	while (i < currentNumber) {
		if (strcmp(array[i].key, key) != 0) {
			count++;
		}
		else {
			break;
		}
		i++;
	}
	if (count == currentNumber) 
		return false; 
	ArrayDictionaryNode * newtable = new ArrayDictionaryNode[maxNumber];
	count = 0;
	int j = 0;
	while (j < currentNumber) {
		if (strcmp(array[j].key,key) != 0) {
			newtable[count].key = (char *) malloc(sizeof(char) * 50);
			strcpy(newtable[count].key, array[j].key);
			newtable[count].data = array[j].data; 
			count++;
			j++;
		}
		else {
			j++;
			continue; 
		}
		
	}
	currentNumber = count; 
	delete [] array;
	array = newtable;
	return true;
	
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
ArrayDictionary::keys(int * n)
{
        // Add yoru code here
	int i = 0;
	KeyType * newkey = new KeyType[currentNumber];
	while (i < currentNumber) {
		newkey[i] = (char *) malloc(sizeof(char) * 50);
		strcpy((char*) newkey[i],array[i].key);
		i++;
		}
	*n = i; 
	return newkey; 
}

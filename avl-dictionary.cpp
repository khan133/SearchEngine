
// Implementation of a dictionary using an AVL tree
// AVL Trees are balanced binary trees 

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "avl-dictionary.h"

bool debug = false;

// Constructor
AVLDictionary::AVLDictionary()
{
	root = NULL;
	nElements = 0;
}

// Add a record to the dictionary. Returns false if key already exists
	bool
AVLDictionary::addRecord( KeyType key, DataType record)
{
	if ( debug) {
		printf("------------------------------------\n");
		printf("addRecord(\"%s\",%ld)\n",  key, (long) record);
		printf("---------- Before -----------------\n");
		printNode("", root, 0);
	}

	// Add your implementation here
	AVLNode * node = new AVLNode(); 
	node -> key = key;
	node -> height = 1;
	node -> data = record;
	node -> parent = NULL;
	node -> right = NULL;
	node -> left = NULL;

	if (root == NULL) {
		root = node;
		nElements = nElements+1;
		return true; 
	}
	AVLNode * curr = root;
	AVLNode * prev = curr; 
	int count = 0;
	while (curr != NULL) {
		prev = curr; 
		if (strcmp(key, curr -> key) < 0)
			curr = curr -> left;
		else if (strcmp(key, curr -> key) > 0)
			curr = curr -> right;
		else {
			curr -> data = record; 
			return false;  
		}
	}
	if (count > 0)
		return false; 

	//Find node to insert into 
	//Node does not exist. Create it.
	//Height might not be valid anymore.
	//We need to restructure .
	if (strcmp(key, prev -> key) <= 0) {
		prev -> left = node;
	}
	else {
		prev -> right = node; 
	}
	node -> parent = prev;
	int max = 0;
	AVLNode * par = node -> parent; 
	while (par != NULL) {
		max = 0; 
		if (par -> left != NULL)
			max = par -> left -> height;
		if (par -> right != NULL && par -> right -> height > max)
			max = par -> right -> height; 
		par -> height = max + 1;
		par = par -> parent; 
	}


	if ( debug) {
		printf("---------- Before Restructure -----------------\n");
		printNode("", root, 0);
	}

	// Call restructure
	if (debug) {
		checkRecursive(root);

		printf("---------- After Restructure -----------------\n");
		printNode("", root, 0);
	}
	nElements++; 
	return true;
}

void
AVLDictionary::restructure(AVLNode * n) {
	//Balance the tree starting at n 

	//Go upwards until root is found

	// See class notes

	// Add your implementation here


	AVLNode * z = n;
	int heightleft;
	while (z != NULL) {
		printf("loop"); 
		heightleft = 0;
		if (z -> left != NULL)
			heightleft = z -> left -> height;
		int heightright = 0;
		if (z -> right != NULL)
			heightright = z -> right -> height; 
		int diff = heightleft - heightright; 
		if (diff < 0)
			diff = diff * -1; 
		if (heightleft > heightright) 
			z -> height = 1 + heightleft;
		else 
			z -> height = 1 + heightright;
		if (diff <= 1) {
			z = z -> parent;
			continue; 
		}
		AVLNode * y = NULL; 
		int maxheight = 0;
		if (z -> left != NULL) {
			y = z -> left;
			maxheight = y -> height; 

		}
		if (z -> right != NULL && z -> right -> height > maxheight) {
			y = z -> right;
			maxheight = y -> height; 

		}
		assert(y != NULL); 
		AVLNode * x = NULL; 
		maxheight = 0; 
		if (y -> left != NULL) {
			x = y -> left;
			maxheight = x -> height; 

		}
		if (y -> right != NULL && y -> right -> height > maxheight) {
			x = y -> right;
			maxheight = x -> height; 
		}
		assert(x != NULL);
		AVLNode * a, *b, * c, *t0, *t1, *t2, *t3; 
		if (z -> right == y) {
			if (y -> right == x) {
				a = z;
				b = y;
				c = x;
				t0 = a -> left;
				t1 = b -> left;
				t2 = c -> left;
				t3 = c -> right; 
			}
			else {
				a = z; 
				b = x;
				c = y; 
				t0 = a -> left; 
				t1 = b -> left;
				t2 = b -> right; 
				t3 = c -> right; 
			}

		}
		else {
			if (y -> left == x) {
				a = x;
				b = y;
				c = z;
				t0 = a -> left;
				t1 = a -> right;
				t2 = b -> right;
				t3 = c -> right; 
			}
			else {
				a = y;
				b = x;
				c = z;
				t0 = a -> left;
				t1 = b -> left;
				t2 = b -> right;
				t3 = c -> right; 
			}
		}
		AVLNode * p = z -> parent; 
		if (p == NULL)
			root = b;
		else {
			if (p -> left == z) 
				p -> left = b;
			else 
				p -> right = b; 
		}
		b -> parent = p;
		b -> left = a;
		b -> right = c;
		a -> parent = b;
		a -> left = t0; 
		a -> right = t1; 
		c -> parent = b;
		c -> left = t2;
		c -> right = t3; 
		if (t0 != NULL)
			t0 -> parent = a; 
		if (t1 != NULL)
			t1 -> parent = a; 
		if (t2 != NULL)
			t2 -> parent = c; 
		if (t3 != NULL)
			t3 -> parent = c; 
		maxheight = 0; 
		if (a -> left != NULL) 
			maxheight = a -> left -> height;
		if (a -> right != NULL && maxheight < a -> right -> height) 
			maxheight = a -> right -> height; 
		a -> height = maxheight + 1; 
		maxheight = 0; 
		if (c -> left != NULL) 
			maxheight = c -> left -> height;
		if (c -> right != NULL && maxheight < c -> right -> height)
			maxheight = c -> right -> height;
		c -> height = maxheight + 1;
		maxheight = 0;
		if (b -> left != NULL) 
			maxheight = b -> left -> height;
		if (b -> right != NULL && maxheight < b -> right -> height)
			maxheight = b -> right -> height; 
		b -> height = maxheight + 1;
		z = p; 
	}


}

// Find a key in the dictionary and return corresponding record or NULL
	DataType
AVLDictionary::findRecord( KeyType key)
{
	// Add your implementation here
AVLNode *e = root;

while (e != NULL) {
	if (strcmp(e -> key, key) == 0)
		return e -> data;
	else if (strcmp(e -> key, key) < 0)
		e = e -> right;
	else
		e = e -> left;
}
return NULL;
}

// Removes one element from the dictionary
	bool
AVLDictionary::removeElement(KeyType key)
{
	if (debug) {
		printf("------------------------------------\n");
		printf("removeElement(\"%s\")\n",  key);
		printf("---------- Before -----------------\n");
		printNode("", root, 0);
	}


	// Add your implementation here
	AVLNode * curr = root; 

	while (curr != NULL) {
		printf("loops "); 
		if (strcmp(curr -> key, key) == 0)
			break;
		if (strcmp(curr -> key, key) > 0)
			curr = curr -> left;
		else
			curr = curr -> right; 
	}
	if (curr == NULL)
		return false; 
	AVLNode * par = NULL; 
	if (curr != root)
		par = curr -> parent;
	else {
		curr -> left = curr -> right -> parent; 
	}
	if (curr -> left == NULL && curr -> right == NULL) {
		if (curr == par -> right)
			par -> right = NULL;
		else if (curr != par -> right)
			par -> left = NULL; 
		delete curr;
		restructure(par);
	}
	else if (curr -> left == NULL) {
		if (curr == par -> right) {
			par -> right = NULL; 
			par -> right = curr -> right; 
			par -> right -> height = curr -> right -> height;
			delete curr;
			curr = NULL;
			restructure(par); 
		}

		else {
			par -> left = NULL; 
			par -> left = curr -> right; 
			par -> left -> height = curr -> right -> height;
			restructure(par); 
			delete curr;
			curr = NULL; 

		}
	}
	else if (curr -> right == NULL) {
		if (curr == par -> right){
			par -> right = NULL;
			par -> right = curr -> left;
			par -> right -> height = curr -> left -> height;
			restructure(par);
			delete curr;
			curr = NULL; 

		}
		else {
			par -> left = NULL;
			par -> left = curr -> left;
			par -> left -> height = curr -> left -> height; 
			restructure(par);
			delete curr;
			curr = NULL; 

		}

	}else {
		AVLNode * diff = curr -> left; 
		while (diff -> right != NULL) {
			diff = diff -> right; 
		}
		curr -> data = diff -> data;
		curr -> key = diff -> key;
		curr = diff; 
		if (curr -> left == NULL) {
			if (curr -> parent -> right == curr) {
				curr -> parent -> right = curr -> right; 
			}
			else if (curr -> parent -> right != curr) 
				curr -> parent -> left = curr -> right; 
		}
		else if (curr -> left != NULL) {
			if (curr -> parent -> right == curr) 
				curr -> parent -> right = curr -> left;
			else 
				curr -> parent -> left = curr -> left; 
		}
		restructure(curr -> parent);

	}
	if (debug) {
		printf("---------- After -----------------\n");
		printNode("", root, 0);

		checkRecursive(root);
	}

	return true;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
	KeyType *
AVLDictionary::keys(int * n)
{
	KeyType * a = (KeyType *) malloc(nElements * sizeof(KeyType));
	*n = 0;
	addKeysRecursive(root, a, n);

	return a;
}

// Add keys recursively
void
AVLDictionary::addKeysRecursive(AVLNode * node, KeyType * a, int * n) {
	if (node==NULL) {
		return;
	}

	a[*n] = node->key;
	(*n)++;
	addKeysRecursive(node->left, a, n);
	addKeysRecursive(node->right, a, n);
}


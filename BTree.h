#pragma once
#include<iostream>
#include<queue>
#include<stack>
#include<fstream>
#include<vector>

using namespace std;

class BTree {
public:
	struct node {
		int* keys;				//niz kljuceva
		node** kids;			//niz sinova
		node* parent;			//otac
		int numKeys;			//broj popunjenih kljuceva
		int NumKids(int max);	//broj podstabala,
		node* CopyNode();
		bool isLeaf;			//da li je list
		int m;
	};
	BTree(int n);												//konstruktor za kreiranje praznog stabla
	BTree(int n, string input);									//konstruktor za kreiranje iz fajla
	int MAX() { return max; }									//getter za max
	node* Root() { return root; }								//getter za root
	bool InsertKey(int k);										//ubacivanje kljuca
	bool DeleteKey(int k);										//brisanje kljuca
	bool SearchKey(int k);										//pretraga kljuca
	void Split(node* child);									//podela cvora
	void Merge(node* c1, node* c2);								//spajanje cvorova
	void RemoveKey(node* c, int i);								//izbacivanje kljuca iz cvora po indeksu
	void RemoveKeyValue(node* c, int k);						//izbacivanje kljuca iz cvora po vrednosti
	void AddKey(node* c, int k);								//dodavanje kljuca u cvor sortirano
	bool DeleteFromLeaf(node* p, int k);						//brisanje kljuca iz lista
	void Balance(node* p);										//balansiranje broja kljuceva u cvoru
	void DeleteTree();											//brisanje stabla
	vector<int> LevelOrder();									//vraca niz svih kljuceva

	bool bp;													//oznaka da li je u rezimu za forsiranje maksimalne popunjenosti

	friend ostream& operator<<(ostream& os, BTree* const bt);	//ispis

	

private:
	int max;													//red stabla, max kljuceva je u stvari max-1
	int min;													//minimalan broj kljuceva
	node* root;													//koren stabla

	static node* GetNode(int m);
	static node* RightSibling(node* k);
	static node* LeftSibling(node* k);
	static int ParentIndex(node* parent, node* child);
	static void Sort(int* arr, int n);							
	static void SortKids(node** arr, int n);					
};



typedef BTree::node node;




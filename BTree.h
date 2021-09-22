#pragma once

/*
* BTree Implementation
* Author: Kosta Matijevic
* Date: Nov 2019
* Operations time complexity: O(log(n))
* Memory: O(n) 
*/

#include<iostream>
#include<queue>
#include<stack>
#include<fstream>
#include<vector>

using namespace std;

class BTree {
public:

    struct node {
        int* keys;
        node** kids;
        node* parent;
        int numKeys;
        int NumKids(int max);
        node* CopyNode();
        bool isLeaf;
        int m;
    };

    BTree(int n);
    BTree(int n, string input);
    int MAX() { return max; }
    node* Root() { return root; }
    bool InsertKey(int k);
    bool DeleteKey(int k);
    bool SearchKey(int k);
    void Split(node* child);
    void Merge(node* c1, node* c2);
    void RemoveKey(node* c, int i);
    void RemoveKeyValue(node* c, int k);
    void AddKey(node* c, int k);
    bool DeleteFromLeaf(node* p, int k);
    void Balance(node* p);
    void DeleteTree();
    vector<int> LevelOrder();

    bool bp;

    friend ostream& operator<<(ostream& os, BTree* const bt);//ispis



private:

    int max;
    int min;
    node* root;

    static node* GetNode(int m);
    static node* RightSibling(node* k);
    static node* LeftSibling(node* k);
    static int ParentIndex(node* parent, node* child);
    static void Sort(int* arr, int n);							
    static void SortKids(node** arr, int n);		

};

typedef BTree::node node;




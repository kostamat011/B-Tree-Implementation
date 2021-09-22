#include"BTree.h"

BTree* EmptyTree()
{
    int m;
    cout << "\nEnter order of the tree: ";
    cin >> m;
    BTree* bt = new BTree(m+1);
    cout << "Successfully created empty tree of order " << m << ".\n\n";
    return bt;
}

BTree* TreeFromFile()
{
    int m;
    string s;
    cout << "\nEnter order of the tree: ";
    cin >> m;
    cout << "\nEnter name of the file in local dir: ";
    cin >> s;
    BTree* bt = new BTree(m, s);
    cout << "Successfully created tree from file "+s+"\n\n";
    return bt;
}

void Insert(BTree*& bt)
{
    int k;
    cout << "\nEnter key to insert: ";
    cin >> k;
    if (bt->SearchKey(k))
        cout << "Insert failed! Key exists!\n\n";
    else if (bt->InsertKey(k))
        cout << "Successfully inserted key " << k << "!\n\n";
    else
        cout << "Insert failed!\n\n";
}

void Delete(BTree*& bt)
{
    int k;
    cout << "\nEnter key to delete: ";
    cin >> k;
    if (bt->DeleteKey(k))
        cout << "Successfully deleted key " << k << "!\n\n";
    else
        cout << "Delete failed! Key does not exist!\n\n";
}

void HeightAndOrder(BTree* bt)
{
    if (bt->Root() == nullptr)
    {
        cout << "Tree does not exist.\n\n";
        return;
    }
    node* n = bt->Root();
    queue<node*> Q;
    int cnt = 0, height = 0;
    Q.push(n);

    while (!Q.empty())
    {
        int size = Q.size();
        for (int k = 0; k < size; k++)
        {
            node* temp = Q.front(); Q.pop();
            for (int i = 0; i < temp->numKeys; i++)
                cnt++;
            for (int i = 0; i < temp->numKeys + 1; i++)
            {
                if (temp->kids[i] != nullptr)
                    Q.push(temp->kids[i]);
            }
        }
        height++;

    }
    cout << "\nHeight of tree is: " << height << "\nNumber of inserted nodes is: " << cnt << "\n\n";
}

void Search(BTree* bt)
{
    int k;
    cout << "\nEnter key to search: ";
    cin >> k;
    if (bt->SearchKey(k))
        cout << "Successfully searched key " << k << "!\n\n";
    else
        cout << "Search failed!\n\n";
}

BTree* Rearrange(BTree* bt)
{
    vector<int> keys = bt->LevelOrder();
    for (int i = 0; i < keys.size() - 1; i++)
    {
        for (int j = i + 1; j < keys.size(); j++)
        {
            if (keys[i] > keys[j])
            {
                int tmp = keys[i];
                keys[i] = keys[j];
                keys[j] = tmp;
            }
        }
    }
    BTree* newBt = new BTree(bt->MAX());
    newBt->bp = true;

    for (int i = 0; i < bt->MAX(); i++)
    {
        //newBt->InsertKey(keys[i]);
        int j = i;
        while (j < keys.size())
        {
            newBt->InsertKey(keys[j]);
            cout << newBt;
            j += bt->MAX();
        }
    }

    newBt->bp = false;
    cout << "Tree successfully rearranged.\n\n";
    return newBt;

}

BTree* ChangeOrder(BTree* bt)
{
    vector<int> keys = bt->LevelOrder();

    int m;
    cout << "Enter new order of the tree: ";
    cin >> m;

    BTree* newBt = new BTree(m+1);
    for (int i = 0; i < keys.size(); i++)
        newBt->InsertKey(keys[i]);

    cout << "Tree successfully converted to order " << m << ".\n\n";

    return newBt;

}

int main()
{
    int ctrl;
    BTree* bt = nullptr;
    while (1)
    {
        cout << "1.Create empty tree.\n";
        cout << "2.Get tree height and number of keys.\n";
        cout << "3.Create tree from text file.\n";
        cout << "4.Search key.\n";
        cout << "5.Insert key.\n";
        cout << "6.Delete key.\n";
        cout << "7.Rearrange keys for maximum fullness.\n";
        cout << "8.Print\n";
        cout << "9.Delete tree\n";
        cout << "10.Change the order of tree\n";
        cout << "0.Exit\n";

        cin >> ctrl;
        switch (ctrl)
        {
        case 1:
            bt = EmptyTree();
            break;
        case 2:
            HeightAndOrder(bt);
            break;
        case 3:
            bt = TreeFromFile();
            break;
        case 4:
            Search(bt);
        case 5:
            Insert(bt);
            break;
        case 6:
            Delete(bt);
            break;
        case 7:
            bt = Rearrange(bt);
            break;
        case 8:
            cout << bt;
            break;
        case 9:
            bt->DeleteTree();			//this function of btree has no wrapper in test program
            break;
        case 10:
            bt = ChangeOrder(bt);
            break;
        case 0:
            exit(1);
        }

    }
}
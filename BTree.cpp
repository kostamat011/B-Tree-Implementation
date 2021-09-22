#include "BTree.h"

int node::NumKids(int max)
{
	int cnt = 0;
	while (kids[cnt] != nullptr && cnt < max + 1)
		cnt++;
	return cnt;
}

node* node::CopyNode()
{
	if (this == nullptr)
		return nullptr;

	node* nc = GetNode(this->m);
	nc->numKeys = this->numKeys;
	nc->isLeaf = this->isLeaf;
	for (int i = 0; i <= this->numKeys; i++)
	{
		if (i != this->numKeys)
			nc->keys[i] = this->keys[i];

		nc->kids[i] = this->kids[i]->CopyNode();
	}
	return nc;
}

node* BTree::GetNode(int m) {
	node* n = new node;
	n->keys = new int[m];
	for (int i = 0; i < m; i++) n->keys[i] = -1;
	n->kids = new node * [m + 1];
	for (int i = 0; i < m; i++) n->kids[i] = nullptr;
	n->parent = nullptr;
	n->numKeys = 0;
	n->isLeaf = true;
	n->m = m;
	return n;
}

ostream& operator<<(ostream& os, BTree* const bt)
{
	if (bt->root == nullptr)
	{
		os << "\nPrazno stablo.\n\n";
		return os;
	}
	node* n = bt->root;
	queue<node*> Q;

	Q.push(n);

	os << "\n\n";
	while (!Q.empty())
	{
		int size = Q.size();
		for (int k = 0; k < size; k++)
		{
			node* temp = Q.front(); Q.pop();
			os << "|";
			for (int i = 0; i < temp->numKeys; i++)
				os << temp->keys[i] << "|";
			for (int i = 0; i < temp->numKeys + 1; i++)
			{
				if (temp->kids[i] != nullptr)
					Q.push(temp->kids[i]);
			}
			if (temp == bt->root)
				os << "--(root)";
			else
				os << "--(" << temp->parent->keys[0] << ")     ";
		}
		os << "\n";
	}
	os << "\n\n";

	return os;
}

node* BTree::RightSibling(node* k)
{
	node* p = k->parent;
	for (int i = 0; i < p->numKeys + 1; i++)
	{
		if (p->kids[i] == k)
		{
			if (i == p->numKeys)
				return nullptr; //najdesniji je znaci nema desnog brata
			else
				return p->kids[(i + 1)];
		}
	}
}

node* BTree::LeftSibling(node* k)
{
	node* p = k->parent;

	for (int i = 0; i < p->numKeys + 1; i++)
	{
		if (p->kids[i] == k)
		{
			if (i == 0)
				return nullptr; //najlevlji je znaci nema levog brata
			else
				return p->kids[(i - 1)];
		}
	}
}


int BTree::ParentIndex(node* parent, node* child)
{
	int i = 0;
	while (parent->kids[i] != child)
		i++;
	if (i == parent->numKeys)
		i--;
	return i;
}

void BTree::Sort(int* arr, int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (((arr[i] > arr[j]) && arr[j] != 999) || arr[i] == 999)
			{
				int tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

void BTree::SortKids(node** arr, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n + 1; j++)
		{
			if (arr[j] != nullptr && (arr[i] == nullptr || (arr[i]->keys[0] > arr[j]->keys[0])))
			{
				node* tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

vector<int> BTree::LevelOrder()
{
	node* n = root;
	vector<int> keys;
	queue<node*> Q;

	Q.push(n);

	while (!Q.empty())
	{

		node* temp = Q.front(); Q.pop();
		for (int i = 0; i < temp->numKeys; i++)
			keys.push_back(temp->keys[i]);
		for (int i = 0; i < temp->numKeys + 1; i++)
		{
			if (temp->kids[i] != nullptr)
				Q.push(temp->kids[i]);
		}
	}
	return keys;
}

BTree::BTree(int n)
{
	max = n;
	min = ((max % 2 == 0) ? max / 2 : max / 2 + 1) - 1;
	root = GetNode(n);
	bp = false;
}

BTree::BTree(int n, string input) : BTree(n) //konstruktor za kreiranje iz fajla
{
	ifstream dat;
	dat.open(input);
	if (!dat.is_open())
	{
		cout << "Greska pri otvaranju datoteke.\n";
		return;
	}
	else
	{
		while (!dat.eof())
		{
			int k;
			dat >> k;
			this->InsertKey(k);
		}
	}
	bp = false;
}

void BTree::RemoveKeyValue(node* c, int k)
{
	int i = 0;
	while (c->keys[i] != k)
	{
		i++;
	}
	if (i == c->numKeys) return;

	c->keys[i] = 999;
	Sort(c->keys, c->numKeys);
	c->numKeys--;
}

void BTree::RemoveKey(node* c, int i)
{
	c->keys[i] = 999;
	Sort(c->keys, c->numKeys);
	c->numKeys--;
}

void BTree::AddKey(node* c, int k)
{
	c->keys[c->numKeys++] = k;
	Sort(c->keys, c->numKeys);
}

void BTree::Merge(node* c1, node* c2)
{
	node* parent = c1->parent;
	for (int i = 0; i < c2->numKeys; i++) //uzme sve kljuceve i decu od brata
	{
		c1->keys[c1->numKeys++] = c2->keys[i];
		if (!c1->isLeaf)
		{
			c1->kids[c1->numKeys] = c2->kids[i]->CopyNode();
			c1->kids[c1->numKeys]->parent = c1;
		}
	}
	if (!c1->isLeaf)
	{
		c1->kids[c1->numKeys + 1] = c2->kids[c2->numKeys]->CopyNode();
		c1->kids[c1->numKeys + 1]->parent = c1;
		SortKids(c1->kids, c1->numKeys + 1);
	}//da ne propusti poslednje dete



	int parentIndex = ParentIndex(parent, c1);
	c1->keys[c1->numKeys++] = parent->keys[parentIndex]; //uzme roditeljski kljuc od oca
	Sort(c1->keys, c1->numKeys);
	RemoveKey(parent, parentIndex); //otac gubi roditeljski kljuc


	if (parent == root && parent->numKeys == 0) //novi koren
	{
		delete parent->kids;
		delete parent->keys;
		delete parent;
		c1->parent = nullptr;
		root = c1;
		if (root->NumKids(max) != 0)
			SortKids(root->kids, root->numKeys);
	}
	else
	{
		for (int i = 0; i < parent->numKeys + 2; i++)
		{
			if (parent->kids[i] == c2)
				parent->kids[i] = nullptr;
		}
		SortKids(parent->kids, parent->numKeys + 1); //+1 jer parent u tom trenutku ima npr 2 kljuca a 4 podstabla
	}


	delete c2->kids;
	delete c2->keys;
	delete c2;
}

void BTree::Split(node* child)
{
	bool fullParent = false;
	node* parent = child->parent;
	if (parent != nullptr && parent->numKeys == max - 1) fullParent = true;

	node* tmp = GetNode(max);
	tmp->isLeaf = child->isLeaf;

	if (parent == nullptr) //splituje se koren
	{
		parent = GetNode(max);
		parent->kids[0] = child;
		parent->isLeaf = false;
		root = parent;
		child->parent = parent;
	}

	//dohvata se srednji kljuc i salje u oca
	int mid = child->numKeys / 2;
	parent->keys[parent->numKeys++] = child->keys[mid];

	//izbacuje se srednji kljuc iz deteta
	child->keys[mid] = 999;
	Sort(child->keys, child->numKeys);
	child->numKeys--;

	//polovina kljuceva ide u novi cvor
	for (int i = mid; i < child->numKeys; i++)
	{
		tmp->keys[tmp->numKeys++] = child->keys[i];
	}
	//ako dete nije bilo list odgovarajuca njegova deca isto idu u novi
	if (!child->isLeaf)
	{
		// ako je imao jedno vise od dozvoljenog broja podstabla onda se jedno vise tera u temp
		if (child->NumKids(max) == max + 1)
		{
			for (int i = mid + 1; i <= child->numKeys + 1; i++)
			{
				tmp->kids[i - mid - 1] = child->kids[i];
				tmp->kids[i - mid - 1]->parent = tmp;
			}
		}
		else
		{
			for (int i = mid; i <= child->numKeys; i++)
			{
				tmp->kids[i - mid] = child->kids[i];
				tmp->kids[i - mid]->parent = tmp;

			}
		}
	}

	//child je izgubio onoliko kljuceva koliko tmp ima
	child->numKeys -= tmp->numKeys;
	child->keys[child->numKeys] = 999;

	//tmp postaje novi sin roditelja
	tmp->parent = parent;
	if (fullParent) //otac je imao maksimalan broj cvorova, broj podstabala ce prevazici maks dok se ne splituje
		parent->kids[max] = tmp;
	else
		parent->kids[parent->numKeys] = tmp;

	Sort(parent->keys, parent->numKeys);
	SortKids(parent->kids, parent->numKeys);
}

bool BTree::InsertKey(int k)
{
	if (SearchKey(k))
		return false;

	node* p = root;
	while (!p->isLeaf)
	{
		int i = 0;
		while (p->keys[i] < k && i < p->numKeys)
			i++;

		if (p->keys[i] == k)
			return false; //kljuc vec postoji

		p = p->kids[i];
	}

	//nadjen list u koji ubacujemo
	//ubacimo, sortiramo pa gledamo da li je owerflow
	p->keys[p->numKeys++] = k;
	Sort(p->keys, p->numKeys);
	if (p->numKeys == max)
	{
		bool bdo = true;
		if (bp && p->parent != nullptr)//ako je u rezimu za max popunjenost pokusaj prelivanje
		{
			if (p->parent != nullptr)
			{
				node* s = nullptr;
				node* r = RightSibling(p);
				node* l = LeftSibling(p);

				if (l != nullptr && ((l->numKeys) < (max - 1)))
					s = l;

				if (r != nullptr && ((r->numKeys) < (max - 1)))
					s = r;

				if (s == nullptr) bdo = false;

				if (bdo)
				{
					node* parent = p->parent;
					int pIndex = (s == RightSibling(p)) ? ParentIndex(parent, p) : ParentIndex(parent, p) - 1;
					if (pIndex == -1) pIndex = 0;
					int eIndex = (s == RightSibling(p)) ? p->numKeys - 1 : 0;

					AddKey(s, parent->keys[pIndex]);
					parent->keys[pIndex] = p->keys[eIndex];
					RemoveKey(p, eIndex);
					return true;
				}
			}


		}
		//ako je owerflow splitujemo roditelja sve dok 
		//ne dodjemo do nekog koji nije pun ili iznikne novi koren
		node* q = p;
		while (q->numKeys >= max)
		{
			Split(q);
			q = q->parent;
		}
	}

	return true;
}

void BTree::Balance(node* p)
{
	node* l = LeftSibling(p); //levi brat
	node* r = RightSibling(p); //desni brat
	node* s = nullptr; //l ili r

	if (r == nullptr)
		s = l;
	else if (l == nullptr)
		s = r;
	else
	{
		if (r->numKeys == min && l->numKeys != min)
			s = l;
		else
			s = r;
	}


	int extremeIndex = 0;  //za levog brata najdesniji kljuc za desnog najlevlji
	if (s == LeftSibling(p))
		extremeIndex = s->numKeys - 1;

	node* parent = p->parent;
	int parentIndex = ParentIndex(parent, p); //indeks podstablja roditelja u kom je p

	if (s->numKeys > min) //1) brat ima da mu da kljuc
	{
		p->keys[p->numKeys++] = parent->keys[parentIndex];
		parent->keys[parentIndex] = s->keys[extremeIndex];
		RemoveKey(s, extremeIndex);

		//ako nisu listovi brat mora da mu da i svoje dete

		if ((!s->isLeaf) && s->NumKids(max) - s->numKeys > 1)
		{
			node* nc = GetNode(max);

			if (s == l) //ako je levi brat daje mu najdesnije dete
			{

				nc = s->kids[s->numKeys + 1]->CopyNode();
				s->kids[s->numKeys + 1] = nullptr;
			}
			else //ako je desni brat daje najlevlje dete
			{
				nc = s->kids[0]->CopyNode();
				s->kids[0] = nullptr;
			}
			p->kids[p->NumKids(max)] = nc; //pok za dete
			nc->parent = p; //pok za oca


			SortKids(p->kids, p->numKeys); //sortiranje dece
			SortKids(s->kids, s->numKeys + 1);
		}
	}
	else if (s->numKeys == min) //2)brat nema da mu pozajmi
	{
		Merge(p, s); //spajanje kljuca p i njegovog brata s
	}

}

bool BTree::DeleteFromLeaf(node* p, int k)
{
	int i = 0;
	while (p->keys[i] != k) i++; //pronalazenje indeksa za brisanje

	if (p->numKeys > min)  //1)  kada se brise iz lista koji ima dovoljno kljuceva
	{
		RemoveKey(p, i); //brisanje kljuca na indeksu i iz cvora p
		return true;
	}

	else if (p->numKeys == min)  //2)  kada se brise iz lista koji nema dovoljno kljuceva
	{
		Balance(p); //regulisanje broja kljuceva u cvoru
		RemoveKeyValue(p, k); //brisanje kljuca k iz cvora p

		//ako je posle regulisanja roditelj ostao bez dovoljno kljuceva mora se regulisati jos
		node* q = p->parent;

		while (q != nullptr && q != root && q->numKeys < min)
		{
			Balance(q);
			q = q->parent;
		}
	}
	return true;
}

bool BTree::DeleteKey(int k)
{
	node* p = root;
	while (p != nullptr)
	{
		int i = 0;
		while (p->keys[i] < k && i < p->numKeys)
			i++;
		if (p->keys[i] == k) //nadjen je kljuc
		{
			if (p->isLeaf) //jeste list
			{
				if (p == root && p->numKeys == 1) //brise se koren ostaje prazno stablo
				{
					delete root->kids;
					delete root->keys;
					root = nullptr;
					return true;
				}
				if (DeleteFromLeaf(p, k)) //brisanje kljuca k iz lista p
					return true;
			}
			else //nije list
			{

				node* r = p->kids[i + 1]; //sledbenicko podstablo
				if (r->isLeaf) //ako je sledbenicko podstablo list zamenjuje se sa najlevljim kljucem
				{
					p->keys[i] = r->keys[0];
					r->keys[0] = k;
					DeleteFromLeaf(r, k);
					return true;
				}

				//ako sledbenicko podstablo nije list sidje se u najlevlje podstablo 
				//i zameni sa najdesnijim kljucem
				while (!r->isLeaf)
					r = r->kids[0];
				p->keys[i] = r->keys[r->numKeys - 1];
				r->keys[r->numKeys - 1] = k;
				DeleteFromLeaf(r, k);
				return true;
			}
		}

		p = p->kids[i]; //ako nije nadjen silazak u odgovarajuce podstablo
	}
	return false;
}


bool BTree::SearchKey(int k)
{
	node* p = root;
	while (p != nullptr)
	{
		int i = 0;

		while (p->keys[i] < k && i < p->numKeys)
			i++;

		if (p->keys[i] == k) return true;

		p = p->kids[i];
	}
	return false;
}

void BTree::DeleteTree()
{
	queue<node*> Q;
	stack<node*> S;
	node* r = root;
	Q.push(r);
	while (!Q.empty())
	{
		node* tmp = Q.front(); Q.pop();
		for (int i = 0; i <= tmp->numKeys; i++)
		{
			if (tmp->kids[i] != nullptr)
			{
				Q.push(tmp->kids[i]);
				S.push(tmp->kids[i]);
			}
		}

	}

	while (!S.empty())
	{
		node* tmp = S.top(); S.pop();
		delete tmp->kids;
		delete tmp->keys;
		delete tmp;
		tmp = nullptr;
	}
	root = nullptr;

	cout << "Uspesno obrisano stablo.\n\n";
}




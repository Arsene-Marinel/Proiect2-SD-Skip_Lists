#include <bits/stdc++.h>
#include <fstream>
using namespace std;
ifstream f("abce.in");
ofstream g("abce.out");


class Node
{
public:
    int key;
    Node **forward;           /// array pentru a memora pointerii nivelurilor de noduri
    Node(int, int);
};

Node::Node(int key, int level)
{
    this->key = key;
    forward = new Node*[level + 1];
    memset(forward, 0, sizeof(Node*)*(level + 1));           /// completam array-ul înainte cu 0 (NULL)
}


class SkipList
{
    int Max_Lvl;
    float p;                           /// fractia de noduri pe nivel
    int level;                          /// nivelul curent al skip list
    Node *header;
public:
    SkipList(int, float);
    int randomLevel();
    Node* createNode(int, int);

    void insertElement(int);
    void deleteElement(int);
    bool findElement(int);

    void afisList_of_Lvl();
    void afis_Numere(int, int);

    Node *predecesor(int);
    Node *succesor(int);
};

SkipList::SkipList(int Max_Lvl, float p)
{
    this->Max_Lvl = Max_Lvl;
    this->p = p;
    level = 0;

    header = new Node(-1, Max_Lvl);    /// cream nodul header si initializam cheia cu -1
}

///Cream un nivel random pentru nod
int SkipList::randomLevel()
{
    float r = (float)rand()/RAND_MAX;
    int lvl = 0;
    while (r < p && lvl < Max_Lvl)
    {
        lvl++;
        r = (float)rand()/RAND_MAX;
    }
    return lvl;
}

/// Cream un nod nou
Node* SkipList::createNode(int key, int level)
{
    Node *n = new Node(key, level);
    return n;
}


void SkipList::insertElement(int key)
{
    Node *current = header;

    Node *update[Max_Lvl + 1];
    memset(update, 0, sizeof(Node*)*(Max_Lvl + 1));       /// initializez update cu 0

    for (int i = level; i >= 0; i--)
    {
        while (current->forward[i] != NULL && current->forward[i]->key < key)       /// ne uitam de fiecare data la forward-ul elementului pe care suntem
            current = current->forward[i];                                          /// daca acesta este mai mare decat elementul de inserat inseamna ca nu
        update[i] = current;                                                        /// putem merge mai departe pe nivelul respectiv si coboram un nivel
    }

    current = current->forward[0];
    if (current == NULL || current->key != key)
    {
        ///Generam un nivel random pentru nod
        int new_level = randomLevel();

        if (new_level > level)       /// dacă nivelul random este mai mare decât nivelul curent al listei,
            /// inițializam update cu header pentru o utilizare ulterioară
        {
            for (int i = level + 1; i < new_level + 1; i++)
                update[i] = header;
            level = new_level;
        }
        Node* n = createNode(key, new_level);

        for (int i = 0; i <= new_level; i++)              ///Inseram nodul pe fiecare nivel pana la nivelul random ales
        {
            n->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = n;
        }
        ///cout << "Successfully Inserted key " << key << '\n';
    }
}

void SkipList::deleteElement(int key)
{
    Node *current = header;

    Node *update[Max_Lvl + 1];
    memset(update, 0, sizeof(Node*)*(Max_Lvl + 1));

    for(int i = level; i >= 0; i--)
    {
        while(current->forward[i] != NULL  && current->forward[i]->key < key)
            current = current->forward[i];
        update[i] = current;
    }
    current = current->forward[0];

    if(current != NULL and current->key == key)
        for(int i = 0; i <= level; i++)
        {
            if(update[i]->forward[i] != current)
                break;
            update[i]->forward[i] = current->forward[i];
        }

    while(level > 0 && header->forward[level] == 0)
        level--;
    ///cout << "Successfully deleted key " << key << '\n';
}

bool SkipList::findElement(int key)
{
    bool ok = 0;
    Node *current = header;

    for(int i = level; i >= 0; i--)
        while(current->forward[i] && current->forward[i]->key < key)
            current = current->forward[i];

    current = current->forward[0];

    if(current!= NULL and current->key == key)
    {
        ok = 1;
        ///cout << "Found key: " << key << '\n';
    }
    return ok;
}

void SkipList::afisList_of_Lvl()
{
    cout<<"Skip List: \n";
    for (int i = 0; i <= level; i++)
    {
        Node *node = header->forward[i];
        cout << "Level " << i << ": ";
        while (node != NULL)
        {
            cout << node->key << ' ';
            node = node->forward[i];
        }
        cout << '\n';
    }
}

void SkipList::afis_Numere(int x, int y)
{
    Node *current = header;
    for(int i = level; i >= 0; i--)
        while(current->forward[i] && current->forward[i]->key < x)
            current = current->forward[i];

    current = current->forward[0];
    g << current->key << ' ';

    while(current->forward[0] != 0 && current->forward[0]->key <= y)
    {
        current = current->forward[0];
        g << current->key << ' ';
    }
    g << '\n';
}

Node* SkipList::predecesor(int x)
{
    Node *current = header;
    for(int i = level; i >= 0; i--)
        while (current->forward[i] != 0 && current->forward[i]->key <= x)
            current = current->forward[i];
    return current;
}

Node* SkipList::succesor(int x)
{
    Node *current = header;
    for(int i = level; i >= 0; i--)
        while (current->forward[i] != 0 && current->forward[i]->key < x)
            current = current->forward[i];
    return current->forward[0];
}

int main()
{
    srand((unsigned)time(0));           /// generator de numere random

    int n, x, y, op, k = 0;
    f >> n;
    int m = n;
    while(m > 1)          ///nivelul maxim este log n
    {
        m /= 2;
        k++;
    }
    SkipList L(k, 0.5);

    for(int i = 0; i < n; i++)
    {
        f >> op;
        if(op == 1)
        {
            f >> x;
            L.insertElement(x);
        }
        if(op == 2)
        {
            f >> x;
            L.deleteElement(x);
        }
        if(op == 3)
        {
            f >> x;
            g << L.findElement(x) << '\n';
        }
        if(op == 4)
        {
            f >> x;
            g << L.predecesor(x)->key << '\n';
        }
        if(op == 5)
        {
            f >> x;
            g << L.succesor(x)->key << '\n';
        }
        if(op == 6)
        {
            f >> x >> y;
            L.afis_Numere(x, y);
        }
    }

    ///L.afisList_of_Lvl();
    return 0;
}


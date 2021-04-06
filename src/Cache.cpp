#include "main.h"

int max(int a, int b)
{
    return a > b ? a : b;
}

Elem *Cache::removeFIFO()
{
    Elem *removed = this->dataQueue.front();
    this->dataQueue.pop();
    this->dataTree.remove(removed->addr);
    delete removed;
    return removed;
}

Data *Cache::read(int addr)
{
    Elem *elem = this->dataTree.query(addr);
    if (elem != NULL)
        return elem->data;
    return NULL;
}

Elem *Cache::put(int addr, Data *cont)
{
    Elem *elem = new Elem(addr, cont, true);
    Elem *removed = NULL;
    if ((int)dataQueue.size() == maxSize)
        removed = this->removeFIFO();
    this->dataQueue.push(elem);
    this->dataTree.insert(elem);
    return removed;
}
Elem *Cache::write(int addr, Data *cont)
{
    Elem *elem = this->dataTree.query(addr);
    Elem *removed = NULL;
    if (elem)
    {
        elem->data = cont;
        elem->sync = false;
        return removed;
    }
    else
    {
        if ((int)dataQueue.size() == maxSize)
            removed = this->removeFIFO();
        elem = new Elem(addr, cont, false);
        this->dataTree.insert(elem);
        this->dataQueue.push(elem);
        return removed;
    }
}
void Cache::print()
{
    queue<Elem *> tmp = dataQueue;
    while (!tmp.empty())
    {
        tmp.front()->print();
        tmp.pop();
    }
}
void Cache::preOrder()
{
    this->dataTree.preOrder();
}
void Cache::inOrder()
{
    this->dataTree.inOrder();
}

Elem *CacheTree::query(int index)
{
    Node *current = this->root;
    while (current != NULL)
    {
        int addr = current->getIndex();
        if (addr == index)
            return current->data;
        else if (addr < index)
            current = current->right;
        else
            current = current->left;
    }
    return NULL;
}

CacheTree::Node *CacheTree::rrRotate(Node *node)
{
    Node *temp;
    temp = node->right;
    node->right = temp->left;
    temp->left = node;

    return temp;
}
CacheTree::Node *CacheTree::llRotate(Node *node)
{
    Node *temp;
    temp = node->left;
    node->left = temp->right;
    temp->right = node;

    return temp;
}
CacheTree::Node *CacheTree::lrRotate(Node *node)
{
    Node *temp;
    temp = node->left;
    node->left = rrRotate(temp);

    return llRotate(node);
}
CacheTree::Node *CacheTree::rlRotate(Node *node)
{
    Node *temp;
    temp = node->right;
    node->right = llRotate(temp);

    return rrRotate(node);
}

CacheTree::Node* CacheTree::balance(Node *node)
{
    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1)
    {
        if (getBalanceFactor(node->left) > 0)
            node = llRotate(node);
        else
            node = lrRotate(node);
    }
    else if (balanceFactor < -1)
    {
        if (getBalanceFactor(node->right) > 0)
            node = rlRotate(node);
        else
            node = rrRotate(node);
    }

    return node;
}

CacheTree::Node *CacheTree::minValueNode(CacheTree::Node *node)
{
    Node* current = node; 
    while (current->left != NULL) 
        current = current->left; 
    return current;
}


CacheTree::Node *CacheTree::recursiveInsert(Node *node, Elem *elem)
{
    if (node == NULL)
    {
        return new Node(elem);
    }
    int index = node->getIndex();
    if (elem->addr < index)
    {
        node->left = recursiveInsert(node->left, elem);
        node = balance(node);
    }
    else if (index < elem->addr)
    {
        node->right = recursiveInsert(node->right, elem);
        node = balance(node);
    }
    return node;
}

CacheTree::Node *CacheTree::recursiveRemove(Node *node, int index)
{
    if (node == NULL)
        return node;
    if (index < node->getIndex())
        node->left = recursiveRemove(node->left, index);
    else if (index > node->getIndex())
        node->right = recursiveRemove(node->right, index);
    else
    {
        if ((node->left == NULL) || (node->right == NULL))
        {
            Node *temp = node->left ? node->left : node->right;
            if (temp == NULL)
            {
                temp = node;
                node = NULL;
            }
            else               // One child case
                *node = *temp; // Copy the contents of
                               // the non-empty child
            delete temp;
        }
        else
        {
            Node *temp = minValueNode(node->right);
            node->data = temp->data;
            node->right = recursiveRemove(node->right,
                                     temp->getIndex());
        }
    }
    if (node == NULL)
        return node;

    return balance(node);
}
void CacheTree::recursivePreOrder(Node *node)
{
    if (node == NULL)
        return;
    node->data->print();
    recursivePreOrder(node->left);
    recursivePreOrder(node->right);
}
void CacheTree::recursiveInOrder(Node *node)
{
    if (node == NULL)
        return;
    recursiveInOrder(node->left);
    node->data->print();
    recursiveInOrder(node->right);
}

void CacheTree::insert(Elem *data)
{
    this->root = recursiveInsert(this->root, data);
}

void CacheTree::remove(int index)
{
    this->root = recursiveRemove(this->root, index);
}

void CacheTree::preOrder()
{
    recursivePreOrder(this->root);
}

void CacheTree::inOrder()
{
    recursiveInOrder(this->root);
}

void CacheTree::dispose(Node* node)
{
    // to do:
}
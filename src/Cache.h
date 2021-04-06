#ifndef CACHE_H
#define CACHE_H

#include "main.h"

class CacheTree
{
private:
	class Node
	{
	public:
		Node *left;
		Node *right;
		Elem *data;
		Node(Elem *data)
		{
			this->left = NULL;
			this->right = NULL;
			this->data = data;
		}
		int getIndex() { return this->data->addr; }
	};

	Node *root;

	Node *rrRotate(Node *node);
	Node *llRotate(Node *node);
	Node *lrRotate(Node *node);
	Node *rlRotate(Node *node);

	Node* balance(Node *node);
	Node *minValueNode(Node *node);

	Node *recursiveInsert(Node *node, Elem *elem);
	Node *recursiveRemove(Node *node, int index);
	void recursivePreOrder(Node *node);
	void recursiveInOrder(Node *node);

	int getBalanceFactor(Node *node)
	{
		return getNodeHeight(node->left) - getNodeHeight(node->right);
	}
	int getNodeHeight(Node *node)
	{
		if (node != NULL)
		{
			int lHeight = getNodeHeight(node->left);
			int rHeight = getNodeHeight(node->right);
			return 1 + max(lHeight, rHeight);
		}
		return 0;
	}

public:
	CacheTree()
	{
		this->root = NULL;
	}

	~CacheTree()
	{
		dispose(this->root);
	}

	Elem *query(int index);
	void insert(Elem *data);
	void remove(int index);
	void preOrder();
	void inOrder();
	void dispose(Node* node);
};

class Cache
{
private:
	int maxSize;
	queue<Elem *> dataQueue;
	CacheTree dataTree;
	int p;
	// Remove an element from cache following FIFO policy
	// Return address to that element (deleted from heap)
	Elem *removeFIFO();

public:
	Cache(int s)
	{
		this->maxSize = s;
	}
	Data *read(int addr);
	Elem *put(int addr, Data *cont);
	Elem *write(int addr, Data *cont);
	void print();
	void preOrder();
	void inOrder();
};

#endif
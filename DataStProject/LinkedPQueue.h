#pragma once
#include "PNode.h"
#include "PQueueADT.h"
#include <vector>
using namespace std;


template <typename T>
class LinkedPQueue :public PQueueADT<T>
{
private:

	PNode<T>* backPtr;
	PNode<T>* frontPtr;
public:
	LinkedPQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry, int P);
	bool dequeue(T& frntEntry);
	bool peek(T& frntEntry)  const;
	int getPCount();
	void PrintQueue();
	~LinkedPQueue();

	//copy constructor
	LinkedPQueue(const LinkedPQueue<T>& LQ);
};
/////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
LinkedPQueue<T>::LinkedPQueue()
{
	backPtr = nullptr;
	frontPtr = nullptr;

}
/////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool LinkedPQueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
bool LinkedPQueue<T>::enqueue(const T& newEntry, int P) //Enqueues according to Priority not FIFO.
{
	PNode<T>* newNodePtr = new PNode<T>(newEntry, P);
	PNode<T>* tempPtr = frontPtr;
	// Insert the new node
	if (isEmpty())	//special case if this is the first node to insert
		frontPtr = newNodePtr; // The queue is empty
	else if (P > frontPtr->getPriority())
	{
		newNodePtr->setNext(frontPtr);
		frontPtr = newNodePtr;
	}
	else
	{
		while (tempPtr->getNext() && P <= tempPtr->getNext()->getPriority())
		{
			tempPtr = tempPtr->getNext();

		}
		if (tempPtr == backPtr)
		{
			newNodePtr->setNext(nullptr);
			tempPtr->setNext(newNodePtr);
			backPtr = backPtr->getNext();
		}
		newNodePtr->setNext(tempPtr->getNext());
		tempPtr->setNext(newNodePtr);
	}


	return true;
} // end enqueue


  /////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
bool LinkedPQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	PNode<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
		backPtr = nullptr;

	// Free memory reserved for the dequeued node
	delete nodeToDeletePtr;

	return true;

}
/////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool LinkedPQueue<T>::peek(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
///////////////////////////////////////////////////////////////////////////////////

template <typename T>
LinkedPQueue<T>::~LinkedPQueue()
{
	T temp;

	//Free (Dequeue) all nodes in the queue
	while (dequeue(temp));
}
/////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
LinkedPQueue<T>::LinkedPQueue(const LinkedPQueue<T>& LQ)
{
	PNode<T>* NodePtr = LQ.frontPtr;
	if (!NodePtr) //LQ is empty
	{
		frontPtr = backPtr = nullptr;
		return;
	}

	//insert the first node
	PNode<T>* ptr = new PNode<T>(NodePtr->getItem(), NodePtr->getPriority());
	frontPtr = backPtr = ptr;
	NodePtr = NodePtr->getNext();

	//insert remaining nodes
	while (NodePtr)
	{
		PNode<T>* ptr = new PNode<T>(NodePtr->getItem(), NodePtr->getPriority());
		backPtr->setNext(ptr);
		backPtr = ptr;
		NodePtr = NodePtr->getNext();
	}
}
template <typename T>
int LinkedPQueue<T>::getPCount()
{
	int count = 0;
	PNode<T>* ptr = frontPtr;
	while (ptr)
	{
		count++;
		ptr = ptr->getNext();
	}
	return count;
}
template <typename T>
void LinkedPQueue<T>::PrintQueue()
{
	PNode<T>* ptr = frontPtr;
	while (ptr)
	{
		cout << ptr->getItem();
		if (ptr->getNext())
		{
			cout << ",";
		}
		ptr = ptr->getNext();
	}
}

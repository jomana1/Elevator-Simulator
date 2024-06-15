#pragma once
template < typename T>
class PNode
{
private:
	T item; // A data item
	PNode<T>* next; // Pointer to next node
	int Priority;
public:
	PNode();
	PNode(const T& r_Item, int P);
	PNode(const T& r_Item, PNode<T>* nextNodePtr);
	void setItem(const T& r_Item);
	void setNext(PNode<T>* nextNodePtr);
	T getItem() const;
	PNode<T>* getNext() const;
	int getPriority();
}; // end Node

template < typename T>
PNode<T>::PNode()
{
	next = nullptr;
	Priority = 0;
}

template < typename T>
PNode<T>::PNode(const T& r_Item, int P)
{
	item = r_Item;
	next = nullptr;
	Priority = P;
}

template < typename T>
PNode<T>::PNode(const T& r_Item, PNode<T>* nextNodePtr)
{
	item = r_Item;
	next = nextNodePtr;
	Priority = nextNodePtr->getPriority();
}
template < typename T>
void PNode<T>::setItem(const T& r_Item)
{
	item = r_Item;
}

template < typename T>
void PNode<T>::setNext(PNode<T>* nextNodePtr)
{
	next = nextNodePtr;
}

template < typename T>
T PNode<T>::getItem() const
{
	return item;
}

template < typename T>
PNode<T>* PNode<T>::getNext() const
{
	return next;
}
template < typename T>

int PNode<T>::getPriority()
{
	return Priority;
}
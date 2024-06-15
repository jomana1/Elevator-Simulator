#pragma once
#include "Node.h"
#include <iostream>
using namespace std;
template <typename T>
class LinkedList
{
protected:
	Node<T>* Head;	//Pointer to the head of the list
	//You can add tail pointer too (depending on your problem)
public:


	LinkedList()
	{
		Head = nullptr;
	}

	//List is being desturcted ==> delete all items in the list
	~LinkedList()
	{
		DeleteAll();
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: PrintList.
	* prints the values of all nodes in a linked list.
	*/
	void PrintList()	const
	{

		Node<T>* p = Head;

		while (p)
		{
			cout << p->getItem();
			if (p->getNext())
			{
				cout << ",";
			}
			p = p->getNext();
		}
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: InsertBeg.
	* Creates a new node and adds it to the beginning of a linked list.
	*
	* Parameters:
	*	- data : The value to be stored in the new node.
	*/
	void InsertBeg(const T& data)
	{
		Node<T>* R = new Node<T>(data);
		R->setNext(Head);
		Head = R;

	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: DeleteAll.
	* Deletes all nodes of the list.
	*/
	void DeleteAll()
	{
		Node<T>* P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
	}



	////////////////     Requirements   ///////////////////
	//
	// Implement the following member functions


	//[1]InsertEnd 
	//inserts a new node at end if the list
	void InsertEnd(const T& data)
	{
		if (Head==nullptr)
		{
			Node<T>* newnode = new Node<T>;
			newnode->setItem(data);
			Head = newnode;
			Head->setNext(nullptr);
			return;
		}
		else
		{
			Node<T>* ptr = Head;
			while (ptr && ptr->getNext())
			{
				ptr = ptr->getNext();
			}
			Node<T>* newnode = new Node<T>;
			newnode->setItem(data);
			ptr->setNext(newnode);
			newnode->setNext(nullptr);
			return;
		}
	}

	//[2]Find 
	//searches for a given value in the list, returns true if found; false otherwise.
	bool Find(int Key, T*& item)
	{
		Node<T>* ptr = Head;
		Node<T>* ptr2;
		while (ptr)
		{
			ptr2 = ptr->getItem();
			if (*ptr2 == Key)
			{
			}
		}
	}

	//[3]CountOccurance
	//returns how many times a certain value appeared in the list
	int CountOccurance(const T& value);

	//[4] DeleteFirst
	//Deletes the first node in the list
	void DeleteFirst()
	{
		if (!Head)
		{
			return;
		}
		else if (!Head->getNext())
		{
			delete Head;
			Head=nullptr;
		}
		else
		{
			Node <T>* nptr = Head;
			Head = Head->getNext();
			delete nptr;
			nptr=nullptr;
		}
	}


	//[5] DeleteLast
	//Deletes the last node in the list
	void DeleteLast();

	//[6] DeleteNode
	//deletes the first node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNode(const T& value)
	{
		/*if (!Head)
			return false;
		Node<T>* ptr = Head;
		Node<T>* ptr2 = Head;
		while (ptr)
		{
			if (ptr->getNext())
			{
				ptr = ptr->getNext();
				if (ptr->getItem() == value)
				{
					if (ptr->getNext())
						ptr2->setNext(ptr->getNext());
					else
						ptr2->setNext(nullptr);
					delete ptr;
				}
			}
			ptr2 = ptr;
			ptr = ptr->getNext();
		}

		if (Head && Head->getItem() == value)
		{
			ptr = Head;
			if (!Head)
				return false;
			if (ptr->getNext())
				Head = ptr->getNext();
			else
				Head = nullptr;
			delete ptr;
		}
		return true;*/
		Node<T>* P = Head;
		if (Head && !Head->getNext())
		{
			Head = nullptr;
			delete P;
			P = nullptr;
			return true;
		}
		if (Head && Head->getItem() == value)
		{
			Head = Head->getNext();
			delete P;
			P = nullptr;
			return true;
		}
		while (P->getNext())
		{
			if (P->getNext()->getItem() == value)
			{
				Node<T>* Temp = P->getNext();
				P->setNext(Temp->getNext());
				delete Temp;
				Temp = nullptr;
				return true;
			}
			P = P->getNext();
		}
		return false;

	}

	//[7] DeleteNodes
	//deletes ALL node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNodes(const T& value);

	//[8]Merge
	//Merges the current list to another list L by making the last Node in the current list 
	//point to the first Node in list L
	void Merge(const LinkedList& L);

	//[9] Reverse
	//Reverses the linked list (without allocating any new Nodes)
	void Reverse();

	int getCount()
	{
		int count = 0;
		Node<T>* ptr = Head;
		while (ptr)
		{
			count++;
			ptr = ptr->getNext();
		}
		return count;
	}
	template<typename T>
	bool peek(T& H)
	{
		if (Head)
		{
			H = Head->getItem();
			return true;
		}
		return false;
	}
	bool  isEmpty()
	{
		if (!Head)
			return true;
		return false;
	}
};


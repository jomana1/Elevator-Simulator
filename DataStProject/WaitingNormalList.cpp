#include "WaitingNormalList.h"
//Inherited from LinkedList to be able to traverse the list.
Cargo* WaitingNormalList::GetCargofromID(int id)
{
	Node<Cargo*>* ptr = Head;
	while (ptr)
	{
		if (ptr->getItem()->getID() == id)
		{
			return ptr->getItem();
		}
		ptr = ptr->getNext();
	}

}
WaitingNormalList::WaitingNormalList()
{
	Head = nullptr;
}
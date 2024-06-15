#include "Cancellation.h"
#include "WaitingNormalList.h"
//Constructor that sets the data member ID with the passed id.
Cancellation::Cancellation(Company*mc,int id):Event(mc)
{
	ID = id;

}
//Calls cancel in class company.
void Cancellation::Execute()
{
	c = getCompany();
	c->cancel(ID);

}
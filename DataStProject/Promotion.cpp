#include "Promotion.h"
//Constructor that sets data member with passed variables.
Promotion::Promotion(Company*mc,int id, int cost):Event(mc)
{
	ID = id;
	additionalCost = cost;

}
//calls promote in class Company. 
void Promotion::Execute()
{
	c = getCompany();
	c->promote(ID, additionalCost);
}

#include "Preparation.h"
//Constructor that sets data member with passed variables.
Preparation::Preparation(Company*mc,int id, Time et, int lt, char t, int d, int c):Event(mc)
{
	ID = id;
	eventTime = et;
	loadTime = lt;
	type = t;
	distance = d;
	cost = c;
}
//calls prepare in class Company.
void Preparation::Execute()
{
	c = getCompany();
	c->prepare(ID, eventTime, loadTime, type, distance, cost);
}

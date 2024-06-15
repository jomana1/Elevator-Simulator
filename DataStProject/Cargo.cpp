#include "Cargo.h"
//Non-Default Constructor.
Cargo::Cargo(int id, Time pt, int lt, char t, int d, int c)  
{
	ID = id;
	preparationTime = pt;
	loadTime = lt;
	type = t;
	distance = d;
	cost = c;
}
//Setters and Getters.
void Cargo::setType(char T)
{
	type = T;
}
void Cargo::setCost(int c)
{
	cost = c;
}
void Cargo::setMoveTime(Time t)
{
	moveTime = t;
}
void Cargo::setCDT(Time t)
{
	CDT = t;
}
void Cargo::setTID(int t)
{
	TID = t;
}

char Cargo::getType()
{
	return type;
}
int Cargo::getID()
{
	return ID;
}
int Cargo::getDistance()
{
	return distance;
}
int Cargo::getLoadTime()
{
	return loadTime;
}
int Cargo::getCost()
{
	return cost;
}
int Cargo::getTID()
{
	return TID;
}
Time Cargo::getwaitTime()
{
	waitingTime = moveTime - preparationTime;
	return waitingTime;
}
Time Cargo::getPreparationTime()
{
	return preparationTime;
}
Time Cargo::getCDT()
{
	return CDT;
}

//Function that calculates priority using some of the data members and returns it.
int Cargo::calcPriority()
{
	int P = 2 * cost + (-2 * preparationTime.getTotalHours()) + distance; //Increasing cost and distance increases priority while increasing time decreases priority.
	return P;
}

//Operator overloading to print ID given Cargo Pointer.
ostream& operator << (ostream& output, const Cargo* c)  
{
	output << c->ID;
	return output;
}


















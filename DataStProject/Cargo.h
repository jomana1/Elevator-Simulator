#pragma once
#include "Time.h"
#include <iostream>
using namespace std;
class Cargo
{
	int ID;
	Time preparationTime;
	int loadTime;
	char type;
	int distance;
	int cost;
	Time moveTime;
	Time waitingTime;
	Time CDT;
	int TID; //id of the truck carrying the cargo


public:
	
	Cargo(int id, Time pt, int lt, char t, int d, int c); //non default constructor that initializes some data members
	                                  //setters and getters for data members
	void setType(char T);
	void setCost(int c);
	void setMoveTime(Time);
	void setCDT(Time t);
	void setTID(int);

	char getType();
	int getID();
	int getDistance();
	int getLoadTime();
	int getCost();
	int getTID();
	Time getwaitTime();
	Time getPreparationTime();
	Time getCDT();

	int calcPriority(); 
	friend ostream& operator <<(ostream& output, const Cargo* c); 
};


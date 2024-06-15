#pragma once
#include "Event.h"
class Preparation : public Event
{
	int ID;
	Time eventTime;
	int loadTime;
	char type;
	int distance;
	int cost;
public:
	Preparation(Company*mc,int id, Time et, int lt, char t, int d, int c);
	virtual void Execute();
	
};

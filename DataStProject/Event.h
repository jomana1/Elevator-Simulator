#pragma once
#include "Time.h"
#include "Cargo.h"
#include "Company.h"
class Company;
class Event
{
protected:
	Time eventTime;
	Company* c;
	Event(Company*);
public:
	void setEventTime(Time t);
	Time getEventTime();
	Company* getCompany();
	virtual void Execute() = 0;
};


#pragma once
#include "Event.h"
class Event;
class Promotion :
    public Event
{
    int ID;
    int additionalCost;
public:
    Promotion(Company*mc,int id, int cost = 0);
    virtual void Execute();
};


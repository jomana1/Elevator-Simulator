#pragma once
#include "Event.h"
class Cancellation :
    public Event
{
    int ID;
public:
    Cancellation(Company*mc,int id);
    virtual void Execute();
};


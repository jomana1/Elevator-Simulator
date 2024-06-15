#pragma once
#include "LinkedLists.h"
#include "Cargo.h"



class WaitingNormalList :
    public LinkedList<Cargo*>
{

public:
    WaitingNormalList();
    Cargo* GetCargofromID(int id);
};



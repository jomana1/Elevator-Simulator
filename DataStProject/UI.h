#pragma once
#include <iostream>
#include "Company.h"
#include <string>
using namespace std;
class Company;
class UI
{
	Company* c;
public:
	UI(Company* myCompany);
	void StartUp(string& input, string& output, char& mode);
	void Print(Time &currentTime, LinkedPQueue<Cargo*>&WaitingVIP, WaitingNormalList &WaitingNormal, LinkedQueue<Cargo*> &WaitingSpecial, LinkedPQueue<Truck*>&MovingT, LinkedQueue<Cargo*>DeliveredCargos, LinkedPQueue<Truck*>&LoadingT, LinkedQueue<Truck*>&EmptyVIPT, LinkedQueue<Truck*>&EmptyNormalT, LinkedQueue<Truck*>&EmptyspecialT, LinkedQueue<Truck*>&IncheckupNormalT,LinkedQueue<Truck*>&IncheckupSpeciallT,LinkedQueue<Truck*>&IncheckupVIPT);
	void PrintEnd();
};
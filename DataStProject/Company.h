#pragma once
#include "LinkedQueue.h"
#include "Cargo.h"
#include "Event.h"
#include "LinkedPQueue.h"
#include "Truck.h"
#include "WaitingNormalList.h"
#include "UI.h"
class Event;
class UI;
class Company
{
private:
	Time currentTime;
	Time AutoP;
	Time MaxWaitTime;
	string input;
	string output;
	char mode;
	UI* interface;
	int normalCargos;
	int specialCargos;
	int VIPCargos;
	int normalTrucks;
	int specialTrucks;
	int VIPTrucks;
	int autoPromoted;
	bool currentlyLoadingNormal; //flags for loading trucks of each type.
	bool currentlyLoadingSpecial;
	bool currentlyLoadingVIP;



	LinkedQueue<Event*>EventList;

	LinkedPQueue<Cargo*>WaitingVIP;
	WaitingNormalList WaitingNormal;
	LinkedQueue<Cargo*> WaitingSpecial;

	LinkedPQueue<Truck*>MovingT;

	LinkedQueue<Cargo*>DeliveredCargos;


	LinkedPQueue<Truck*>LoadingT;

	LinkedQueue<Truck*>EmptyVIPT;
	LinkedQueue<Truck*>EmptyNormalT;
	LinkedQueue<Truck*>EmptyspecialT;

	LinkedQueue<Truck*>IncheckupNormalT;
	LinkedQueue<Truck*>IncheckupSpecialT;
	LinkedQueue<Truck*>IncheckupVIPT;
public:
	Company();
	void SetFiles();
	void Simulate();
	void TakeEvent();
	bool prepare(int id, Time pt, int lt, char t, int d, int c);
	void cancel(int i);
	void promote(int i, int cost);
	void AutoPromote();
	void LoadToMoving();        //checks for trucks that are ready to move.
	void Moving();              //move loaded trucks.
	void Loading();             //assigns cargos to trucks to start loading.
	void LoadVIP(Truck*T);
	void LoadSpecial(Truck*T);
	void LoadNormal(Truck*T);
	void Deliver();             //checks if a cargo is delivered and unloads it.
	void toCheckUp();           //moves a truck to check up if needed.
	void CheckupToEmpty();      //moves a truck that completed check up to empty.
	void toEmpty(Truck* T);     //moves the truck to the empty trucks list.
	void OutputFile();
	void PrintModeAndIncrement();
	void Print();
	~Company();


};



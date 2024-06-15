#include "Company.h"
#include "Preparation.h"
#include "Cancellation.h"
#include "Promotion.h"
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

//Constructor that sets starting time to day 1 and calls StartUp from UI to get inputs from user.
Company::Company()
{
	currentTime.setTime(1, 0);
	interface->StartUp(input, output, mode);
	SetFiles(); //reads from input file and initializez data members.
	normalCargos=0;
	specialCargos=0;
	VIPCargos=0;
	autoPromoted = 0;
	currentlyLoadingNormal=false; //flags for the loading rule of no 2 trucks of same type loading at the same time
	currentlyLoadingSpecial=false;
	currentlyLoadingVIP=false;
}
//reads from input file and initializez data members.
void Company::SetFiles()
{
	Truck* trucks;
	ifstream inputFile(input);
	if (inputFile.is_open() == false)
	{
		return;
	}
	int sTs, nTs, vTs, nTc, sTc, vTc, nJ, chN, chS, chV, numOfEvents, ap, mwt;
	char eventType;
	char Type;
	Time t;
	int dt, dh;
	char colon;
	int id, cost, lt, dist;
	inputFile >> normalTrucks >> specialTrucks >> VIPTrucks >> nTs >> sTs >> vTs >> nTc >> sTc >> vTc >> nJ >> chN >> chS >> chV >> ap >> mwt >> numOfEvents;
	AutoP.setTime(ap, 0);
	MaxWaitTime.setTime(0, mwt);
	for (int i = 0; i < normalTrucks; i++)
	{
		trucks = new Truck('N', nTc, chN, nTs, nJ);
		EmptyNormalT.enqueue(trucks);
	}
	for (int i = 0; i < specialTrucks; i++)
	{
		trucks = new Truck('S', sTc, chS, sTs, nJ);
		EmptyspecialT.enqueue(trucks);
	}
	for (int i = 0; i < VIPTrucks; i++)
	{
		trucks = new Truck('V', vTc, chV, vTs, nJ);
		EmptyVIPT.enqueue(trucks);
	}
	for (int i = 0; i < numOfEvents; i++)
	{
		inputFile >> eventType;
		if (eventType == 'R')
		{
			inputFile >> Type >> dt >> colon >> dh >> id >> dist >> lt >> cost;
			if (dh < 5)
			{
				dh = 5;
			}
			t.setTime(dt, dh);
			Preparation* P = new Preparation(this, id, t, lt, Type, dist, cost);
			P->setEventTime(t);
			EventList.enqueue(P);
		}
		else if (eventType == 'X')
		{
			inputFile >> dt >> colon >> dh >> id;
			if (dh < 5)
			{
				dh = 5;
			}
			t.setTime(dt, dh);
			Cancellation* C = new Cancellation(this, id);
			C->setEventTime(t);
			EventList.enqueue(C);

		}
		else if (eventType == 'P')
		{
			inputFile >> dt >> colon >> dh >> id >> cost;
			if (dh < 5)
			{
				dh = 5;
			}
			t.setTime(dt, dh);
			Promotion* Pr = new Promotion(this, id, cost);
			Pr->setEventTime(t);
			EventList.enqueue(Pr);
		}
	}
}
//This is the function where all events are handled and data are printed.
void Company::Simulate()
{

	while ((!EventList.isEmpty()) || !WaitingSpecial.isEmpty() || !WaitingVIP.isEmpty() || !WaitingNormal.isEmpty() || !LoadingT.isEmpty() || !MovingT.isEmpty() || !IncheckupNormalT.isEmpty() || !IncheckupVIPT.isEmpty() || !IncheckupSpecialT.isEmpty()) //Operating conditions.
	{
		if (currentTime.getHour() >= 5 && currentTime.getHour() <= 23)//working hours
		{
			TakeEvent();//checks if there are events in current time
			AutoPromote();//checks if any normal cargos will auto promote
			LoadToMoving(); // moves loaded trucks  //executed before moving to reset the loading flags
			Loading();//loads waiting cargos if there are any
		}
		Deliver();//delivers cargos and returns trucks to the company
		CheckupToEmpty();//returns trucks that finished checkup to empty list
		PrintModeAndIncrement();//increases current time and prints with appropriate mode
	}
	OutputFile();
	interface->PrintEnd();
}
void Company::TakeEvent()
{
	Event* e;
	if (EventList.peek(e))
	{
		Time x = e->getEventTime();
		while ((x < currentTime) || (x == currentTime)) //Checks if an event time has come to be executed.
		{
			if (x < currentTime)
			{
				x.setTime(currentTime.getDay(), 5);
				e->setEventTime(x);
			}
			if (EventList.isEmpty()) //Checks if the list is now empty.
			{
				break;
			}
			EventList.dequeue(e);
			e->Execute();
			delete e;
			e = nullptr;
			if (EventList.peek(e))
			{
				x = e->getEventTime();
			}
		}
	}
}
//Constructs cargo and adds it to the appropriate list.
bool Company::prepare(int id, Time pt, int lt, char t, int d, int c)
{
	Cargo* eventCargo = new Cargo(id, pt, lt, t, d, c);
	if (t == 'N')
	{
		Company::WaitingNormal.InsertEnd(eventCargo);
	}
	else if (t == 'S')
	{
		Company::WaitingSpecial.enqueue(eventCargo);
	}
	else if (t == 'V')
	{
		int P = eventCargo->calcPriority();
		Company::WaitingVIP.enqueue(eventCargo, P);
	}
	return true;
}
//Gets the needed cargo given its ID and deletes it.
void Company::cancel(int i)
{
	Cargo* C = NULL;
	C = WaitingNormal.GetCargofromID(i);
	if (C)
	{
		WaitingNormal.DeleteNode(C);
	}
}
//Gets the needed cargo given its ID, updates the cost and promotes cargo to VIP.
void Company::promote(int i, int cost)
{
	Cargo* C = NULL;
	C = WaitingNormal.GetCargofromID(i);
	if (C)
	{
		C->setType('V');
		C->setCost(C->getCost() + cost);
		int p = C->calcPriority();
		WaitingVIP.enqueue(C, p);
		WaitingNormal.DeleteNode(C);
	}
}
void Company::AutoPromote()
{
	Cargo* c;
	Promotion* autoPromote;
	if (WaitingNormal.peek(c))
	{
		while (((currentTime - c->getPreparationTime()) >= AutoP) && !WaitingNormal.isEmpty()) //checks if cargo has been waiting AutoP time.
		{

			autoPromote = new Promotion(this, c->getID(), 0);
			autoPromote->Execute();
			delete autoPromote;
			autoPromote = nullptr;
			autoPromoted++;
			WaitingNormal.peek(c);
		}
	}
}
//checks if loading truck reached moving time
void Company::LoadToMoving()
{

	Truck* T;
	if (LoadingT.peek(T))
	{
		while (LoadingT.peek(T) && currentTime >= T->getReadytoMove())
		{
			Moving();
		}
	}
}
//moves trucks from loading to moving
void Company::Moving()
{
	Truck* T;
	LoadingT.dequeue(T);
	char cargoType = T->getCargoType();
	//resets flags of loading cargos
	if (cargoType == 'N')
	{
		currentlyLoadingNormal = false;
	}
	else if (cargoType == 'S')
	{
		currentlyLoadingSpecial = false;
	}
	else
	{
		currentlyLoadingVIP = false;
	}
	MovingT.enqueue(T, T->getPriority());//moves trucks to moving
}
//loads cargos on to empty trucks and moves trucks from empty to loading
void Company::Loading()
{
	Cargo* C;
	Truck* T;
	int vWC = 0, sWC = 0, nWC = 0;
	//chacks for waiting VIP cargos
	if (WaitingVIP.peek(C))
	{
		vWC = WaitingVIP.getPCount();
	}
	if (EmptyVIPT.peek(T))
	{

		if (vWC >= T->getcapacity() && currentlyLoadingVIP == false)//loading rule
		{
			EmptyVIPT.dequeue(T);
			LoadVIP(T);
		}
	}
	else if (EmptyNormalT.peek(T))//if no empty VIP trucks checks normal
	{
		if (vWC >= T->getcapacity() && currentlyLoadingVIP==false)
		{
			EmptyNormalT.dequeue(T);
			LoadVIP(T);
		}
	}
	else if (EmptyspecialT.peek(T))//if no VIP nor Normal trucks checks special
	{
		if (vWC >= T->getcapacity() && currentlyLoadingVIP == false)
		{
			EmptyspecialT.dequeue(T);
			LoadVIP(T);
		}
	}
	if (WaitingSpecial.peek(C))//checks if there are waiting special cargos
	{
		sWC = WaitingSpecial.getCount();
	}
	if (EmptyspecialT.peek(T))//checks for empty special trucks
	{
		if (sWC >= T->getcapacity() && currentlyLoadingSpecial == false)//loading rule
		{
			EmptyspecialT.dequeue(T);
			LoadSpecial(T);
		}
		else
		{

			if (WaitingSpecial.peek(C))
			{

				if (currentTime - C->getPreparationTime() >= MaxWaitTime)//checks if there are special cargos that exceeded max wait time 
				{
					EmptyspecialT.dequeue(T);
					for (int i = 0; i < sWC; i++)//loads all waiting special cargos
					{

						WaitingSpecial.dequeue(C);
						T->Load(C);
						specialCargos++;
					}
					T->setLoadTime();
					T->setDelivery();
					if ((currentTime + T->getLoadTime()).getHour() < 5)
					{
						T->setReadytoMove(currentTime + T->getLoadTime() + 5);
					}
					else
					{
						T->setReadytoMove(currentTime + T->getLoadTime());
					}
					T->calcCDT();
					LoadingT.enqueue(T, -1 * (T->getReadytoMove().getTotalHours()));
				}
			}
		}
		if (WaitingNormal.peek(C))//checks if there are waiting normal cargos
		{
			nWC = WaitingNormal.getCount();
		}
		if (EmptyNormalT.peek(T))
		{
			if (nWC >= T->getcapacity() && currentlyLoadingNormal == false)//loading rule
			{
				EmptyNormalT.dequeue(T);
				LoadNormal(T);
			}
			else
			{
				if (WaitingNormal.peek(C))
				{
					if (currentTime - C->getPreparationTime() >= MaxWaitTime)//checks if any normal cargos exceeded max wait time
					{
						EmptyNormalT.dequeue(T);

						for (int i = 0; i < nWC; i++)//loads all waiting cargos
						{
							T->Load(C);
							WaitingNormal.DeleteFirst();
							WaitingNormal.peek(C);
							normalCargos++;
						}
						T->setLoadTime();
						T->setDelivery();
						if ((currentTime + T->getLoadTime()).getHour() < 5)
						{
							T->setReadytoMove(currentTime + T->getLoadTime() + 5);
						}
						else
						{
							T->setReadytoMove(currentTime + T->getLoadTime());
						}
						T->calcCDT();
						LoadingT.enqueue(T, -1 * (T->getReadytoMove().getTotalHours()));
					}
				}
			}
		}
		else if (EmptyVIPT.peek(T))//if no normal trucks available checks vip trucks
		{
			if (nWC >= T->getcapacity() && currentlyLoadingNormal == false)
			{
				EmptyVIPT.dequeue(T);
				LoadNormal(T);
			}
			else
			{
				if (WaitingNormal.peek(C))
				{
					if (currentTime - C->getPreparationTime() >= MaxWaitTime)//wait time condition
					{
						EmptyVIPT.dequeue(T);
						for (int i = 0; i < nWC; i++)//loads all waiting cargos
						{
							T->Load(C);
							WaitingNormal.DeleteFirst();
							WaitingNormal.peek(C);
							normalCargos++;
						}
						T->setLoadTime();
						T->setDelivery();
						if ((currentTime + T->getLoadTime()).getHour() < 5)
						{
							T->setReadytoMove(currentTime + T->getLoadTime() + 5);
						}
						else
						{
							T->setReadytoMove(currentTime + T->getLoadTime());
						}
						T->calcCDT();
						LoadingT.enqueue(T, -1 * (T->getReadytoMove().getTotalHours()));
					}
				}
			}
		}

	}
}
void Company::LoadVIP(Truck*T)
{
	Cargo* C;
	for (int i = 0; i < T->getcapacity(); i++)
	{

		WaitingVIP.dequeue(C);
		T->Load(C);//load cargo to truck
		VIPCargos++;
	}
	currentlyLoadingVIP = true;
	T->setLoadTime();//calculates and sets the loading time of the truck according to load time of each cargo
	T->setDelivery();//calculates and sets the arrival time after delivering cargos of the empty truck
	if ((currentTime + T->getLoadTime()).getHour() < 5)//off hours rule(doesnt begin  untill after off hours)
	{
		T->setReadytoMove(currentTime + T->getLoadTime() + 5);
	}
	else
	{
		T->setReadytoMove(currentTime + T->getLoadTime());
	}
	T->calcCDT();//calculates each cargo delivery time inside the truck 
	LoadingT.enqueue(T, -1 * (T->getReadytoMove().getTotalHours())); //loads Truck inside Loading trucks accoring to first to finish loading
}
void Company::LoadSpecial(Truck*T)
{
	Cargo* C;
	for (int i = 0; i < T->getcapacity(); i++)
	{
		WaitingSpecial.dequeue(C);
		T->Load(C);
		specialCargos++;
	}
	currentlyLoadingSpecial = true;
	T->setLoadTime();
	T->setDelivery();
	if ((currentTime + T->getLoadTime()).getHour() < 5)
	{
		T->setReadytoMove(currentTime + T->getLoadTime() + 5);
	}
	else
	{
		T->setReadytoMove(currentTime + T->getLoadTime());
	}
	T->calcCDT();
	LoadingT.enqueue(T, -1 * (T->getReadytoMove().getTotalHours()));
}
void Company::LoadNormal(Truck*T)
{
	Cargo* C;
	for (int i = 0; i < T->getcapacity(); i++)
	{
		WaitingNormal.peek(C);
		WaitingNormal.DeleteFirst();
		T->Load(C);
		normalCargos++;
	}
	currentlyLoadingNormal = true;
	T->setLoadTime();
	T->setDelivery();
	if ((currentTime + T->getLoadTime()).getHour() < 5)
	{
		T->setReadytoMove(currentTime + T->getLoadTime() + 5);
	}
	else
	{
		T->setReadytoMove(currentTime + T->getLoadTime());
	}
	T->calcCDT();
	LoadingT.enqueue(T, -1 * (T->getReadytoMove().getTotalHours()));
}
//delivers cargos
void Company::Deliver()
{
	Truck* T;
	if (MovingT.peek(T))//checks if there are moving trucks
	{
		while (MovingT.peek(T) && currentTime.getTotalHours() == (-1 * T->getPriority()))//since priority is equivalent to the next action time we check if time for action is reached
		{
			if (T->isEmpty())//checks if truck is empty
			{
				Time t = T->deliveryTime();
				while (currentTime >= t && MovingT.peek(T))
				{

					T->addCurrJ();//increments journeys done by truck
					if (T->needCheckUp())//checks if it needs check up and enqueues to appropriate list
					{
						toCheckUp();
					}
					else
					{
						MovingT.dequeue(T);
						toEmpty(T);
					}
					if (MovingT.peek(T))
					{
						t = T->deliveryTime();
					}
				}
			}
			else
			{
				MovingT.dequeue(T);
				DeliveredCargos.enqueue(T->unLoad());
				MovingT.enqueue(T, T->getPriority());
			}
		}
	}
}
//checks if truck needs checkup after each journey 
void Company::toCheckUp()
{
	Truck* T;
	MovingT.dequeue(T);
	T->setEndCheckUp(currentTime + T->getMaintenanceTime());//sets the time truck goes back to empty trucks list(available)
	if (T->getType()== 'V')
	{
		IncheckupVIPT.enqueue(T);
	}
	else if (T->getType() == 'S')
	{
		IncheckupSpecialT.enqueue(T);
	}
	else if (T->getType() == 'N')
	{
		IncheckupNormalT.enqueue(T);
	}

}
//checks if truck in checkup finished checkup
void Company::CheckupToEmpty()
{
	Truck* T;
	while (IncheckupNormalT.peek(T) && currentTime >= T->getEndCheckUp())
	{
		IncheckupNormalT.dequeue(T);
		toEmpty(T);
	}
	while (IncheckupSpecialT.peek(T) && currentTime >= T->getEndCheckUp())
	{
		IncheckupSpecialT.dequeue(T);
		toEmpty(T);
	}
	while (IncheckupVIPT.peek(T) && currentTime >= T->getEndCheckUp())
	{
		IncheckupVIPT.dequeue(T);
		toEmpty(T);
	}
}
//moves truck from moving/checkup to empty
void Company::toEmpty( Truck* T)
{
	T->addJourney();//increments number of journeys
	T->setLoadTime();
	if (T->getType() == 'V')
	{
		EmptyVIPT.enqueue(T);
	}
	else if (T->getType() == 'S')
	{
		EmptyspecialT.enqueue(T);
	}
	else if (T->getType() == 'N')
	{
		EmptyNormalT.enqueue(T);
	}
}
void Company::OutputFile()
{
	Cargo* C;
	Truck* T;
	float tu=0;
	ofstream outputFile(output);
	int simT = currentTime.getTotalHours();//converts current time to hours
	Time wait;
	float active=0;
	int waitTemp = 0;
	wait.setTime(0, 0);
	if (outputFile.is_open() == false)
	{
		return;
	}
	outputFile << "CDT"<<"\t"<<"ID" << "\t" <<"PT" << "\t" <<"WT" << "\t" << "TID" << endl;
	//responsible for printing each cargo and its data
	while (DeliveredCargos.dequeue(C))
	{
		wait = wait + C->getwaitTime();     //increases waiting time with the waiting time of the dequeued cargo.
		outputFile << C->getCDT() <<"\t" << C->getID() << "\t" << C->getPreparationTime() << "\t" << C->getwaitTime() << "\t" << C->getTID() << endl;
		delete C;           //deleting the dynamically allocated cargo.
		C = nullptr;
	}
	outputFile << endl << "Cargos: " << normalCargos + specialCargos + VIPCargos << "[N: " << normalCargos << ", S: " << specialCargos << ", V: " << VIPCargos << "]" << endl;
	waitTemp = wait.getTotalHours();       //converting wait time to hours.
	waitTemp = waitTemp / (normalCargos + specialCargos + VIPCargos);    //calculating average wait time.
	wait.setTime(0, 0);
	wait = wait + waitTemp;      //converting it back to time;
	float aP = (autoPromoted == 0 && normalCargos == 0) ? 0 : (float)(autoPromoted*100) / (autoPromoted + normalCargos);      //handling division by zero.
	outputFile << "Cargo Avg Wait = " << wait << endl;
	outputFile << "Auto-promoted Cargos: " <<aP << "%" << endl;
	outputFile<<"Trucks: "<<normalTrucks+specialTrucks+VIPTrucks<< "[N: " << normalTrucks << ", S: " << specialTrucks << ", V: " << VIPTrucks << "]" << endl;
	while (EmptyNormalT.dequeue(T))
	{
		active = active + (T->getTruckActiveTime().getTotalHours());    //incrasing active time with the active time of the dequeued truck.
		tu = tu + T->getUtilization(simT);
		delete T;           //deleting the dynamically allocated Truck.
		T = nullptr;
	}
	while (EmptyspecialT.dequeue(T))
	{
		active = active + (T->getTruckActiveTime().getTotalHours());    //incrasing active time with the active time of the dequeued truck.
		tu = tu + T->getUtilization(simT);
		delete T;
		T = nullptr;

	}
	while (EmptyVIPT.dequeue(T))
	{
		active = active + (T->getTruckActiveTime().getTotalHours());    //incrasing active time with the active time of the dequeued truck.
		tu = tu + T->getUtilization(simT);
		delete T;
		T = nullptr;
	}
	active = (100*active)/ (simT*(normalTrucks + specialTrucks + VIPTrucks));    //calculating average active time.
	outputFile << "Avg Active Time = " << ((active))  <<"%" << endl;
	outputFile << "Avg Utilization = " << tu / (normalTrucks + specialTrucks + VIPTrucks) << "%";
}
void Company::PrintModeAndIncrement()
{
	//The following conditions determines how the data will be printed.
   //interactive mode
	if (mode == 'I')
	{
		if (cin.get() == '\n')       //waits for an enter from the user.
		{
			Print();
			++currentTime;
		}

	}
	//step by step mode
	else if (mode == 'T')
	{
		sleep_for(nanoseconds(10));
		sleep_until(system_clock::now() + seconds(1));   //waits 1 second before printing.
		Print();
		++currentTime;


	}
	//silent mode
	else
	{
		++currentTime;
	}

}
void Company::Print()//calls UI to print output screen
{
	interface->Print(currentTime, WaitingVIP, WaitingNormal, WaitingSpecial, MovingT, DeliveredCargos, LoadingT, EmptyVIPT, EmptyNormalT, EmptyspecialT, IncheckupNormalT, IncheckupSpecialT, IncheckupVIPT);
}
Company::~Company()
{
}
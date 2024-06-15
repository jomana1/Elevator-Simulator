#pragma once
#include "Cargo.h"
#include <iostream>
#include "LinkedPQueue.h"
#include "LinkedQueue.h"
#include "Time.h"
using namespace std;
class Truck
{
	char type;
	int truckCapacity;
	int maintenanceTime;
	int speed;
	int deliveryInterval;
	char status;
	int J;
	int currJ;
	int ID;
	static int countID; 
	LinkedPQueue<Cargo*> truckC;
	Time readyToMove;//time a truck has finished loading and is ready to move
	Time endCheckUp;//time a truck has finished check up and is ready to go back to empty
	int movedDistance;//sum of distance moved by truck
	int loadTimeNow;//loading time of all previously loaded cargos
	int totalLoad;
	Time truckActiveTime;
	int totalCargosDelivered;
	int totalJourneys;
	float truckUtilization;

public:
	Truck(char t, int tc,int mt, int s, int Jt);
                              //setters and getters for data members.
	void setDelivery();
	void setLoadTime();
	void setReadytoMove(Time t);
	void setEndCheckUp(Time);

	char getType();
	int getcapacity();
	int getID();
	int getSpeed();
	int getMaintenanceTime();
	int getLoadTime();
	int getJourneys();
	int getCargosDelivered();
	Time getReadytoMove();
	Time getEndCheckUp();
	Time getTruckActiveTime();
	float getUtilization(int simT);

	char getCargoType();
	int getFurthestCargo();
	int getPriority();
	void Load(Cargo*c);      //adds a cargo to the truck's queue of cargos.
	Cargo* unLoad();
	bool needCheckUp();      //checks if max no of journeys is reached.
	Time deliveryTime();
	Time topCargo();
	void addCurrJ();
	void addJourney();
	void calcCDT();
	bool isEmpty();
	friend ostream& operator << (ostream& output, Truck* t);
};



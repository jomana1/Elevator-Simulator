#include "Truck.h"
// Initializing static data member.
int Truck::countID = 0;
//Constructor that initializes all data members and generates new ID for each truck.
Truck::Truck(char t, int tc, int mt, int s, int Jt)
{
	movedDistance = 0;
	type = t;
	truckCapacity = tc;
	speed = s;
	maintenanceTime = mt;
	status = 'e';
	J = Jt;
	currJ = 0;
	countID++;
	ID = countID;
	readyToMove.setTime(9999999999999, 1);
	loadTimeNow = 0;
	totalLoad = 0;
	truckActiveTime.setTime(0,0);
	totalCargosDelivered = 0;
	totalJourneys=0;
}
//Setters and getters.

//Function that calculates delivery interval
void Truck::setDelivery()
{
	truckActiveTime=truckActiveTime+ (ceil(((float)getFurthestCargo() / speed))) + 2*totalLoad;//increases active time with the delivery time of the journey without the time to get back
	deliveryInterval = (ceil((float)(2*getFurthestCargo()) / speed)) + totalLoad;//calculates journey time including time getting back
}
void Truck::setLoadTime()
{
	LinkedQueue<Cargo*>Temp;
	Cargo* c;
	int count = truckC.getPCount();
	for (int i = 0; i < count; i++)
	{
		truckC.dequeue(c);
		totalLoad += c->getLoadTime();//sum of all loading time of cargos inside truck 
		Temp.enqueue(c);
	}
	for (int i = 0; i < count; i++)
	{
		Temp.dequeue(c);
		truckC.enqueue(c, -1 * (c->getDistance()));
	}
	totalLoad = (count == 0) ? 0 : totalLoad;//if no cargos resets total load 
	loadTimeNow = (count == 0) ? 0 : loadTimeNow;//if no cargos resets load time now
	//truckActiveTime = truckActiveTime + totalLoad;//increases truck active time with total load time
}
void Truck::setReadytoMove(Time t)
{
	readyToMove = t;
}
void Truck::setEndCheckUp(Time t)
{
	endCheckUp = t;
}
char Truck::getType()
{
	return type;
}
int Truck::getcapacity()
{
	return truckCapacity;
}
int Truck::getID()
{
	return ID;
}
int Truck::getSpeed()
{
	return speed;
}
int Truck::getMaintenanceTime()
{
	return maintenanceTime;
}
int Truck::getLoadTime()
{
	return totalLoad;
}
int Truck::getJourneys()
{
	return totalJourneys;
}
int Truck::getCargosDelivered()
{
	return totalCargosDelivered;
}
Time Truck::getReadytoMove()
{
	return readyToMove;
}
Time Truck::getEndCheckUp()
{
	return endCheckUp;
}
Time Truck::getTruckActiveTime()
{
	return truckActiveTime;
}
float Truck::getUtilization(int simT) //gets % Utilization of the truck given the total simulation time
{
	truckUtilization =(totalJourneys==0)? 0 : ((float)(100 * totalCargosDelivered) / (truckCapacity * totalJourneys)) *(((float)truckActiveTime.getTotalHours()) / simT);
	return truckUtilization;
}

char Truck::getCargoType()
{
	Cargo* C;
	truckC.peek(C);
	return C->getType();
}
//gets the distance of the furthest cargo on the truck.
int Truck::getFurthestCargo()
{
	Cargo* c;
	LinkedQueue<Cargo*>Temp;
	int max = 0;
	while (truckC.dequeue(c))
	{
		if (c->getDistance() > max)
		{
			max = c->getDistance();//sets greatest distance as max.
		}
		Temp.enqueue(c);//enqueues in temp to reserve the orginal queue of truck.
	}
	while (Temp.dequeue(c))
	{
		truckC.enqueue(c, -1 * (c->getDistance()));//returns to original queue.
	}
	return max;
}
int Truck::getPriority()
{
	Cargo* c;
	int p;
	if (truckC.peek(c))
	{
		p = -1 * (topCargo().getTotalHours());//if cargo exists priority equivalent to delivery time of top cargo
	}
	else
	{
		p = -1 * (deliveryTime().getTotalHours());//if no cargos priority equivalent to time to get back
	}
	return p;
}
void Truck::Load(Cargo* c)
{
	c->setTID(ID);
	truckC.enqueue(c, -1 * c->getDistance());//enqueues cargos to truck with priority of nearest distance
}
//removes cargo from truck 
Cargo* Truck::unLoad()
{
	Cargo* c;
	if (truckC.dequeue(c))
	{
		totalCargosDelivered++;//increments number of cargos delivered by this truck
		movedDistance = c->getDistance();//increments moved distance by cargo distance
	}
	return c;
}
//checks if cuurent journeys = journeys until checkup
bool Truck::needCheckUp()
{
	if (currJ == J)
	{
		currJ = 0;//resets current journeys if true
		return true;
	}
	else
	{
		return false;
	}
}
Time Truck::deliveryTime()
{
	return readyToMove + deliveryInterval;
}
//returns delivery time of the cargo in front
Time Truck::topCargo()
{
	Cargo* c;
	if (truckC.peek(c))
	{
		return (c->getCDT());
	}
	else
	{
		Time t;
		t.setTime(0, 0);
		return t;
	}
}
void Truck::addCurrJ()
{
	currJ++;
}
void Truck::addJourney()
{
	totalJourneys++;
}
void Truck::calcCDT()
{
	Cargo* C;
	LinkedQueue<Cargo*>Temp;
	while (truckC.dequeue(C))
	{
		C->setCDT(readyToMove + (ceil(((float)C->getDistance() / speed)) + C->getLoadTime() + loadTimeNow));//time for truck to reach distance + unload of all preavious cargos
		C->setMoveTime(readyToMove);
		Temp.enqueue(C);
		loadTimeNow += C->getLoadTime();//increment load time now by cargos unload time
	}
	while (Temp.dequeue(C))
	{
		truckC.enqueue(C, -1 * (C->getDistance()));
	}
}
bool Truck::isEmpty()
{
	Cargo* c;
	if (truckC.peek(c))
	{
		return false;
	}
	return true;
}
//Operator overloading to print ID given Truck Pointer, and Cargos on Truck IDs if exists.
ostream& operator << (ostream& output, Truck* t)
{
	LinkedPQueue<Cargo*>temp;
	Cargo* c;
	output << t->ID;
	if (t->truckC.peek(c))
	{
		if (c->getType() == 'N')
		{
			output << "[";
			t->truckC.PrintQueue();
			output << "]";
		}
		else if (c->getType() == 'S')
		{
			output << "(";
			t->truckC.PrintQueue();
			output << ")";
		}
		else
		{
			output << "{";
			t->truckC.PrintQueue();
			output << "}";
		}
	}
	return output;
}


















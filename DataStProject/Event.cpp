#include "Event.h"
//Constructor that sets the pointer to the given company
Event::Event(Company* myCompany)
{
	c = myCompany;
}
//Setters and getters
Company* Event::getCompany()
{
	return c;
}
Time Event::getEventTime()
{
	return eventTime;
}
void Event::setEventTime(Time t)
{
	eventTime = t;
}
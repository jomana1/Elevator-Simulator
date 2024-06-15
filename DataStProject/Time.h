#pragma once
#include <iostream>
using namespace std;
class Time
{
	int day;
	int hour;
public:
	void setTime(int d, int h);
	int getHour();
	int getDay();
	int getTotalHours();
	Time operator - (Time t);
	Time operator + (Time t);
	Time operator + (int t);
	void operator ++ ();
	bool operator >= (Time t);
	bool operator == (Time t);
	bool operator < (Time t);
	friend ostream& operator <<(ostream& output, const Time t);
};
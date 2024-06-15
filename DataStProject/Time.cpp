#include "Time.h"
//Setters and getters
void Time::setTime(int d, int h)
{
	if (d >= 0 && h >= 0)
	{
		day = d;
		hour = h;
	}
}
int Time::getDay()
{
	return day;
}
int Time::getHour()
{
	return hour;
}
int Time::getTotalHours()
{
	return day * 24 + hour;
}

//Operator overloading for various operators
Time Time::operator - (Time t)
{
	Time x;
	int h, d;
	d = day - t.day;
	h = hour - t.hour;
	if (h < 0)
	{
		h = h + 24;
		d--;
	}
	x.setTime(d , h);
	return x;
}
bool Time::operator >= (Time t)
{
	if (*this == t)
	{
		return true;
	}
	if (day > t.day)
	{
		return true;
	}
	else if (day < t.day)
	{
		return false;
	}
	else if (hour > t.hour)
	{
		return true;
	}
	else return false;
}
bool Time::operator == (Time t)
{
	if (day == t.day && hour == t.hour)
	{
		return true;
	}
	else return false;
}
void Time::operator ++ ()
{
	hour++;
	if (hour == 24)
	{
		hour = 0;
		day++;
	}
}
bool Time::operator < (Time t)
{
	if (day < t.day)
	{
		return true;
	}
	else
	{
		if (day == t.day)
		{
			if (hour < t.hour)
			{
				return true;
			}
		}
	}
	return false;
}
Time Time::operator + (int t)
{
	int h, d;
	d = day;
	h = hour + t;
	while (h >=24)
	{
		d++;
		h =h- 24;
	}
	Time x;
	x.setTime(d, h);
	return x;
}
ostream& operator << (ostream& output, const Time t)
{
	output <<t.day<<":"<<t.hour;
	return output;
}
Time Time::operator + (Time t)
{
	Time x;
	int h, d;
	d = day + t.day;
	h = hour + t.hour;
	if (h >24)
	{
		h-=24;
		d++;
	}
	x.setTime(d, h);
	return x;
}
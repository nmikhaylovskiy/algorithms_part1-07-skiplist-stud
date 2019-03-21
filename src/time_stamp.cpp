////////////////////////////////////////////////////////////////////////////////
// Module Name:  time_stamp.h/cpp
// Authors:      Leonid Dworzanski, Sergey Shershakov
// Version:      2.0.0
// Date:         28.10.2018
//
// This is a part of the course "Algorithms and Data Structures"
// provided by  the School of Software Engineering of the Faculty
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////


#include "time_stamp.h"

//-----------------------------------------------------------------------------

TimeStamp::TimeStamp()
{
    time_t tmp = time(nullptr);
    _time = *localtime(&tmp);      // разыменовываем результат-структуру и копируем большой объект
}

//-----------------------------------------------------------------------------

// Constructor with parameters
TimeStamp::TimeStamp(int year, int mon, int mday, int hour, int min, int sec)
{
    _time.tm_year = year-1900;
    _time.tm_mon  = mon;
    _time.tm_mday = mday;
    _time.tm_hour = hour;
    _time.tm_min  = min;
    _time.tm_sec  = sec;
	normalize();
}

//-----------------------------------------------------------------------------

// Auxiliary comparison function for time stamps
int TimeStamp::compareTo (const TimeStamp & another) const
{
     if (_time.tm_year > another._time.tm_year)
        return 1;

    if (_time.tm_year == another._time.tm_year)
	{
        if (_time.tm_mon > another._time.tm_mon)
            return 1;

        if (_time.tm_mon == another._time.tm_mon)
		{
            if (_time.tm_mday > another._time.tm_mday)
                return 1;

            if (_time.tm_mday == another._time.tm_mday)
			{
                if (_time.tm_hour > another._time.tm_hour)
                    return 1;
                if (_time.tm_hour == another._time.tm_hour)
				{
                    if (_time.tm_min > another._time.tm_min)
                        return 1;

                    if (_time.tm_min == another._time.tm_min)
					{
                        if (_time.tm_sec > another._time.tm_sec)
                            return 1;

                        if (_time.tm_sec == another._time.tm_sec)
							return 0; // equal
					}
				}
			}
		}
	}

	return -1;

   // Красиво, правда?
}

//-----------------------------------------------------------------------------

void TimeStamp::normalize()
{
    _time.tm_isdst = -1;
    time_t tTime = mktime(&_time);
    _time = *localtime(&tTime);
}

//-----------------------------------------------------------------------------

std::istream& operator>>(std::istream& in, TimeStamp& tsInput)
{
	char tmp;
	do
	{
        in >> tsInput._time.tm_year;
        if (!in)
            break;

        tsInput._time.tm_year -= 1900;
		in >> tmp; // skip .
        if (!in)
            break;

        in >> tsInput._time.tm_mon;
        if (!in)
            break;

		in >> tmp; // skip .
        if (!in)
            break;

        in >> tsInput._time.tm_mday;
        if (!in)
            break;

        in >> std::ws;                  // skip whitespaces (кто бы думал!)
        if (!in)
            break;

        in >> tsInput._time.tm_hour;
        if (!in)
            break;

		in >> tmp; // skip :
        if (!in)
            break;

        in >> tsInput._time.tm_min;
        if (!in)
            break;

		in >> tmp; // skip :
        if (!in)
            break;

        in >> tsInput._time.tm_sec;
        if (!in)
            break;

		tsInput.normalize();
	} while (false);

	return in;
}

//-----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& out, const TimeStamp& tsOutput)
{
    char buf[TimeStamp::SIZE_MAXSTRTIME];

    // вызов этой функции небезопасен, но она относится к базовому блоку C-функций
    strftime(buf, TimeStamp::SIZE_MAXSTRTIME, "%Y.%m.%d %H:%M:%S", &tsOutput._time);
	out << buf;

	return out;
}

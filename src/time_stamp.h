////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief      Contains interface part of Timestamp class.
///
/// \author     Leonid W. Dworzanski <leo@mathtech.ru> © 2014–2016.
/// \author     Sergey A. Shershakov <sshershakov@hse.ru> © 2015–2018.
/// \version    2.0.0
/// \date       28.10.2018
///             This is a part of the course "Algorithms and Data Structures"
///             provided by  the School of Software Engineering of the Faculty
///             of Computer Science at the Higher School of Economics.
///
/// When altering code, a copyright line must be preserved.
///
////////////////////////////////////////////////////////////////////////////////


#ifndef CYBERPOLICE_TIME_STAMP_H_
#define CYBERPOLICE_TIME_STAMP_H_

#include <iostream>
#include <ctime>

//using namespace std;

/*! ****************************************************************************
 *  \brief Используется для представления временного штампа «дата+время».
 ******************************************************************************/
class TimeStamp
{
public:
    // Constants

    /// 22 is the maximum length of such timestamps (да, 22!).
    static const size_t SIZE_MAXSTRTIME = 25;
public:
    /// Default constructor.
    TimeStamp();

    /// Initialize with date components.
	TimeStamp(int year, int mon = 1, int mday = 1, int hour = 0, int min = 0, int sec = 0);

    /// Checks if timestamps are equal.
    bool operator== (const TimeStamp& another) const
    {
        return (compareTo(another) == 0);
    }

    /// Checks if timestamps are not equal.
    bool operator!= (const TimeStamp& another) const
    {
        return compareTo(another) != 0;
    }

    /// Checks if left timestamp is earlier than the right one.
    bool operator< (const TimeStamp& another) const
    {
        return (compareTo(another) == -1);
    }

    /// Checks if left timestamp is earlier than or equal to the right one.
    bool operator<= (const TimeStamp& another) const
    {
        int compare = compareTo(another);
        return (compare == 0 || compare == -1);
    }

    bool operator>= (const TimeStamp& another) const
    {
        return !operator<(another);
    }

    bool operator> (const TimeStamp& another) const
    {
        return !operator<=(another);
    }

public:
    /// \brief \a operator>> Reads timestamp from the input stream \a in.
    /// The format is: 2015.06.17 10:33:03
    friend std::istream &operator>>(std::istream& in, TimeStamp& tsInput);

    /// \brief \a operator<< Puts timestamp to the output stream out
    /// The format is: 2015.06.17 10:33:03
    friend std::ostream &operator<<(std::ostream& out, const TimeStamp& tsOutput);

protected:

    /*!
     *  \brief Auxiliary comparison function for time stamps.
     *  \param \a another timestamp to caompare with.
     *  \return 0 if two timestamps are equal to each other, 1 if this > anothers,
     *      -1  --  this < another.
     */
    int compareTo(const TimeStamp& another) const;

    /// Structure "struct tm" normalization fills "week day" and similiar fields.
    /// This is needed for correct work of other <ctime> functions.
    void normalize();

protected:
    /// struct tm - internal structure of <ctime> library.
    struct tm _time;
}; // class TimeStamp

#endif // CYBERPOLICE_TIME_STAMP_H_

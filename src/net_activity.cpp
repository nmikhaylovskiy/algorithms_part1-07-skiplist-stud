////////////////////////////////////////////////////////////////////////////////
// Module Name:  net_activity.h/cpp
// Authors:      Leonid Dworzanski, Sergey Shershakov
// Version:      2.0.0
// Date:         28.10.2018
//
// This is a part of the course "Algorithms and Data Structures"
// provided by  the School of Software Engineering of the Faculty
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////

#include "net_activity.h"

/// Puts the \a na network actitivty to the out stream.
/// The format is: "mary218 msdn.com".
std::ostream& operator<<(std::ostream& out, const NetActivity& na)
{
    out << na.user << " " << na.host;

    return out;
}

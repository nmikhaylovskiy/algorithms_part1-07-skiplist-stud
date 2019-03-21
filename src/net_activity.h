////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief      Contains interfaces for the following classes:
///                 NetActivity.
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

#ifndef CYBERPOLICE_NET_ACTIVITY_H_
#define CYBERPOLICE_NET_ACTIVITY_H_


#include <string>
#include <iostream>

/*! ****************************************************************************
 *  The NetActivity struct
 ******************************************************************************/
struct NetActivity
{
    /// Name of the user.
    std::string user;

    /// URL of the host.
    std::string host;
};

/// Operator is declared in the global scope!
std::ostream& operator<< (std::ostream& out, const NetActivity& na);


#endif // CYBERPOLICE_NET_ACTIVITY_H_

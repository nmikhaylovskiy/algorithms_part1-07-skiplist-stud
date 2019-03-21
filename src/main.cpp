////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief      Defines the entry point for the demo application.
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


#include <iostream>
#include <string>
#include <ctime>

#include "skip_list.h"
#include "journal_net_activity.h"


/*! ****************************************************************************
 *  The TickTack class is used for tracking exscution time.
 ******************************************************************************/
class TickTack
{
public:
    TickTack() : _ts(0) {}

public:
    /// Starts a time counting.
    void tick()
    {
        _ts = clock();
    }

    /// Stops the timer and prints a message.
    void tack(const std::string& action = "TickTack")
    {
        std::cout << action << " : " <<
                     clock() - _ts << "/ " << CLOCKS_PER_SEC << " seconds" << std::endl;
    }

protected:
    long _ts;
}; // class TickTack


//------------------------------------------------------------------------------

/// Test procedure
template <int numLevels>
void testJournal(const JournalNetActivity<numLevels>& journal,
                 std::string host, const TimeStamp& from, const TimeStamp& to)
{
    TickTack tmr;

    static int cntTest = 0;
    cntTest++;
    std::cout << "===============================================================================" << std::endl;
    std::cout << "SkipList: Test #" << cntTest << std::endl << std::endl;

    tmr.tick();
    journal.outputSuspiciousActivities(host, from, to, std::cout);
    tmr.tack("Test took");
    std::cout << "===============================================================================" << std::endl;

    std::cout << std::endl;
}

const std::string LOG_FOLDER = "../../data/";

//------------------------------------------------------------------------------

int main (int /*argc*/, char* /*argv*/[])
{
    std::cout << "Hello World!\n\n";

    TickTack tmr;

    // Test SkipList over integers
    typedef SkipList<int,int,15> TypeSkipList;
    typedef typename TypeSkipList::Node TypeSkipNode;

    TypeSkipList list;
    tmr.tick();
    int i = 0;
    for (i = 0; i < 200 * 1000; ++i)
        list.insert(i, i);

    tmr.tack("SkipList over integers creation");

    TypeSkipNode* node = nullptr;

    tmr.tick();
    // We call here the method findFirst inherited from OrderedList
    node = list.OrderedList <int,int,TypeSkipNode >::findFirst(i - 5);
    std::cout << "Element found:" << node->value << std::endl;
    tmr.tack("OrderedList over integers searching");

    tmr.tick();
    node = list.findFirst(i - 5);
    std::cout << "Element found:" << node->value << std::endl;
    tmr.tack("SkipList over integers searching");
    // End of: Test SkipList over integers

    try{
        // Test #1
        JournalNetActivity<5> journal1;
        journal1.parseLog(LOG_FOLDER + "test1.log");
        testJournal(journal1, "e-maxx.ru", TimeStamp(2015,6,10,10,33,1), TimeStamp(2015,6,10,10,33,4));

        // Test #2
        JournalNetActivity<5> journal2;
        journal2.parseLog(LOG_FOLDER + "test2.log");
        testJournal(journal2, "verisicretproxi.com", TimeStamp(2015,6,10,10,33,54), TimeStamp(2015,6,10,10,33,54));

//        // Test #3
//        JournalNetActivity<5> journal3;
//        tmr.tick();
//        journal3.parseLog(LOG_FOLDER + "test3.log");
//        tmr.tack("Parsing test3.log");
//        testJournal(journal3, "verisicretproxi.com", TimeStamp(2015,6,10,12,27,45), TimeStamp(2015,6,10,12,27,59));

//        // Test #4
//        JournalNetActivity<5> journal4;
//        tmr.tick();
//        journal4.parseLog(LOG_FOLDER + "test4.log");
//        tmr.tack("Parsing test4.log:");
//        testJournal(journal4, "verisicretproxi.com", TimeStamp(2015,6,10,22,30,20), TimeStamp(2015,6,10,22,30,50));

    }
    catch(std::exception &e)
    {
        std::cout << e.what();
    }
    catch (...)
    {
        std::cout << "Something wrong happend." << std::endl;
    }

    std::cout << std::endl;

    return 0;
}

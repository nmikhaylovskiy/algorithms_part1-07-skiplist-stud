////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief     Unit tests for TimeStamp class.
///
/// \author    Sergey Shershakov
/// \version   0.2.0
/// \date      23.01.2017
///            This is a part of the course "Algorithms and Data Structures" 
///            provided by  the School of Software Engineering of the Faculty 
///            of Computer Science at the Higher School of Economics.
///
/// Gtest-based unit test.
/// The naming conventions imply the name of a unit-test module is the same as 
/// the name of the corresponding tested module with _test suffix
/// 
////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "journal_net_activity.h"


#include "time_stamp.h"

using namespace std;

stringstream getLog1()
{
    return stringstream(
            "2015.06.10 10:33:01 porosenok751 msdn.com\n"
            "2015.06.10 10:33:01      ivan736 www.nist.gov/dads\n"
            "2015.06.10 10:33:01       ann525 verisicretproxi.com\n"
            "2015.06.10 10:33:02      user126 e-maxx.ru\n"
            "2015.06.10 10:33:03      mary218 msdn.com\n"
            "2015.06.10 10:33:04      mary634 www.nist.gov/dads\n"
            "2015.06.10 10:33:04      ivan756 en.wikipedia.org/wiki/Trie\n"
            "2015.06.10 10:33:05 porosenok798 mathoverflow.com\n"
            "2015.06.10 10:33:06      mary697 mathoverflow.com\n"
            "2015.06.10 10:33:06       zlo529 mathoverflow.com\n"
            "2015.06.10 10:33:07     kotik386 mathoverflow.com\n"
            "2015.06.10 10:33:07 porosenok781 dictionary.cambridge.com\n"
            "2015.06.10 10:33:07       zlo835 msdn.com\n"
            "2015.06.10 10:33:07      ivan736 e-maxx.ru\n"
            "2015.06.10 10:33:07      kotik386 e-maxx.ru\n"
            "2015.06.10 10:33:08      ivan736 e-maxx.ru\n"
            "2015.06.10 10:33:08      mary857 dictionary.cambridge.com\n"
            "2015.06.10 10:33:08     kotik772 msdn.com\n"
            "2015.06.10 10:33:08     kotik127 dictionary.cambridge.com\n"
            "2015.06.10 10:33:09       zlo555 en.wikipedia.org/wiki/Trie\n"
            "2015.06.10 10:33:09      mary831 mathoverflow.com\n"
            "2015.06.10 10:33:09       ann176 e-maxx.ru\n"
            "2015.06.10 10:33:10      ivan218 en.wikipedia.org/wiki/Trie"
    );
}


TEST(Journal, log1_emaxx)
{
    JournalNetActivity<5> journal;
    stringstream log1 = getLog1();
    journal.parseLogFromStream(log1);

    stringstream output;
    output.str(""); // clear
    journal.outputSuspiciousActivities("e-maxx.ru", TimeStamp(2015, 6, 10, 10, 33, 1),
                                       TimeStamp(2015, 6, 10, 10, 33, 4), output);
    EXPECT_EQ(output.str(), "2015.07.10 10:33:02 user126 e-maxx.ru\n");

    output.str(""); // clear
    journal.outputSuspiciousActivities("e-maxx.ru", TimeStamp(2015, 6, 10, 10, 33, 7),
                                       TimeStamp(2015, 6, 10, 10, 33, 8), output);
    EXPECT_EQ(output.str(), "2015.07.10 10:33:07 ivan736 e-maxx.ru\n"
                            "2015.07.10 10:33:07 kotik386 e-maxx.ru\n"
                            "2015.07.10 10:33:08 ivan736 e-maxx.ru\n");

    output.str(""); // clear
    journal.outputSuspiciousActivities("unknown", TimeStamp(2000, 6, 10, 10, 33, 0), TimeStamp(2018, 6, 10, 10, 33, 8),
                                       output);
    EXPECT_EQ(output.str(), "");
}






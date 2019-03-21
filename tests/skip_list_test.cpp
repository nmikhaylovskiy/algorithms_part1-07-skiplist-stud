////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief     Unit tests for TimeStamp class.
///
/// \author    Pavel Pertsukhov
/// \version   0.2.0
/// \date      23.01.2018
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


#include "ordered_list.h"
#include "skip_list.h"

#include <vector>

using namespace std;

const int MAX_LEVELS = 10;
// Disables all large debug prints
const bool PRINT_ALL = true;

/** \brief СкипЛист на стероидах.
 *
 *  Умеет печатать себя с помощью printRefs, printValues, printKeys.
 *  Ещё умеет запрашивать нужную ноду через operator[], вычислять длину себя, и среднюю длину уровней.
 *  Может создаваться по заданным значениям ключей и их уровней.
 */
class TestSkipList
        : public SkipList<std::string, int, MAX_LEVELS>
{
public:
    explicit TestSkipList(double prob = 0.5)
        : SkipList(prob)
    {}

    /// Constr with initial values and levels. Levels must be in [0..MAX_LEVELS]
    TestSkipList(const vector<int>& keys,
                 const vector<int>& levels,
                 const vector<string>& values = vector<string>() )
        : SkipList()
    {
        if (keys.size() != levels.size())
            throw invalid_argument("keys.size != levels.size");

        if (!values.empty() && !levels.empty() && values.size() != levels.size())
            throw invalid_argument("values.size != levels.size");

        Node* last = _preHead;
        for (int i = 0; i < keys.size(); ++i)
        {
            string value = values.empty() ? "val" + to_string(i) : values[i];
            Node* newNode = new Node(keys[i], value);
            newNode->levelHighest = levels[i] - 1;
            if (levels[i] > MAX_LEVELS || levels[i] < 0)
                throw invalid_argument("some level is not in [0..MAX_LEVELS]");

            last->next = newNode;
            newNode->next = _preHead;

            last = newNode;
        }

        for (int i = 0; i < MAX_LEVELS; ++i)
        {
            Node* prev = _preHead;

            Node* curr = _preHead;
            while (curr->next != _preHead)
            {
                curr = curr->next;

                if (curr->levelHighest >= i)
                {
                    prev->nextJump[i] = curr;
                    prev = curr;
                }
            }
            prev->nextJump[i] = _preHead;
        }
    }

    double getProbability()
    {
        return _probability;
    }

    /// -1 returns _preHead.
    Node* operator[](int pos)
    {
        if (pos < -1)
            throw invalid_argument("pos < -1");
        Node* curr = _preHead;
        while (pos-- != -1)
            curr = curr->next;
        return curr;
    }

    int size()
    {
        int ans = 0;
        Node* curr = _preHead;
        while (curr->next != _preHead)
        {
            ans++;
            curr = curr->next;
        }
        return ans;
    }

    double meanHeight()
    {
        long hSum = 0;
        int size = 0;
        Node* curr = _preHead;
        while (curr->next != _preHead)
        {
            curr = curr->next;
            hSum += curr->levelHighest + 1;
            size++;
        }
        return (double) hSum / size;
    }

    /// Displays only heights of all nodes. If you need to validate references, use PRINT_REF.
    void printKeys()
    {
        if (!PRINT_ALL) return;

        printf("\nPRINT_KEYS\n");
        for (int i = MAX_LEVELS - 1; i >= -1; --i)
        {
            printf("%2d pr ", i);
            Node* curr = _preHead;
            while (curr->next != _preHead)
            {
                curr = curr->next;
                if (curr->levelHighest < i)
                    printf("--- ");
                else
                    printf("%3d ", curr->key);
            }
            printf("pr\n");
        }
        cout << endl;
    }

    /// Works like PRINT_KEYS, but prints values instead of keys.
    /// size - max size of string value
    void printValues(unsigned int size = 5)
    {
        if (!PRINT_ALL) return;

        string pattern_val = "%" + to_string(size) + "s ";
        string pattern_sparse = string(size, '-') + " ";

        printf("\nPRINT_VALUES\n");
        for (int i = MAX_LEVELS - 1; i >= -1; --i)
        {
            printf("%2d pr ", i);
            Node* curr = _preHead;
            while (curr->next != _preHead)
            {
                curr = curr->next;
                if (curr->levelHighest < i)
                    printf(pattern_sparse.c_str());
                else
                    printf(pattern_val.c_str(), curr->value.c_str());
            }
            printf("pr\n");
        }
        cout << endl;
    }

    /// It is like PRINT_KEYS, but it iterates over references in sparse levels.
    void printRefs()
    {
        if (!PRINT_ALL) return;

        printf("\nPRINT_REFS\n");
        for (int i = MAX_LEVELS - 1; i >= 0; --i)
        {
            printf("%2d pr ", i);
            Node* sparse = _preHead;
            Node* full = _preHead->next;
            while (full != _preHead)
            {
                if (sparse->nextJump[i] == full->next)
                {
                    printf("--> ");
                } else if (sparse->nextJump[i] == full)
                {
                    printf("%3d ", full->key);
                    sparse = sparse->nextJump[i];
                } else
                {
                    printf("--- ");
                }
                full = full->next;
            }
            printf("pr\n");
        }
        printf("-1 pr ");
        Node* curr = _preHead->next;
        while (curr != _preHead)
        {
            printf("%3d ", curr->key);
            curr = curr->next;
        }
        printf("pr\n");
        cout << endl;
    }


    string joinKeysToString()
    {
        stringstream ss;
        Node* curr = _preHead;
        while (curr->next != _preHead)
        {
            curr = curr->next;
            ss << curr->key << " ";
        }
        return ss.str().substr(0, ss.str().size() - 1);
    }

    string joinValuesToString()
    {
        stringstream ss;
        Node* curr = _preHead;
        while (curr->next != _preHead)
        {
            curr = curr->next;
            ss << curr->value << " ";
        }
        return ss.str().substr(0, ss.str().size() - 1);
    }


    /// Checks if all levels have cycled refs (start and end with preHead).
    void checkRefs()
    {
        int size = this->size();

        for (int level = -1; level < MAX_LEVELS; ++level)
        {
            bool foundPreHead = false;
            Node* curr = _preHead;
            // iterate until we reach size or find preHead
            for (int i = 0; i <= size; ++i)
            {
                Node* next = level == -1 ? curr->next : curr->nextJump[level];
                if (next == _preHead)
                {
                    foundPreHead = true;
                    break;
                }
                curr = next;
            }
            EXPECT_TRUE(foundPreHead) << "not found cycled preHead on level " << to_string(level)
                                      << ", list size is " << size;
        }

    }
};


TEST(SkipList, simple)
{
    TestSkipList list();

//    TestSkipList list1(0.3);
//    TestSkipList list2(0.6);
//    for (int i = 0; i < 20; ++i)
//    {
//        list1.insert("val", i);
//        list2.insert("val", i);
//    }
//    cout << "list1" << endl;
//    list1.printRefs();
//    cout << "list2" << endl;
//    list2.printRefs();
//    list2.printValues();
//    list2.printKeys();

}

TEST(SkipList, initTest)
{
    TestSkipList list(0.2);
    EXPECT_EQ(list.getProbability(), 0.2);
    EXPECT_NE(list.getPreHead(), nullptr);
    EXPECT_EQ(list.getPreHead()->next, list.getPreHead());
    EXPECT_EQ(list.getPreHead()->levelHighest, MAX_LEVELS - 1);
}


TEST(SkipList, insertSimple)
{
    TestSkipList list;

    list.insert("hello", 1);
    list.insert("val", 3);
    list.insert("val2", 2);
    list.insert("val", 5);
    list.insert("val", 4);
    list.insert("val", 100);

    list.printRefs();
    list.printValues();

    EXPECT_EQ(list[0]->key, 1);
    EXPECT_EQ(list[1]->key, 2);
    EXPECT_EQ(list[2]->key, 3);
    EXPECT_EQ(list[3]->key, 4);
    EXPECT_EQ(list[4]->key, 5);
    EXPECT_EQ(list[5]->key, 100);
}

TEST(SkipList, insertRepeating)
{
    TestSkipList list;

    list.insert("one1", 1);
    list.insert("three1", 3);
    list.insert("one2", 1);
    list.insert("one3", 1);
    list.insert("four1", 4);
    list.insert("three2", 3);
    list.insert("one4", 1);

    list.printValues();
    list.checkRefs();

    EXPECT_EQ(list[0]->value, "one1");
    EXPECT_EQ(list[1]->value, "one2");
    EXPECT_EQ(list[2]->value, "one3");
    EXPECT_EQ(list[3]->value, "one4");
    EXPECT_EQ(list[4]->value, "three1");
    EXPECT_EQ(list[5]->value, "three2");
    EXPECT_EQ(list[6]->value, "four1");
}

TEST(SkipList, oneProbability)
{
    TestSkipList list(1);
    for (int i = 0; i < 10; ++i)
    {
        list.insert("val", i);
    }
    list.printRefs();

    list.checkRefs();

    EXPECT_DOUBLE_EQ(list.meanHeight(), MAX_LEVELS);
}


TEST(SkipList, findFirst_Simple)
{
    TestSkipList list(
            vector<int>{10, 20, 30, 40, 50, 60, 70, 90},
            vector<int>{1, 2, 3, 0, 2, 0, 1, 0}
    );
    list.printRefs();
    list.printValues();

    EXPECT_EQ(list.findFirst(10), list[0]);
    EXPECT_EQ(list.findFirst(20), list[1]);
    EXPECT_EQ(list.findFirst(30), list[2]);
    EXPECT_EQ(list.findFirst(40), list[3]);
    EXPECT_EQ(list.findFirst(50), list[4]);
    EXPECT_EQ(list.findFirst(60), list[5]);
    EXPECT_EQ(list.findFirst(70), list[6]);
    EXPECT_EQ(list.findFirst(90), list[7]);
}


TEST(SkipList, findFirst_Unknown)
{
    TestSkipList list(
            vector<int>{10, 20, 30, 40, 50, 60, 70, 90},
            vector<int>{1, 2, 3, 0, 2, 0, 1, 0}
    );
    list.printRefs();
    list.printValues();

    EXPECT_EQ(list.findFirst(80), nullptr);
    EXPECT_EQ(list.findFirst(-10), nullptr);
    EXPECT_EQ(list.findFirst(1000), nullptr);
}


TEST(SkipList, findFirst_Repeating1)
{
    TestSkipList list(
            vector<int>{10, 20, 20, 20, 30},
            vector<int>{3, 2, 0, 1, 0}
    );
    list.printRefs();
    list.printValues();

    EXPECT_EQ(list.findFirst(20), list[1]);
}


TEST(SkipList, findLastLessThen1)
{
    TestSkipList list(
            vector<int>{10, 10, 20, 20, 30, 30},
            vector<int>{1, 3, 0, 2, 1, 0}
    );
    list.printRefs();
    list.printValues();

    EXPECT_EQ(list.findLastLessThan(10), list[-1]);
    EXPECT_EQ(list.findLastLessThan(20), list[1]);
    EXPECT_EQ(list.findLastLessThan(30), list[3]);

}

TEST(SkipList, removeNext1)
{
    TestSkipList list(
            vector<int>{10, 20, 30},
            vector<int>{6, 0, 3},
            vector<string>{"a", "b", "c"}
    );
    list.printRefs();
    list.printValues(1);
    list.checkRefs();

    cout << "removing 20 b" << endl;
    list.removeNext(list[0]); // b
    list.printRefs();
    list.checkRefs();
    EXPECT_EQ(list.joinValuesToString(), "a c");

    cout << "removing 30 c" << endl;
    list.removeNext(list[0]); // c
    list.printRefs();
    list.checkRefs();
    EXPECT_EQ(list.joinValuesToString(), "a");

    cout << "removing 10 a" << endl;
    list.removeNext(list[-1]); // a
    list.checkRefs();
    list.printRefs();
    EXPECT_EQ(list.joinValuesToString(), "");
    EXPECT_EQ(list.size(), 0);
}



TEST(SkipList, removeNext_Throws)
{
    TestSkipList list(
            vector<int>{10, 20, 30},
            vector<int>{6, 0, 3}
    );

    EXPECT_THROW(list.removeNext(list[2]), invalid_argument);
    EXPECT_THROW(list.removeNext(nullptr), invalid_argument);
}

TEST(SkipList, removeNext_Repeating1)
{
    TestSkipList list(
            vector<int>{10, 20, 20, 20, 30},
            vector<int>{6, 1, 3, 2, 1},
            vector<string>{"a", "b", "c", "d", "e"}
    );
    list.printRefs();
    list.printValues(1);
    list.checkRefs();

    cout << "removing b" << endl;
    list.removeNext(list[0]);
    list.printRefs();
    list.checkRefs();
    EXPECT_EQ(list.joinValuesToString(), "a c d e");

    cout << "removing c" << endl;
    list.removeNext(list[0]);
    list.printRefs();
    list.checkRefs();
    EXPECT_EQ(list.joinValuesToString(), "a d e");

    cout << "removing d" << endl;
    list.removeNext(list[0]);
    list.printRefs();
    list.checkRefs();
    EXPECT_EQ(list.joinValuesToString(), "a e");
}

// TODO: check more situations with repeating keys.
// For example, like this: https://pastebin.com/Ky5dJqma




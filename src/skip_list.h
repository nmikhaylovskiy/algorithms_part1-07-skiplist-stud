////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief      Contains interfaces for the following classes:
///                 NodeSkipListAbstract, NodeSkipList, SkipList.
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


#ifndef CYBERPOLICE_SKIP_LIST_H_
#define CYBERPOLICE_SKIP_LIST_H_

#include "ordered_list.h"


/*! ****************************************************************************
 *  \brief A node of skip-list data structure
 *
 *  The lowest (= dense) level is implemented with \a next.
 *  Warning! The most dense level is \a next, not nextJump[0]!!!
 ******************************************************************************/
template <class Value, class Key, int numLevels, class Next>
struct NodeSkipListAbstract
        : public NodeWithKeyAbstract<Value, Key, Next >
{
    /// Alias for the base class.
    typedef NodeWithKeyAbstract<Value, Key, Next > Base;

    /// \brief Stores Skip List sparse levels.
    ///
    /// \a (numLevels-1) is the highest/sparsest level.
    Next* nextJump[numLevels];

    /// \brief Current highest level of the node
    ///
    /// Important!!
    /// -1 means that there are no sparse numLevels.
    ///  0 = nextJump[0] is enabled and contains sparse jumps
    /// (numLevels-1) is the maximal value for \a levelHighest.
    ///
    /// Note: In industrial envirnoment I would use 0 for no \a numLevels,
    /// but it may confuse students.
    int levelHighest;
};

//==============================================================================



/*! ****************************************************************************
 *  Declares a node for SkipList .
 ******************************************************************************/
template <class Value, class Key, int numLevels>
class NodeSkipList
        : public NodeSkipListAbstract<Value, Key, numLevels, NodeSkipList<Value, Key, numLevels> >
{
public:
    /// Alias for the base class.
    typedef NodeSkipListAbstract<Value, Key, numLevels, NodeSkipList<Value, Key, numLevels> > Base;

public:
    /// Default constructor.
	NodeSkipList();

    /// Init with a key.
    NodeSkipList(const Key& tkey);

    /// Init with a key and a value.
    NodeSkipList(const Key& tkey, const Value& val);

protected:
    /// Clears all next links - dense and sparse levels and other fields.
    void clear();
};

//==============================================================================



/*! ****************************************************************************
 *  Declares SkipList.
 ******************************************************************************/
template <class Value, class Key, int numLevels >
class SkipList
        : public OrderedList < Value, Key, NodeSkipList<Value, Key, numLevels> >
{
public:
    /// Alias for base class type.
    typedef OrderedList<Value, Key, NodeSkipList<Value, Key, numLevels> > Base;

    /// Alias for corresponding list node.
    typedef NodeSkipList<Value,Key, numLevels> Node;


    /// \brief Constructor initializes with a probability.
    /// \param probability is the probability of each sparse level to appear.
    SkipList(double probability = 0.5);

    /// Virtual destructor: must take into account different levels!
    virtual ~SkipList();


    /// \brief Insert a new element into your list.
    ///
    /// Think very hard how will you guarantee thatnew element will be
    /// included in sparse levels correctly!
    ///
    /// Make different test. Check border cases - empty list, end of the list,
    /// et cetera ...
    virtual void insert(const Value& val, const Key& key);

    /// \brief Remove the node from the list and delete it from the memory.
    ///
    /// Check if an idiot called your function
    /// (i.e. don't delete pprehead occasionaly, and other cases)
    /// (Throw std::invalid_argument in such case.)
    ///
    /// Continue to think hard about sparse levels.
    /// Check different cases.
    virtual void removeNext(Node* nodeBefore);

    /// \brief Find the last element with key strictly less than key.
    ///
    /// You have to do it in log(n) time,  i.e. you have to use sparse levels
    /// correctly from the highest to the lowest.
    /// 
    /// If the key is less than the first element or the list is empty, returns _preHead.
    virtual Node* findLastLessThan(const Key& key) const;

    /// \brief Find the first element with key equal to key.
    ///
    /// You have to do it in log(n) time, i.e. you have to use sparse levels
    /// correctly from the highest to the lowest.
    /// 
    /// If nothing was found, returns nullptr.
    virtual Node* findFirst(const Key& key) const;

protected:
    /// Stores the probability of the next level to appear.
    double _probability;
}; // class SkipList


//==============================================================================

// Move out "implementation" to a separate header.
#include "skip_list.hpp"


#endif // CYBERPOLICE_SKIP_LIST_H_


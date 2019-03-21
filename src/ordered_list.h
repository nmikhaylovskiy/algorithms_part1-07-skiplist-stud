////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief      Contains interfaces for the following classes:
///                 NodeAbstract, NodeWithKeyAbstract, OrderedList.
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

#ifndef CYBERPOLICE_ORDERED_LIST_H_
#define CYBERPOLICE_ORDERED_LIST_H_

/*!****************************************************************************
 *  Abstract list node with a value.
 *****************************************************************************/
template <class Value, class Next>
struct NodeAbstract
{
    /// Default constructor.
    NodeAbstract () : next(nullptr) { }

    /// Init with value.
    NodeAbstract (const Value& value) : next(nullptr), value(value) { }

    //----<Fields>-----
    Value value;            ///< Stores value;
    Next* next;             ///< Stores ptr to a next node;
};


//=============================================================================
// NOTE:
// We don't have Node class (only NodeAbstract),
// because we don't need to implement simple (unordered) list.
//=============================================================================


/*!****************************************************************************
 *  Abstract node with a value and a key
 *  key is an ordering key
 *****************************************************************************/
template <class Value, class Key, class Next>
struct NodeWithKeyAbstract
        : public NodeAbstract <Value, Next>
{
    /// For convenience, we define an alias for the base datatype.
    typedef NodeAbstract <Value, Next > Base;

    /// Default constructor.
    NodeWithKeyAbstract()
        : Base()
    {}

    /// Init with a key.
    NodeWithKeyAbstract(const Key& key)
        : Base()
        , key(key)
    { }

    /// Init with a key and a value.
    NodeWithKeyAbstract(const Key& key, const Value& val)
        : Base(val)
        , key(key)
    { }

    //----<Fields>-----
    Key key;                ///< Stores a key together with \a value and \a next.
};

//=============================================================================


/*!****************************************************************************
 *  Concrete NodeWithKey class.
 *****************************************************************************/
template <class Value, class Key>
struct NodeWithKey
        : public NodeWithKeyAbstract <Value, Key, NodeWithKey<Value,Key> >
{
    /// For convenience, we define an alias for the base datatype.
    typedef NodeWithKeyAbstract <Value, Key, NodeWithKey<Value,Key> > Base;

	NodeWithKey()  {}
    NodeWithKey(const Key& key)
        : Base(key)
    { }

    NodeWithKey(const Key& key, const Value& val):
        Base(key, val)
    { }
};

//=============================================================================


/*!****************************************************************************
 *  \brief Represents an ordered list.
 *  Due to virtual nature of major methods, we do not declare them as inline ones.
 *****************************************************************************/
template <class Value, class Key, class Node = NodeWithKey<Value,Key> >
class OrderedList
{
public:
	typedef NodeWithKey<Value,Key> TypeNode;

    /// Default constructor.
    OrderedList();

    /// Virtual destructor.
    virtual ~OrderedList();

    // TODO: деструктор!

    /// Inserts a new node with the given (value == val) and (key == tkey).
    virtual void insert(const Value& val, const Key& tkey);

    /// Removes node just after \a nodeBefore.
    virtual void removeNext(Node* nodeBefore);

    /// Finds the last node in the list, which key is strictly less
    /// than \a key argument.
    virtual Node* findLastLessThan(const Key& key) const;

    /// Finds the first node in the list, which key is equal to
    /// \a key argument.
    virtual Node* findFirst(const Key& key) const;


    virtual Node* getPreHead() const;

protected:
    // Sentinel element - placed before first and after last elements
    Node* _preHead;
};


// Move out "implementation" to a separate header.
#include "ordered_list.hpp"


#endif // CYBERPOLICE_ORDERED_LIST_H_

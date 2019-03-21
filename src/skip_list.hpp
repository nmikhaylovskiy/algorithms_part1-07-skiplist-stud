////////////////////////////////////////////////////////////////////////////////
// Module Name:  skip_list.h/hpp
// Authors:      Leonid Dworzanski, Sergey Shershakov
// Version:      2.0.0
// Date:         28.10.2018
//
// This is a part of the course "Algorithms and Data Structures"
// provided by  the School of Software Engineering of the Faculty
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////

// !!! DO NOT include skip_list.h here, 'cause it leads to circular refs. !!!

#include <cstdlib>
#include <stdlib.h> 

//==============================================================================
// class NodeSkipList
//==============================================================================

template <class Value, class Key, int numLevels>
void NodeSkipList<Value, Key, numLevels>::clear(void)
{
    for (int i = 0; i < numLevels; ++i)
        Base::nextJump[i] = 0;

    Base::levelHighest = -1;
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(void)
{
    clear();
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(const Key& tkey)
{
    clear();

    Base::Base::key = tkey;
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(const Key& tkey, const Value& val)
{
    clear();

    Base::Base::key = tkey;
    Base::Base::value = val;
}


//==============================================================================
// class SkipList
//==============================================================================

template <class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::SkipList(double probability)
{
    _probability = probability;

    // Lets use m_pPreHead as a final sentinel element
    for (int i = 0; i < numLevels; ++i)
        Base::_preHead->nextJump[i] = Base::_preHead;

    Base::_preHead->levelHighest = numLevels - 1;
}


template<class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::insert(const Value &val, const Key &key)
{
    //Определяем колиичество уровней элемента
    int level = -1;
    while(((double) rand() / RAND_MAX) < _probability)
        //Чтобы среднее колличество элементов на уровне numLevels - 1 было в 2 раза больше чем на  numLevels - 2
        level = (level + 1) % numLevels;

    //Создаем новый элемент списка
    NodeSkipList<Value, Key, numLevels>* insNode = new NodeSkipList<Value, Key, numLevels>();
    insNode->levelHighest = level;
    insNode->value = val;
    insNode->key = key;

    //Проходимся по уровням с максимального по минимальный и ищем промежуток, в который нужно вставить элемент
    NodeSkipList<Value, Key, numLevels>* cNode = this->_preHead;
    for(int i = cNode->levelHighest; i >= 0; --i) {
        //Проходимся по уровням
        while(cNode->nextJump[i] != this->_preHead && cNode->nextJump[i]->key < key)
            cNode = cNode->nextJump[i];

        //Обновляем уровни для списка (нужно для корректного добавления элемента в список)
        if(i <= level) {
            insNode->nextJump[i] = cNode->nextJump[i];
            cNode->nextJump[i] = insNode;
        }
    }

    //Находим точное место, куда надо вставить элемент с ключем key
    while(cNode->next != this->_preHead && cNode->next->key < key)
        cNode = cNode->next;

    //Вставляем элемент в его позицию в списке
    insNode->next = cNode->next;
    cNode->next = insNode;
}



template<class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::removeNext(SkipList<Value, Key, numLevels>::Node *nodeBefore)
{
    if(nodeBefore == nullptr || nodeBefore == this->_preHead)
        throw std::invalid_argument("No such node!");

    //Проходимся по уровням с максимального по минимальный и ищем промежуток, в котором может находиться элемент
    NodeSkipList<Value, Key, numLevels>* cNode = this->_preHead;
    for(int i = cNode->levelHighest; i >= 0; --i) {
        while(cNode->nextJump[i] != this->_preHead && cNode->nextJump[i]->key < nodeBefore->key)
            cNode = cNode->nextJump[i];

        if(cNode->nextJump[i] == nodeBefore)
            cNode->nextJump[i] = cNode->nextJump[i]->nextJump[i];
    }

    //Находим точное место, где находится элемент с ключем key
    while(cNode->next != this->_preHead && cNode->next->key < nodeBefore->key)
        cNode = cNode->next;

    //Если элемент не найден, выбрасывается исключение
    if(cNode->next != nodeBefore)
        throw std::invalid_argument("No such node!");

    NodeSkipList<Value, Key, numLevels>* removedNode = cNode->next;
    cNode->next = removedNode->next;
    delete removedNode;
}

template<class Value, class Key, int numLevels>
typename SkipList<Value, Key, numLevels>::Node* SkipList<Value, Key, numLevels>::findLastLessThan(const Key &key) const
{
    NodeSkipList<Value, Key, numLevels>* cNode = this->_preHead;
    for(int i = cNode->levelHighest; i >= 0; --i)
        while(cNode->nextJump[i] != this->_preHead && cNode->nextJump[i]->key < key)
            cNode = cNode->nextJump[i];
    while(cNode->next != this->_preHead && cNode->next->key < key)
        cNode = cNode->next;

    return cNode;
}


template<class Value, class Key, int numLevels>
typename SkipList<Value, Key, numLevels>::Node* SkipList<Value, Key, numLevels>::findFirst(const Key &key) const
{
    NodeSkipList<Value, Key, numLevels>* node = this->_preHead;
    for(int i = node->levelHighest; i >= 0; --i) {
        if(node->nextJump[i]->key == key && node->nextJump[i] != this->_preHead)
            return node->nextJump[i];

        while(node->nextJump[i] != this->_preHead && node->nextJump[i]->key < key) //preHead
            node = node->nextJump[i];
    }

    while(node->next != this->_preHead && node->next->key < key)
        node = node->next;

    return node->next->key == key ? node->next : nullptr;
}

template<class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::~SkipList()
{
    //Nothing to do here
}

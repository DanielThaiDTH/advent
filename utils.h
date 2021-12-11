//Daniel Thai
//151900198

#ifndef UTILS_H
#define UTILS_H

#include <cstddef>

template<typename T>
class Stack
{
    struct Node
    {
        T _data;
        Node* _next;
        Node()
        {
            _next = nullptr;
        }
        Node(const T& data) : Node()
        {
            _data = T{data};
        }
        void setNext(Node* next)
        {
            _next = next;
        }
    };
    Node* _top;
    Node* _bottom;
    size_t _size;
public:
    Stack();
    ~Stack();
    void push(const T& data);
    void pop();
    const T& top() const;
    size_t size() const;
};


/*Implementations*/

template<typename T>
Stack<T>::Stack()
{
    _top = nullptr;
    _bottom = nullptr;
    _size = 0;
}


template<typename T>
Stack<T>::~Stack()
{
    if (_size > 0) {
        Node* nextToDel;
        Node* delNode = _top;

        while (delNode) {
            nextToDel = delNode->_next;
            delete delNode;
            delNode = nextToDel;
        }
    }
}


template<typename T>
void Stack<T>::push(const T& data)
{
    if (_size == 0) {
        _top = new Node(data);
        _bottom = _top;
        _size = 1;
    } else {
        Node* oldTop = _top;
        _top = new Node(data);
        _top->setNext(oldTop);
        _size++;
    }
}


template<typename T>
void Stack<T>::pop()
{
    if (_size == 0) {
        return;
    } else if (_size == 1) {
        delete _top;
        _top = nullptr;
        _bottom = nullptr;
        _size--;
    } else {
        Node* newTop = _top->_next;
        delete _top;
        _top = newTop;
        _size--;
    }
}


template<typename T>
const T& Stack<T>::top() const
{
    return _top->_data;
}


template<typename T>
size_t Stack<T>::size() const
{
    return _size;
}
#endif
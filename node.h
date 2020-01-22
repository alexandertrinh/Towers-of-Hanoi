#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <fstream>

using namespace std;

template<typename T>
struct node {
    T data;
    node* next;

    node(const T &d);
    ~node();
    node(const node<T> &other);
    node<T>& operator=(const node<T> &other);
};

/* Creates new node and assigns the node's data value as the given
 * T data type. The next pointer is null; this is a leaf node.
 * @param some data value with data type T
 */
template<typename T>
node<T>::node(const T &d) {
    data = d;
    next = nullptr;
}

/*
 * Destructor. Next pointer is null and data value is assigned an empty data value
 */
template<typename T>
node<T>::~node() {
    data = T();
    next = nullptr;
}

/*
 * ? Used to create a copy node.
 * @param node<T>
 */
template<typename T>
node<T>::node(const node<T> &other) {
    data = other.data;
    next = nullptr;
}

/*
 * Overloading the = operator. Used to assign the data value of the other node to
 * this node's data value
 */
template<typename T>
node<T>& node<T>::operator=(const node<T> &other) {
    if(this != &other) {
        data = other.data;
        next = nullptr;
    }
    return *this;
}

#endif // NODE_H

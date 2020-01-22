#ifndef QUEUE_H
#define QUEUE_H
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <cstring>
#include <sstream>
#include "node.h"

enum QUEUE_ERRORS { EMPTY_QUEUE, BAD_SIZE_QUEUE };

using namespace std;

template<typename T>
class Queue {

    public:
        Queue();
        ~Queue();
        Queue(const Queue<T> &other);
        Queue<T>& operator=(const Queue<T> &other);

        void enqueue(const T &data); //insert item into FIFO queue
        T dequeue(); //remove element from other side of queue
        bool empty() const;
        T peek();
        size_t size() const; //returns num of elements in queue

        Queue<T>& operator<<(const T &d);
        Queue<T>& operator>>(T &d);

        template<typename U>
        friend
        ostream& operator<<(ostream &out, const Queue<U> &q);

        template<typename U>
        friend
        istream& operator>>(istream &in, Queue<U> &q);

    private:
        void nukem();
        void copy(const Queue<T> &other);

        size_t mySize;
        node<T> *head, *tail;

};


template<typename T>
Queue<T>::Queue() {
    mySize = 0;
    head = tail = nullptr;
}

template<typename T>
Queue<T>::~Queue() {
    nukem();
    mySize = 0;
    head = tail = nullptr;
}

template<typename T>
Queue<T>::Queue(const Queue<T> &other) {
    nukem();
    copy(other);
}

template<typename T>
Queue<T>& Queue<T>::operator=(const Queue<T> &other) {
    if(this != &other) {
        nukem();
        copy(other);
    }
    return *this;
}

/*
 * Push a node to the back of the stack with T data into FIFO queue
 * @param T &data This data with Data type T, will be stored in a node
 */

template<typename T>
void Queue<T>::enqueue(const T &data) {
    node<T> *newNode = new node<T>(data);
    if (empty())
        head = tail = newNode;
    else {
        tail->next = newNode;
        tail = newNode;
    }
    ++mySize;
}

/*
 * Removes the first node in the queue and returns the data value
 * @return T data This is the data value at the beginnning of queue
 */
template<typename T>
T Queue<T>::dequeue() {
    if(empty())
        throw EMPTY_QUEUE;
    T temp = head->data; //get the data value into temp
    node<T> *tempNode = head; //tempNode points to current head
    head = head->next; //new head is the next

    tempNode->next = nullptr; //delete the node's contents
    tempNode->data = T();
    //delete *tempNode;

    --mySize;
    return temp;
}

/*
 * Is the queue empty or does it contain any nodes
 * @return true or false
 */
template<typename T>
bool Queue<T>::empty() const {
    return !mySize;
}

/*
 * This function returns the top data value carried by the top node
 * on the queue
 * @return returns head->data The data value carried by the top node
 */
template<typename T>
T Queue<T>::peek() {
    return head->data;
}

/*
 * This function returns the number of elements currently in the queue
 * @return retuns mySize
 */
template<typename T>
size_t Queue<T>::size() const {
    return mySize;
}

template<typename T>
Queue<T>& Queue<T>::operator<<(const T &d) {
    enqueue(d);
    return *this;
}

template<typename T>
Queue<T>& Queue<T>::operator>>(T &d) {
    dequeue(d);
    return *this;
}

template<typename T>
void Queue<T>::nukem() {
    while(!empty()) {
        dequeue();
    }
}

/*
 * This function makes a copy queue using this queue's data values
 * @param Queue<T> other queue
 */
template<typename T>
void Queue<T>::copy(const Queue<T> &other) {
    Queue<T> tempQueue = new Queue<T>();
    T tempT;
    for (size_t i = 0; i < this->mySize; i++) { //pop out every element in this queue
        tempT = this->dequeue();
        other.enqueue(tempT);
        tempQueue.enqueue(tempT);
    }
    for (size_t i = 0; i < this->mySize; i++) { //reinsert the elements into this queue
        this->enqueue(tempQueue.dequeue());
    }
}

template<typename  U>
ostream& operator<<(ostream& out, const Queue<U> &q) {
    if(&out != &cout) {
        out << "Queue size: " << q.mySize << endl;
    }
    for(size_t i=0; i<q.mySize; ++i) {
        out << q.dequeue() << endl;
    }
    return out;
}

template<typename U>
istream& operator>>(istream& in, Queue<U> &q) {
    q.nukem();
    q.capacity = q.mySize = q.head = q.tail = 0;
    if(&in != &cin) {
        string line;
        if(in.peek() == '\n')
            in.ignore(32767, '\n');
        getline(in, line, ':');
        in >> q.capacity;
        in.ignore(32767, '\n');
        getline(in, line, ':');
        in >> q.mySize;
        q.que = new U[q.capacity+1];
        for(size_t i=0; i<q.mySize; ++i, ++q.tail)
            in >> q.que[i];
    } else {
        cout << "Enter size: ";
        in >> q.capacity;
        cout << "Enter data: " << endl;
        q.que = new U[q.capacity+1];
        U data;
        while(!q.full() && in >> data)
            q << data;
    }
    return in;
}

#endif // Queue_H

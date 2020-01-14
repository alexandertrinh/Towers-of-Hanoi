#ifndef QUEUE_H
#define QUEUE_H
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <cstring>
#include <sstream>
#include "disk.h"

enum QUEUE_ERRORS { EMPTY_QUEUE, FULL_QUEUE, BAD_SIZE_QUEUE };

using namespace std;

template<typename T>
class Queue {

    public:
        Queue(size_t s = 10);
        ~Queue();
        Queue(const Queue<T> &other);
        Queue<T>& operator=(const Queue<T> &other);

        T peek();
        bool full() const;
        bool empty() const;
        void clear();
        size_t size() const;
        size_t getCapacity() const;
        void resize(size_t s = 10);
        void enqueue(const T &d);
        T dequeue();
        std::string getName();
        void assignQueueName();
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

        T *que;
        int *que2;
        size_t head, tail, mySize, capacity;
        std::string name;

};


template<typename T>
Queue<T>::Queue(size_t s) {
    assignQueueName();
    if(s + 1 < 3)
        throw BAD_SIZE_QUEUE;
    capacity = s;
    que = new T[capacity+ 1];
    que2 = new int[capacity+ 1];
    //for(size_t i=0; i<capacity+1; ++i)
    //    que[i] = T();
    mySize = head = tail = 0;
}

template<typename T>
Queue<T>::~Queue() {
    nukem();
    capacity = mySize = head = tail = 0;
}

template<>
Queue<char*>::~Queue() {
    for(size_t i = 0; i < capacity+1; ++i)
        if(que[i] && que[i][0])
            delete[] que[i];
    delete [] que;
    que = NULL;
    capacity = mySize = head = tail = 0;
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

template<typename T>
T Queue<T>::peek() {
    return que[head];
}

template<typename T>
bool Queue<T>::full() const {
    return (mySize == capacity);
}

template<typename T>
bool Queue<T>::empty() const {
    return !mySize;
}

template<typename T>
void Queue<T>::clear() {
    tail = head = mySize = 0;
}

template<typename T>
size_t Queue<T>::size() const {
    return mySize;
}

template<typename T>
size_t Queue<T>::getCapacity() const {
    return capacity;
}

template<typename T>
void Queue<T>::resize(size_t s) {
    bool over = (s >= capacity);
    size_t j = 0, endingCondition = over ? head + mySize : s + head;
    T *temp = new T[s + 1];
    for(size_t i = head; i < endingCondition; ++i, ++j) {
        temp[j] = que[i%(capacity+1)];
        que[i%(capacity+1)] = T();
    }
    delete [] que;
    que = temp;
    //mySize = tail = over ? mySize : s;
    head = 0;
    capacity = s;
}

template<>
void Queue<char*>::resize(size_t s) {
    bool over = (s >= capacity);
    size_t j = 0, endingCondition = over ? head + mySize : s + head;
    char **temp = new char*[s + 1];
    for(size_t i=0; i<s+1; ++i)
        temp[i] = nullptr;
    for(size_t i = head; i < endingCondition; ++i, ++j) {
        size_t len = strlen(que[i%(capacity+1)]);
        temp[j] = new char[len];
        strncpy(temp[j], que[i%(capacity+1)], len);
    }
    for(size_t i = 0; i < capacity+1; ++i)
        if(que[i] && que[i][0])
            delete [] que[i];
    delete [] que;
    que = temp;
    mySize = tail = over ? mySize : s;
    capacity = s;
}

template<typename T>
void Queue<T>::enqueue(const T &d) {
    if(full())
        throw FULL_QUEUE;
    que[tail] = d;
    tail = (tail+1) % (capacity+1);
    ++mySize;
}

template<typename T>
T Queue<T>::dequeue() {
    if(empty())
        throw EMPTY_QUEUE;
    //d = que[head];
    T temp = que[head];
    //head = (head+1) % (capacity+1);
    --head;
    --mySize;
    return temp;
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
    for(size_t i = 0; i < capacity+1; ++i)
        que[i] = T();
    delete [] que;
    que = NULL;
}

template<typename T>
void Queue<T>::copy(const Queue<T> &other) {
    mySize = other.mySize;
    capacity = other.capacity;
    head = other.head;
    tail = other.tail;
    que = new T[capacity + 1];
    for(size_t i =  0; i < capacity + 1; ++i)
        que[i] = other.que[i];
}

template<>
void Queue<char*>::copy(const Queue<char*> &other) {
    mySize = other.mySize;
    capacity = other.capacity;
    head = other.head;
    tail = other.tail;
    que = new char*[capacity + 1];
    for(size_t i =  0; i < capacity + 1; ++i) {
        que[i] = new char[strlen(other.que[i])];
        strncpy(que[i], other.que[i], strlen(other.que[i]));
    }
}

template<typename  U>
ostream& operator<<(ostream& out, const Queue<U> &q) {
    if(&out != &cout) {
        out << "Queue capacity: " << q.capacity << endl
            << "Queue size: " << q.mySize << endl;
    }
    for(size_t i=q.head; i<q.mySize+q.head; ++i)
        out << q.que[i%(q.capacity+1)] << endl;
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

template<>
istream& operator>>(istream& in, Queue<string> &q) {
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
        in.ignore(32767, '\n');
        q.que = new string[q.capacity+1];
        for(size_t i=0; i<q.mySize; ++i, ++q.tail)
            getline(in, q.que[i]);
    } else {
        cout << "Enter size: ";
        in >> q.capacity;
        cout << "Enter data: " << endl;
        in.ignore(32767, '\n');
        q.que = new string[q.capacity+1];
        string data;
        while(!q.full()) {
            getline(in, data);
            if(data.empty())
                break;
            q << data;
        }
    }
    return in;
}

template<>
istream& operator>>(istream& in, Queue<char*> &q) {
    string line;
    for(size_t i = 0; i < q.capacity+1; ++i)
        if(q.que[i] && q.que[i][0])
            delete[] q.que[i];
    delete [] q.que;
    q.que = NULL;
    q.capacity = q.mySize = q.head = q.tail = 0;
    if(&in != &cin) {
        if(in.peek() == '\n')
            in.ignore(32767, '\n');
        getline(in, line, ':');
        in >> q.capacity;
        in.ignore(32767, '\n');
        getline(in, line, ':');
        in >> q.mySize;
        in.ignore(32767, '\n');
        q.que = new char*[q.capacity+1];
        for(size_t i=0; i<q.mySize; ++i, ++q.tail) {
            getline(in, line);
            char* data = new char[line.length()];
            strcpy(data, line.c_str());
            q.que[i] = data;
        }
    } else {
        cout << "Enter size: ";
        in >> q.capacity;
        cout << "Enter data: " << endl;
        in.ignore(32767, '\n');
        q.que = new char*[q.capacity+1];
        while(!q.full()) {
            getline(in, line);
            if(line.empty())
                break;
            char* data = new char[line.length()];
            strcpy(data, line.c_str());
            q << data;
        }
    }
    return in;
}

template<typename T>
void Queue<T>::assignQueueName() {
    static int i = 1;
    std::ostringstream s;
    s << "Queue " << i;
    std::string query(s.str());
    name = query;
    i++;
}

template<typename T>
std::string Queue<T>::getName() {
    return name;
}

#endif // Queue_H

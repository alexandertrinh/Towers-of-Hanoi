#ifndef STACK_H
#define STACK_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "node.h"

enum STACK_ERRORS { FULL, EMPTY, BAD_SIZE };

using namespace std;

template<typename T>
class Stack {
   public:
      Stack(int s = 1000);
      ~Stack();
      Stack(const Stack<T> &other);
      Stack<T> operator=(const Stack<T> &other);

      void push(const T &data);
      T pop();
      T peek() const;
      bool full() const;
      bool empty() const;
      int size() const;
      int capacity() const;
      void resize(int s);
      string getStackName(); //returns the given number of the stack

      Stack<T>& operator<<(const T &data);
      Stack<T>& operator>>(T &data);

      template<typename U>
      friend
      ostream& operator<<(ostream &out, const Stack<U> &stck);

      template<typename U>
      friend
      istream& operator>>(istream &in, Stack<U> &stck);

   private:
      node<T> *tos; //top of stack
      int mySize, cap; //mySize is how many Nodes inside stack, capacity is how much it can hold
      string name;

      void copy(const Stack<T> &other);
      void nukem(node<T> *top);
      void assignStackName(); //at initilization stack is given a specfic string name
};


template<typename T>
Stack<T>::Stack(int s) {
    assignStackName();
    if((cap = s) < 1)
        throw BAD_SIZE;
    tos = nullptr;
    mySize = 0;
}

template<typename T>
Stack<T>::~Stack() {
    nukem(tos);
}

template<typename T>
Stack<T>::Stack(const Stack<T> &other) {
    copy(other);
}

template<typename T>
Stack<T> Stack<T>::operator=(const Stack<T> &other) {
    if(this != &other) {
        nukem(tos);
        copy(other);
    }
    return *this;
}

template<typename T>
void Stack<T>::push(const T &data) {
    if(full())
        throw FULL;
    node<T> *newNode = new node<T>(data);
    newNode->next = tos;
    tos = newNode;
    ++mySize;
}

template<typename T>
T Stack<T>::pop() {
    if(empty())
        throw EMPTY;
    T data = tos->data;
    node<T> *bye = tos;
    tos = tos->next;
    delete bye;
    --mySize;
    return data;
}

template<typename T>
T Stack<T>::peek() const {
    if(empty())
        throw EMPTY;
    return tos->data;
}

template<typename T>
bool Stack<T>::full() const {
    return (mySize == cap);
}

template<typename T>
bool Stack<T>::empty() const {
    return !tos;
}

template<typename T>
int Stack<T>::size() const {
    return mySize;
}

template<typename T>
int Stack<T>::capacity() const {
    return cap;
}

template<typename T>
void Stack<T>::resize(int s) {
    if(s < 1)
        throw BAD_SIZE;
    if(s < cap) {
        node<T> *temp;
        for(int i=0; i<(cap - s); i++) {
            temp = tos;
            if (tos != nullptr)
                tos = tos->next; //no tos
            delete temp;
        }
    }
    cap = s;
}

template<>
void Stack<char*>::resize(int s) {
    if(s < 1)
        throw BAD_SIZE;
    if(s < cap) {
        node<char*> *temp;
        for(int i=0; i<cap - s; ++i) {
            temp = tos;
            tos = tos->next;
            if(temp->data)
                delete[] temp->data;
            delete temp;
        }
    }
    cap = s;
}

template<typename T>
Stack<T>& Stack<T>::operator<<(const T &data) {
    push(data);
    return *this;
}

template<typename T>
Stack<T>& Stack<T>::operator>>(T &data) {
    data = pop();
    return *this;
}

template<typename T>
void Stack<T>::copy(const Stack<T> &other) {
    node<T> *me, *you;
    nukem(tos);
    tos = nullptr;
    mySize = other.mySize;
    cap = other.cap;
    if(other.empty())
        return;
    tos = new node<T>(other.tos->data);
    me = tos, you = other.tos;
    while(you)
    {
        me->next = new node<T>(you->next->data);
        me = me->next;
        you = you->next;
    }
}

template<typename T>
void Stack<T>::nukem(node<T> *top) {
    if(!top)
        return;
    for(node<T> *bye = top; bye; bye = top) {
        top = top->next;
        delete bye;
    }
}

template<>
void Stack<char*>::nukem(node<char*> *top) {
    if(!top)
        return;
    for(node<char*> *bye = top; bye; bye = top) {
        top = top->next;
        if(bye->data)
            delete[] bye->data;
        delete bye;
    }
}

template<typename U>
ostream& operator<<(ostream &out, const Stack<U> &stck) {
    node<U> *tos = stck.tos;
    if(&out != &cout) {
        out<<"Stack size: "<<stck.mySize<<endl
           <<"Stack cap: "<<stck.cap<<endl;
    }
    while(tos) {
        out<<tos->data<<endl;
        tos = tos->next;
    }
    return out;
}

template<typename U>
istream& operator>>(istream &in, Stack<U> &stck) {
    string line;
    U data;
    stck.nukem(stck.tos);
    if(&in == &cin) {
        cout<<"Stack size: ";
        in>>stck.cap;
        cout<<"Enter data: "<<endl;
        while(!stck.full() && in>>data)
            stck<<data;
    }
    else {
        Stack<U> temp;
        getline(in, line, ':');
        in>>stck.mySize;
        getline(in, line, ':');
        in>>stck.cap;
        for(unsigned int i = 0; i < stck.mySize; ++i) {
            in>>data;
            temp<<data;
        }
        for(unsigned int i = 0; i < stck.mySize; ++i)
            stck<<temp.pop();
    }
    return in;
}

template<>
istream& operator>>(istream &in, Stack<string> &stck) {
    string line;
    string data;
    stck.nukem(stck.tos);
    if(&in == &cin) {
        cout<<"Stack size: ";
        in>>stck.cap;
        cout<<"Enter data: "<<endl;
        in.ignore(32767, '\n');
        while(!stck.full()) {
            getline(in, data);
            if(data.empty())
                break;
            stck<<data;
        }
    }
    else {
        Stack<string> temp;
        getline(in, line, ':');
        in>>stck.mySize;
        getline(in, line, ':');
        in>>stck.cap;
        for(unsigned int i = 0; i < stck.mySize; ++i) {
            in>>data;
            temp<<data;
        }
        for(unsigned int i = 0; i < stck.mySize; ++i)
            stck<<temp.pop();
    }
    return in;
}

template<>
istream& operator>>(istream &in, Stack<char*> &stck) {
    string line;
    stck.nukem(stck.tos);
    if(&in == &cin) {
        cout<<"Stack size: ";
        in>>stck.cap;
        cout<<"Enter data: "<<endl;
        in.ignore(32767, '\n');
        while(!stck.full()) {
            getline(in, line);
            if(line.empty())
                break;
            char* data = new char[line.length()];
            strcpy(data, line.c_str());
            stck<<data;
        }
    }
    else {
        Stack<char*> temp;
        getline(in, line, ':');
        in>>stck.mySize;
        getline(in, line, ':');
        in>>stck.cap;
        for(unsigned int i = 0; i < stck.mySize; ++i) {
            in.ignore(32767, '\n');
            getline(in, line);
            char* data = new char[line.length()];
            strcpy(data, line.c_str());
            temp<<data;
        }
        for(unsigned int i = 0; i < stck.mySize; ++i)
            stck<<temp.pop();
    }
    return in;
}

template<typename T>
void Stack<T>::assignStackName() {
    static int i = 1;
    std::ostringstream s;
    s << "Stack " << i;
    std::string query(s.str());
    name = query;
    i++;
}

template<typename T>
std::string Stack<T>::getName() {
    return name;
}

#endif // STACK_H

#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <math.h>
#include <chrono>
#include "stack.h"
#include "queue.h"
#include "disk.h"

using namespace std::chrono;
using namespace std;
/* This class Board creates three rods that can hold Disks.
 * This class starts with peg1 and ends with peg2.
 */

class Board {
    public:

        Board(int disks, char stackOrQueue);
        ~Board();

        void stackOutput(Disk d, Stack<Disk> &stack1, Stack<Disk> &stack2) const;
        void queueOutput(Disk d, Queue<Disk> &queue1, Queue<Disk> &queue2) const;

    private:
        void makeStack(); //route Stack
        void makeQueue(); //route Queue
        void makeDisks1(); //fills stack1 with all the disks
        void makeDisks2(); //fills queue1 with all the disks
        void stackAlgorithm(); //applies the algorithm and solves the game
        void queueAlgorithm(); //applies the algorithm and solves the game
        void moveS1toS2(); //find legal move between s1 and s2
        void moveS1toS3(); //find legal move between s1 and s3
        void moveS2toS3(); //find legal move between s2 and s3
        void moveQ1toQ2(); //find legal move between q1 and q2
        void moveQ1toQ3(); //find legal move between q1 and q3
        void moveQ2toQ3(); //find legal move between q2 and q3

        int diskCount;
        Stack<Disk> s1, s2, s3;
        Queue<Disk> q1, q2, q3, temp;
        Disk tempDisk;
};

Board::Board(int disks, char stackOrQueue) {
    this->diskCount = disks;

    if (stackOrQueue == 's' || stackOrQueue == 'S')
        makeStack();
    else if (stackOrQueue == 'q' || stackOrQueue == 'Q')
        makeQueue();
}

Board::~Board() {

}

void Board::makeStack() {
    auto start = high_resolution_clock::now();
    s1.resize(diskCount); //create objects and pass capacity
    s2.resize(diskCount - 1);
    s3.resize(diskCount);

    makeDisks1();
    stackAlgorithm();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << duration.count() << " ms" << endl;
}

void Board::makeDisks1() {
    Stack<Disk> temp;
    for (unsigned int i = 1; i <= diskCount; i++) { //push into temp stack
        Disk *d = new Disk(i);
        temp.push(*d);
    }

    for (unsigned int i = 1; i <= diskCount; i++) //push into stack1 for desired order from smallest to greatest disk size
        s1.push(temp.pop());
}

void Board::stackAlgorithm() {
    int numOfMoves = pow(2, diskCount);

    if (diskCount % 2 == 0) { //if stack starts with even disks
        for (unsigned int i = 1; i < numOfMoves; i++) {
            if (i % 3 == 1)
                moveS1toS2();
            if (i % 3 == 2)
                moveS1toS3();
            if (i % 3 == 0)
                moveS2toS3();
        }
    } else { //if stack starts with odd disks
        for (unsigned int i = 1; i < numOfMoves; i++) {
            if (i % 3 == 1)
                moveS1toS3();
            if (i % 3 == 2)
                moveS1toS2();
            if (i % 3 == 0)
                moveS2toS3();
        }
    }
}

void Board::moveS1toS2() {
    if (s1.size() == 0) {
        s1.push(s2.pop());
        stackOutput(s1.peek(), s2, s1);
    } else if (s2.size() == 0) {
        s2.push(s1.pop());
        stackOutput(s2.peek(), s1, s2);
    } else if ((s1.peek()).getDiskSize() > (s2.peek()).getDiskSize()) {
        s1.push(s2.pop());
        stackOutput(s1.peek(), s2, s1);
    } else {
        s2.push(s1.pop());
        stackOutput(s2.peek(), s1, s2);
    }
}

void Board::moveS1toS3() {
    if (s1.size() == 0) {
        s1.push(s3.pop());
        stackOutput(s1.peek(), s3, s1);
    } else if (s3.size() == 0) {
        s3.push(s1.pop());
        stackOutput(s3.peek(), s1, s3);
    } else if ((s1.peek()).getDiskSize() > (s3.peek()).getDiskSize()) {
        s1.push(s3.pop());
        stackOutput(s1.peek(), s3, s1);
    } else {
        s3.push(s1.pop());
        stackOutput(s3.peek(), s1, s3);
    }
}

void Board::moveS2toS3() {
    if (s2.size() == 0) {
        s2.push(s3.pop());
        stackOutput(s2.peek(), s3, s2);
    } else if (s3.size() == 0) {
        s3.push(s2.pop());
        stackOutput(s3.peek(), s2, s3);
    } else if ((s2.peek()).getDiskSize() > (s3.peek()).getDiskSize()) {
        s2.push(s3.pop());
        stackOutput(s2.peek(), s3, s2);
    } else {
        s3.push(s2.pop());
        stackOutput(s3.peek(), s2, s3);
    }
}

void Board::stackOutput(Disk d, Stack<Disk> &stack1, Stack<Disk> &stack2) const{
    cout << "Disk " << d.getDiskSize() << ": " << stack1.getName() << " -> " << stack2.getName() << endl;
    d.deleteDisk();
}


//************************************ QUEUE ****************************************
void Board::makeQueue() {
    auto start = high_resolution_clock::now();
    size_t t = diskCount;
    q1.resize(t);
    q2.resize(t - 1);
    q3.resize(t);
    temp.resize(t);

    makeDisks2();
    queueAlgorithm();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << duration.count() << " ms" << endl;
}

void Board::makeDisks2() {
    for (unsigned int i = 1; i <= diskCount; i++) {
        Disk *d = new Disk(i);
        q1.enqueue(*d);
    }
}

void Board::queueAlgorithm() {
    int numOfMoves = pow(2, diskCount);

    if (diskCount % 2 == 0) { //if queue starts with even disks
        for (unsigned int i = 1; i < numOfMoves; i++) {
            if (i % 3 == 1)
                moveQ1toQ2();
            if (i % 3 == 2)
                moveQ1toQ3();
            if (i % 3 == 0)
                moveQ2toQ3();
        }
    } else { //if queue starts with odd disks
        for (unsigned int i = 1; i < numOfMoves; i++) {
            if (i % 3 == 1)
                moveQ1toQ3();
            if (i % 3 == 2)
                moveQ1toQ2();
            if (i % 3 == 0)
                moveQ2toQ3();
        }
    }
}

void Board::moveQ1toQ2() {
    if (q1.size() == 0) {
        while (q2.size() > 1) //pop all but 1 elements out of queue in temp queue
            temp.enqueue(q2.dequeue());
        q1.enqueue(q2.dequeue()); //pass last element in queue2 to queue1
        while (temp.size() > 0) //copy back temp to q2
            q2.enqueue(temp.dequeue());
        queueOutput(q1.peek(), q2, q1);
    } else if (q2.size() == 0) {
        while (q1.size() > 1)
            temp.enqueue(q1.dequeue());

        q2.enqueue(q1.dequeue());
        while (temp.size() > 0)
            q1.enqueue(temp.dequeue());
        queueOutput(q2.peek(), q1, q2);
    } else if ((q1.peek()).getDiskSize() > (q2.peek()).getDiskSize()) {
        while (q2.size() > 1)
            temp.enqueue(q2.dequeue());
        q1.enqueue(q2.dequeue());
        while (temp.size() > 0)
            q2.enqueue(temp.dequeue());
        queueOutput(q2.peek(), q2, q1);
    } else {
        while (q1.size() > 1)
            temp.enqueue(q1.dequeue());
        q2.enqueue(q1.dequeue());
        while (temp.size() > 0)
            q1.enqueue(temp.dequeue());
        queueOutput(q2.peek(), q1, q2);
    }
}

void Board::moveQ1toQ3() {
    if (q1.size() == 0) {
        while (q3.size() > 1)
            temp.enqueue(q3.dequeue());
        q1.enqueue(q3.dequeue());
        while (temp.size() > 0)
            q3.enqueue(temp.dequeue());
        queueOutput(tempDisk, q3, q1);
    } else if (q3.size() == 0) {
        while (q1.size() > 1)
            temp.enqueue(q1.dequeue());
        tempDisk = q1.peek();
        q3.enqueue(q1.dequeue());
        while (temp.size() > 0)
            q1.enqueue(temp.dequeue());
        queueOutput(tempDisk, q1, q3);
    } else if ((q1.peek()).getDiskSize() > (q3.peek()).getDiskSize()) {
        while (q3.size() > 1)
            temp.enqueue(q3.dequeue());
        tempDisk = q3.peek();
        q1.enqueue(q3.dequeue());
        while (temp.size() > 0)
            q3.enqueue(temp.dequeue());
        queueOutput(tempDisk, q3, q1);
    } else {
        while (q1.size() > 1)
            temp.enqueue(q1.dequeue());
        tempDisk = q1.peek();
        q3.enqueue(q1.dequeue());
        while (temp.size() > 0)
            q1.enqueue(temp.dequeue());
        queueOutput(tempDisk, q1, q3);
    }
}

void Board::moveQ2toQ3() {
    if (q2.size() == 0) {
        while (q3.size() > 1)
            temp.enqueue(q3.dequeue());
        tempDisk = q3.peek();
        q2.enqueue(q3.dequeue());
        while (temp.size() > 0)
            q3.enqueue(temp.dequeue());
        queueOutput(tempDisk, q3, q2);
    } else if (q3.size() == 0) {
        while (q2.size() > 1)
            temp.enqueue(q2.dequeue());
        tempDisk = q2.peek();
        q3.enqueue(q2.dequeue());
        while (temp.size() > 0)
            q2.enqueue(temp.dequeue());
        queueOutput(tempDisk, q2, q3);
    } else if ((q2.peek()).getDiskSize() > (q3.peek()).getDiskSize()) {
        while (q3.size() > 1)
            temp.enqueue(q3.dequeue());
        tempDisk = q3.peek();
        q2.enqueue(q3.dequeue());
        while (temp.size() > 0)
            q3.enqueue(temp.dequeue());
        queueOutput(tempDisk, q3, q1);
    } else {
        while (q2.size() > 1)
            temp.enqueue(q2.dequeue());
        tempDisk = q2.peek();
        q3.enqueue(q2.dequeue());
        while (temp.size() > 0)
            q2.enqueue(temp.dequeue());
        queueOutput(tempDisk, q2, q2);
    }
}

void Board::queueOutput(Disk d, Queue<Disk> &queue1, Queue<Disk> &queue2) const {
    cout << "Disk " << d.getDiskSize() << ": " << queue1.getName() << " -> " << queue2.getName() << endl;
    d.deleteDisk();
}


#endif // BOARD_H

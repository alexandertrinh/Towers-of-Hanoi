#ifndef DISK_H
#define DISK_H
#include <iostream>

using namespace std;

/*Create a class Disk, which will
create disks of the appropriate size. That is,
if a disk of size X has just been created, then
the next disk will be of size X + 1. This class must
also contain a interface function that returns size of the disk.
*/

class Interface {
public:
    Interface(){}
    virtual ~Interface(){}
    virtual unsigned int getDiskSize() = 0;
};

class Disk: public Interface {
    public:
        Disk();
        Disk(unsigned int &size);
        ~Disk();
        unsigned int getDiskSize();
        void deleteDisk();
        void operator=(const Disk &other);

    private:
        unsigned int diskSize;
};

Disk::Disk() {
    diskSize = NULL;
}

Disk::Disk(unsigned int &size) {
    diskSize = size;
}

Disk::~Disk() {
    diskSize = NULL;
}

void Disk::deleteDisk() {
    diskSize = NULL;
}

void Disk::operator=(const Disk &other) {
    unsigned int newSize = other.diskSize;
    this->diskSize = newSize;
}

unsigned int Disk::getDiskSize() {
    return diskSize;
}

#endif // DISK_H

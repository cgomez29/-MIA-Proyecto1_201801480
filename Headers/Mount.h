//
// Created by cgomez on 28/02/21.
//

#ifndef PROYECTO1_MOUNT_H
#define PROYECTO1_MOUNT_H
#include <iostream>

using namespace std;

class Mount {

private:
    Mount *next = NULL;
    string id;
    string path;
    string name;
    int diskNumber;

public:
    Mount();
    Mount(string _id, string _path, string _name);
    Mount* getNext();
    void setNext(Mount *next);
    const string &getId() const;
    void setId(const string &id);
    const string &getPath() const;
    void setPath(const string &path);
    const string &getName() const;
    void setName(const string &name);
    const int &getDiskNumber() const;
    void setDiskNumber(const int &diskNumber);


};

#endif //PROYECTO1_MOUNT_H

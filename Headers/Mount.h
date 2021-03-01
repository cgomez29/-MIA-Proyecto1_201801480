//
// Created by cgomez on 28/02/21.
//

#ifndef PROYECTO1_MOUNT_H
#define PROYECTO1_MOUNT_H
#include <iostream>

using namespace std;

class Mount {
private:
    Mount *next;
public:
    const string &getId() const;

    void setId(const string &id);

    const string &getPath() const;

    void setPath(const string &path);

    const string &getName() const;

    void setName(const string &name);

private:
    string id;
    string path;
    string name;


public:
    Mount();
    Mount(string _id, string _path, string _name);
    Mount* getNext();
    void setNext(Mount *next);
};

#endif //PROYECTO1_MOUNT_H

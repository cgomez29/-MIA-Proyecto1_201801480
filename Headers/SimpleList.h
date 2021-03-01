//
// Created by cgomez on 28/02/21.
//

#ifndef PROYECTO1_SIMPLELIST_H
#define PROYECTO1_SIMPLELIST_H

#include "Mount.h"

class SimpleList {
private:
    Mount *head;
    Mount *tail;
public:
    void add(string _id, string _path, string _name);
    void printList();
};


#endif //PROYECTO1_SIMPLELIST_H

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
    SimpleList();
    void add(string id, string path, string name);
    void printList();
    void unMount(string id);
};


#endif //PROYECTO1_SIMPLELIST_H

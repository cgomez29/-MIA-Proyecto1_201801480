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
    char alphabet[26] = {'A','B','C','D','E','F', 'G','H','I',
                         'J','K','L','M','N','O', 'P','Q','R',
                         'S','T','U','V','W','X', 'Y','Z'};
    SimpleList();
    int partitionNumber;
    static SimpleList* instance;
public:

    static SimpleList* getInstance();
    void add(string id, string path, string name);
    void printList();
    void unMount(string id);
    char checkLetterMount(string);
    int checkNumberMount(string);
    string existsMount(string);
    Mount* existsMountObject(string);
};


#endif //PROYECTO1_SIMPLELIST_H

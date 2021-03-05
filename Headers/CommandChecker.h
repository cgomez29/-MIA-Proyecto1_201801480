//
// Created by cgomez on 11/02/21.
//

#ifndef PROYECTO1_COMMANDCHECKER_H
#define PROYECTO1_COMMANDCHECKER_H

#include "Node.h"
#include <list>

class CommandChecker {
public:
    bool checkMKDISK(Node *root);
    bool checkRMDISK(Node *root);
    bool checkFDISK(Node *root);
    bool checkMOUNT(Node *root);
    bool checkUNMOUNT(Node *root);

    bool checkREP(Node *root);
};


#endif //PROYECTO1_COMMANDCHECKER_H

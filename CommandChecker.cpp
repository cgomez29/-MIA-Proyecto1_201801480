//
// Created by cgomez on 11/02/21.
//

#include "CommandChecker.h"

using namespace std;

bool CommandChecker::checkMKDISK(Node *root) {
    list<Node>:: iterator aux;
    aux = root->childs.begin()->childs.begin();

    int counter = 0;
    while (counter < root->childs.begin()->count) {
        if(aux->type == "SIZE") {
            cout << "size siiiiiii" << endl;
            return true;
        }

        counter++;
    }
    return false;
}
//
// Created by cgomez on 11/02/21.
//

#include "CommandChecker.h"

using namespace std;

bool CommandChecker::checkMKDISK(Node *root) {
    list<Node>:: iterator aux;
    aux = root->childs.begin()->childs.begin();
    int counter  = 0, valueInt = 0;
    bool flag = true, flagSize = false, flagPath = false;
    while (counter < root->childs.begin()->count) {
        if(aux->type == "SIZE") {
            for (char i: aux->value) {
                valueInt = valueInt + i;
            }
            if(valueInt == 48) {
                flag = false;
                cout << "SIZE solo acepta nùmeros positvos mayores que cero!" << endl;
                break;
            }
            flagSize = true;
        } else if (aux->type == "PATH") {
            flagPath = true;
        }
        aux++;
        counter++;
    }
    // checker require parameters size and path
    if(!flagSize ) {
        cout << "Parámetro \"size\" es requerido!" << endl;
        flag = false;
    } else if(!flagPath) {
        cout << "Parámetro \"path\" es requerido!" << endl;
        flag = false;
    }
    return flag;
}

bool CommandChecker::checkRMDISK(Node *root) {
    bool flag = false;
    if(root->type == "RMDISK") {
        flag = true;
    }
    return flag;
}

bool CommandChecker::checkFDISK(Node *root) {
    list<Node> :: iterator aux;
    aux = root->childs.begin()->childs.begin();
    int counter = 0;
    bool flag = true, flagPath = false, flagName = false, flagSize = false,
            flagAdd = false;

    while(counter < root->childs.begin()->count) {
        if(aux->type == "PATH") {
            flagPath = true;
        } else if(aux->type == "NAME") {
            flagName = true;
        } else if (aux->type == "SIZE") {
            flagSize = true;
        } else if (aux->type == "ADD") {
            flagAdd = true;
        }
        aux++;
        counter++;
    }
    if(!flagPath) {
        flag = false;
    }
    if(!flagName) {
        flag = false;
    }
    if(!flagAdd) {
        flag = false;
    }
    // Command add required size
    if(flagAdd) {
        if(!flagSize) {
            flag = false;
        }
    }
    return flag;
}

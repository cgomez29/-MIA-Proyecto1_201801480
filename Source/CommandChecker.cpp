//
// Created by cgomez on 11/02/21.
//

#include "../Headers/CommandChecker.h"

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
            for (char i: aux->value) {
                if(i < '0' || i > '9'){
                    cout << "SIZE solo acepta nùmeros positvos mayores que cero!" << endl;
                    return false;
                }
            }
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

    // Command add required size
    if(flagAdd) {
        if(!flagSize) {
            flag = false;
        }
    }
    return flag;
}

bool CommandChecker::checkMOUNT(Node *root) {
    list<Node> :: iterator aux;
    aux = root->childs.begin()->childs.begin();
    bool flag = true, flagPath = false, flagName = false;
    int counter = 0;
    while(counter < root->childs.begin()->count) {
        if(aux->type == "PATH"){
            flagPath = true;
        } else if (aux->type == "NAME") {
            flagName = true;
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
    return flag;
}

bool CommandChecker::checkUNMOUNT(Node *root) {
    bool flag = false;
    if(root->type == "UNMOUNT") {
        flag = true;
    }
    return flag;
}

bool CommandChecker::checkREP(Node *root) {
    list<Node> :: iterator aux;
    aux = root->childs.begin()->childs.begin();
    bool flag = false;
    int counter = 0;
    while(counter < root->childs.begin()->count) {
        if(aux->type == "NAME"){
            if(aux->value == "MBR" || aux->value == "mbr"){
                flag= true;
                break;
            } else if(aux->value == "DISK" || aux->value == "disk"){
                flag= true;
                break;
            } else if(aux->value == "INODE" || aux->value == "inode"){
                flag= true;
                break;
            } else if(aux->value == "Journaling" || aux->value == "journaling"){
                flag= true;
                break;
            } else if(aux->value == "BLOCK" || aux->value == "block"){
                flag= true;
                break;
            } else if(aux->value == "bm_inode" || aux->value == "BM_INODE"){
                flag= true;
                break;
            } else if(aux->value == "bm_block" || aux->value == "BM_BLOCK"){
                flag= true;
                break;
            } else if(aux->value == "tree" || aux->value == "TREE"){
                flag= true;
                break;
            } else if(aux->value == "sb" || aux->value == "SB"){
                flag= true;
                break;
            } else if(aux->value == "file" || aux->value == "FILE"){
                flag= true;
                break;
            } else if(aux->value == "ls" || aux->value == "ls"){
                flag= true;
                break;
            }
        }
        aux++;
        counter++;
    }
    return flag;
}

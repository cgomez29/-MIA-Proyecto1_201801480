//
// Created by cgomez on 28/02/21.
//

#include "../Headers/SimpleList.h"

void SimpleList::add(string _id, string _path, string _name) {
    Mount* newMount = new Mount(_id, _path, _name);
    if(this->head == nullptr) {
        this->head = newMount;
        this->tail = newMount;
    } else {
        this->tail->setNext(newMount);
    }
}

void SimpleList::printList() {
    Mount *aux = this->head;
    while(aux->getNext() != nullptr) {
        cout << aux->getName() << endl;
        aux = aux->getNext();
    }
}


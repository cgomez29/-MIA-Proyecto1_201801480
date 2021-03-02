//
// Created by cgomez on 28/02/21.
//

#include "../Headers/SimpleList.h"

SimpleList::SimpleList() {
    this->head = nullptr;
    this->tail = nullptr;
}

void SimpleList::add(string id, string path, string name) {
    Mount* newMount = new Mount(id, path, name);
    if(this->head == nullptr) {
        this->head = newMount;
        this->tail = newMount;
    } else {
        this->tail->setNext(newMount);
        this->tail = newMount;
    }
}

void SimpleList::printList() {
    Mount *aux = this->head;
    while(aux != nullptr) {
        cout << aux->getName() << endl;
        aux = aux->getNext();
    }
}

void SimpleList::unMount(string id) {
    Mount *aux = this->head;
    Mount *ant;

    if(aux != nullptr) {
        if(aux->getId() == id) {
            if(aux->getNext() != nullptr) {
                this->head = aux->getNext();
            } else {
                this->head = nullptr;
            }
            return;
        }
    }

    while(aux != nullptr) {
        if(aux->getId() == id){
            ant->setNext(aux->getNext());
        }
        ant = aux;
        aux = aux->getNext();
    }
}



//
// Created by cgomez on 28/02/21.
//

#include "../Headers/SimpleList.h"

SimpleList::SimpleList() {
    this->head = NULL;
    this->tail = NULL;
    this->partitionNumber = 0;
}

void SimpleList::add(string id, string path, string name) {
    int numberDisk = checkNumberMount(path);
    id = "80" + to_string(numberDisk) + checkLetterMount(path);

    Mount* newMount = new Mount(id, path, name);
    newMount->setDiskNumber(numberDisk);

    if(this->head == NULL) {
        this->head = newMount;
        this->tail = newMount;
    } else {
        this->tail->setNext(newMount);
        this->tail = newMount;
    }
    //TODO
    printList();
}

void SimpleList::printList() {
    Mount *aux = this->head;
    cout << "----------PARTICIONES MONTADAS-----------" << endl;

    while(aux != NULL) {
        cout << "ID: " << aux->getId() << endl;
        cout << "Partición: " << aux->getName() << endl;
        aux = aux->getNext();
    }
    cout << "----------------------" << endl;
}

void SimpleList::unMount(string id) {
    Mount *aux = this->head;
    Mount *ant;

    if(aux != NULL) {
        if(aux->getId() == id) {
            if(aux->getNext() != NULL) {
                this->head = aux->getNext();
            } else {
                this->head = NULL;
            }
            cout << "\nPartición desmontada!\n" << endl;
            return;
        }
    }
    bool flag = true;
    while(aux != NULL) {
        if(aux->getId() == id){
            ant->setNext(aux->getNext());
            flag = false;
            cout << "\nPartición desmontada!\n" << endl;
            break;
        }
        ant = aux;
        aux = aux->getNext();
    }
    if(flag)
        cout << "\nERROR: La partición: "<< id << " No se encuentra montada!\n" << endl;
}

char SimpleList::checkLetterMount(string path) {
    Mount *aux = this->head;
    bool flag = true;
    while(aux != NULL){
        if(aux->getPath() == path) {
            flag = false;
            int letter = aux->getId().length()-1;
            for (int i = 0; i < 26; ++i) {
                if(alphabet[i] == aux->getId()[letter]) {
                    if(i+1 != 26){
                        return alphabet[i+1];
                    }
                }
            }

        }
        aux = aux->getNext();
    }
    /**
     * Si del disco no se ha recibido niguna particion se asgina la primera letra
     * */
    if(flag) {
        return alphabet[0];
    }
    return 0;
}

int SimpleList::checkNumberMount(string path) {
    Mount *aux = this->head;
    /*Different disk*/
    bool flag = true;
    while(aux != NULL){
        if(aux->getPath() == path) {
            flag = false;
            return aux->getDiskNumber();

        }
        aux = aux->getNext();
    }
    if(flag){
        partitionNumber++;
        return partitionNumber;
    }
}

bool SimpleList::existsMount(string id) {
    Mount* aux = this->head;
    while (aux != NULL){
        if(aux->getId() == id){
            return true;
        }
        aux = aux->getNext();
    }
    return false;
}




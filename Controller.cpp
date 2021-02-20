//
// Created by cgomez on 20/02/21.
//

#include "Controller.h"

void Controller::run() {
    string input;
    while(true) {
        cout << "Ingrese un comando:" << endl;
        cout << ">>" ;
        getline(cin, input);
        readCommand(input);
    }
}

void Controller::readCommand(string input) {
    if(input[0] != '#' && input.size() > 2){
        YY_BUFFER_STATE bufferState = yy_scan_string(input.c_str());
        if(yyparse()==0){
            command(root);
        }
        yy_delete_buffer(bufferState);
    }
}

void Controller::command(Node *root) {
    if(root->type == "MKDISK") {
        if(commandChecker->checkMKDISK(root)) {
            makeMKDISK(root);
            return;
        }
    }
    cout << "Comando no valido" << endl;
}

void Controller::makeMKDISK(Node *root) {
    list<Node>:: iterator aux;
    aux = root->childs.begin()->childs.begin();
    int counter=0;
    struct MKDISK disk;
    while (counter < root->childs.begin()->count) {
        if (aux->type == "SIZE") {
            disk.size = stoi(aux->value);
        }
        else if (aux->type == "PATH") {
            disk.path = aux->value;
        }
        else if (aux->type == "U") {
            disk.u = aux->value;
        }
        else if (aux->type == "F") {
            disk.f = aux->value;
        }
        counter++;
        aux++;
    }
    executeMKDISK(disk);
}

//Metodo para crear el disco
void Controller::executeMKDISK(MKDISK disk) {
    FILE *file;
    string path = disk.path;
    //Verificando si ya existe
    char ruta[path.size()+1];
    strcpy(ruta, path.c_str());
    file = fopen(ruta, "r");
    if(file != NULL) {
        msj("El disco ya existe");
        return;
    }
    file = fopen(ruta, "wb");
    fwrite("\0", 1, 1, file);

    int tamano;

    if(disk.u == "k") {
        tamano = disk.size * 1024;
    } else {
        tamano = disk.size * 1024 * 1024;
    }

    //FALTA F

    fseek(file, tamano, SEEK_SET);
    fwrite("\0",1,1,file);
    fclose(file);
    msj("Disco creado exitosamente!");
}

void Controller::msj(string mensaje) {
    cout << endl;
    cout << mensaje << endl;
    cout << endl;
}



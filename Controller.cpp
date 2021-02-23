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

void Controller::msj(string mensaje) {
    cout << endl;
    cout << mensaje << endl;
    cout << endl;
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
    if(root->type == "MKDISK"){
        if(commandChecker->checkMKDISK(root)) {
            makeMKDISK(root);
            return;
        }
    } else if(root->type == "RMDISK") {
        if(commandChecker->checkRMDISK(root)) {
            makeRMDISK(root);
            return;
        }
    } else if(root->type == "FDISK") {
        if(commandChecker->checkFDISK(root)) {
            makeFDISK(root);
            return;
        }
    } else if(root->type == "MOUNT") {
        if(commandChecker->checkMOUNT(root)) {
            //TODO
            return;
        }
    } else if(root->type == "UNMOUNT") {
        if(commandChecker->checkUNMOUNT(root)) {
            //TODO
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
    int tamano =0;

    if(disk.u == "k") {
        tamano = disk.size * 1024;
    } else {
        tamano = disk.size * 1024 * 1024;
    }

    //FALTA F

    MBR mbr;
    mbr.mbr_tamano = tamano;
    mbr.mbr_disk_signature = rand()%1000;
    mbr.mbr_fecha_creacion = time(0);
    for (int i = 0; i < 4; ++i) {
        mbr.mbr_partition[i].part_status = '0';
        mbr.mbr_partition[i].part_size = 0;
        mbr.mbr_partition[i].part_type = 'p';
        mbr.mbr_partition[i].part_fit = 'w';
        mbr.mbr_partition[i].part_start = 0;
        strcpy(mbr.mbr_partition[i].part_name, "");
    }

    cout<<"Disco\nFecha de creacion: "<<asctime(gmtime(&mbr.mbr_fecha_creacion))<<endl;

    char test[1024];

    file = fopen(ruta, "wb");
    for(int i=0; i < 4; i++){
        fwrite(&test, sizeof (test), 1, file);
    }
    fseek(file, 0, SEEK_SET);
    fwrite(&mbr, sizeof (MBR), 1, file);
    fclose(file);



    msj("Disco creado exitosamente!");
}

void Controller::makeRMDISK(Node *root) {
    string path = root->value;
    executeRMDISK(path);
}

void Controller::executeRMDISK(string path) {
    char ruta[path.size() + 1];
    strcpy(ruta, path.c_str());
    string input;
    while (true) {
        msj("Esta seguro que desea eliminar el Disco? (Y/N)");
        cin >> input;
        if (input == "y" || input == "Y") {
            if (remove(ruta) != 0) {
                msj("Error al borrar el disco!");
            } else {
                msj("Disco eliminado exitosamente!");
            }
            break;
        } else {
            if (input == "n" || input == "N") {
                break;
            }
        }
    }
}

void Controller::makeFDISK(Node *root) {
    list<Node> :: iterator aux;
    aux = root->childs.begin()->childs.begin();
    int counter = 0;
    Partition partition;
    string path;
    while(counter < root->childs.begin()->count) {
        if(aux->type == "PATH") {
            path = aux->value;
        } else if(aux->type == "NAME") {
            strcpy(partition.part_name, aux->value.c_str());
        } else if (aux->type == "SIZE") {
            partition.part_size = stoi(aux->value);
        } else if (aux->type == "ADD") {

        } else if (aux->type == "U") {
            if(aux->value == "b") {
                partition.part_unit = 'b';
            } else if(aux->value == "m") {
                partition.part_unit = 'm';
            } else {
                //default k
                partition.part_unit = 'k';
            }
        } else if (aux->type == "F") {
            if(aux->value == "bf") {
                partition.part_fit = 'b';
            } else if(aux->value == "f") {
                partition.part_fit = 'f';
            } else {
                //default w
                partition.part_fit = 'w';
            }
        } else if (aux->type == "DELETE") {
            if(aux->value == "fast") {

            } else {
                // default full

            }
        } else if (aux->type == "TYPE") {
            if(aux->value == "e") {
                partition.part_fit = 'e';
            } else if(aux->value == "l") {
                partition.part_fit = 'l';
            } else {
                //default P
                partition.part_fit = 'p';
            }
        }
        aux++;
        counter++;
    }

    executeFDISK(partition, path);
}

void Controller::executeFDISK(Partition partition, string path) {
    FILE *file;
    file = fopen(path.c_str(), "rb+");

    if(file == NULL) {
        msj("El Disco no existe!");
        return;
    }
    MBR auxDisk;
    fseek(file,0,SEEK_SET);
    fread(&auxDisk, sizeof(MBR), 1, file);
    fclose(file);

    if(diskNotIsEmpty(auxDisk)){
        if(partition.part_type == 'p') {
            createPrimaryPartition(auxDisk, partition, path);
        } else if(partition.part_type == 'e') {
            createExtendPartition(auxDisk, partition, path);
        } else {
            createLogicPartition(auxDisk, partition, path);
        }
    } else {
        msj("Ya no se pueden realizar mas particiones!");
    }
}

bool Controller::diskIsEmpty(Controller::MBR mbr) {
    for (int i = 0; i < 4; ++i) {
        if(mbr.mbr_partition->part_status == '1') {
            return false;
        }
    }
    return true;
}

bool Controller::diskNotIsEmpty(Controller::MBR mbr) {
    for (int i = 0; i < 4; ++i) {
        if(mbr.mbr_partition->part_status != '1') {
            return false;
        }
    }
    return true;
}

void Controller::createPrimaryPartition(MBR mbr, Partition partition, string path) {
    if(diskIsEmpty(mbr)){
        partition.part_status = '1';
        partition.part_start = 136 + partition.part_size;
        mbr.mbr_partition[0] = partition;
    }

}

void Controller::createExtendPartition(MBR mbr, Controller::Partition part, string path) {

}

void Controller::createLogicPartition(MBR mbr, Controller::Partition part, string path) {

}







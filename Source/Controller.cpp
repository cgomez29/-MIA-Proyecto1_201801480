//
// Created by cgomez on 20/02/21.
//

#include "../Headers/Controller.h"

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

    if(disk.u == "m") {
        disk.size = disk.size * 1024;
    }

    //FALTA F

    MBR mbr;
    mbr.mbr_tamano = disk.size;
    mbr.mbr_disk_signature = rand()%1000;
    mbr.mbr_fecha_creacion = time(0);
    for (int i = 0; i < 4; ++i) {
        mbr.mbr_partition[i].part_status = '0';
        mbr.mbr_partition[i].part_size = 0;
        mbr.mbr_partition[i].part_type = 'p';
        mbr.mbr_partition[i].part_fit = 'w';
        mbr.mbr_partition[i].part_start = -1;
        strcpy(mbr.mbr_partition[i].part_name, "");
    }

    cout<<"Disco\nFecha de creacion: "<<asctime(gmtime(&mbr.mbr_fecha_creacion))<<endl;
    cout<<"Tamaño: "<< mbr.mbr_tamano << disk.u << "b" <<endl;

    char test[1024];

    file = fopen(ruta, "wb");
    for(int i=0; i < disk.size; i++){
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

    char type = 'p';
    char fit = 'w'; //wf
    int size;
    char name[16];
    char unit = 'k';
    string path;
    int add = 0;
    string cDelete;

    while(counter < root->childs.begin()->count) {
        if(aux->type == "PATH") {
            path = aux->value;
        } else if(aux->type == "NAME") {
            strcpy(name, aux->value.c_str());
        } else if (aux->type == "SIZE") {
            size = stoi(aux->value);
        } else if (aux->type == "ADD") {
            add = stoi(aux->value);
        } else if (aux->type == "U") {
            if(aux->value == "b") {
                unit = 'b';
            } else if(aux->value == "m") {
                unit = 'm';
            } else {
                //default k
                unit = 'k';
            }
        } else if (aux->type == "F") {
            if(aux->value == "bf") {
                fit = 'b';
            } else if(aux->value == "ff") {
                fit = 'f';
            } else {
                //default wf
                fit = 'w';
            }
        } else if (aux->type == "DELETE") {
            if(aux->value == "fast") {
                cDelete = "fast";
            } else {
                // default full
                cDelete = "full";
            }
        } else if (aux->type == "TYPE") {
            if(aux->value == "e") {
                fit = 'e';
            } else if(aux->value == "l") {
                fit = 'l';
            } else {
                //default P
                fit = 'p';
            }
        }
        aux++;
        counter++;
    }

    executeFDISK(path, type, fit, size, name, unit, add, cDelete);
}

void Controller::executeFDISK(string path, char type, char fit, int size, char name[16], char unit, int add, string cDelete) {
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

    if(type == 'p') {
        createPrimaryPartition(auxDisk, path, fit, size, name, unit);
    } else if(type == 'e') {
        createExtendPartition(auxDisk, path, fit, size, name, unit);
    } else if (type == 'l') {
        createLogicPartition(auxDisk, path, fit, size, name, unit);
    }

}

int Controller::firstFit(MBR mbr) {
    for (int i = 0; i < 4; ++i) {
        if(mbr.mbr_partition[i].part_start == -1) {
            return i;
        }
    }
    return -1;
}

int Controller::bestFit(MBR mbr) {
    int aux = 0;
    bool flag = false;
    for (int i = 0; i < 4; ++i) {
        if(mbr.mbr_partition[i].part_start == -1) {
            flag = true;
            if(aux != i && mbr.mbr_partition[aux].part_size > mbr.mbr_partition[i].part_size) {
                aux = i;
            }
        }
    }

    if(flag) {
        return aux;
    }
    return -1;
}

int Controller::worstFit(MBR mbr) {
    int aux = 0;
    bool flag = false;
    for (int i = 0; i < 4; ++i) {
        if(mbr.mbr_partition[i].part_start == -1) {
            flag = true;
            if(aux != i && mbr.mbr_partition[aux].part_size < mbr.mbr_partition[i].part_size) {
                aux = i;
            }
        }
    }
    if(flag) {
        return aux;
    }
    return -1;
}

void Controller::createPrimaryPartition(MBR mbr, string path, char fit, int size, char name[16], char unit) {
    FILE *file;

    file = fopen(path.c_str(), "rb+");

    if(file == NULL) {
        msj("El Disco no existe!");
        return;
    }

    int sizeUsed = 0;
    bool flagName = false;

    for (int i = 0; i < 4; ++i) {
        if(mbr.mbr_partition[i].part_status == '1') {
            sizeUsed = sizeUsed + mbr.mbr_partition[i].part_size;
        }
        if(strcmp(mbr.mbr_partition[i].part_name, name) == 0) {
            flagName = true;
            break;
        }
    }

    if(flagName) {
        msj("El nombre de la particion a crear ya existe!");
        return;
    }

    if((mbr.mbr_tamano - sizeUsed) >= size ) {
        int index = 0;
        if(fit == 'f') {
            index  = firstFit(mbr);
        } else if(fit == 'b') {
            index = bestFit(mbr);
        } else if(fit == 'w') {
            index = worstFit(mbr);
        }

        Partition partition;
        partition.part_status = '1';
        partition.part_fit = fit;
        partition.part_type = 'p';
        partition.part_size = size;
        strcpy(partition.part_name, name);
        if(index == 0) {
            partition.part_start = sizeof(MBR);
        } else {
            partition.part_start = mbr.mbr_partition[index-1].part_start + mbr.mbr_partition[index-1].part_size;
        }
        mbr.mbr_partition[index] = partition;

        fseek(file, 0, SEEK_SET);
        fwrite(&mbr,sizeof(MBR), 1, file);
        char test = '1';
        fseek(file, partition.part_start, SEEK_SET);
        for (int i = 0; i < partition.part_size; ++i) {
            fwrite(&test, 1, 1, file);
        }
        fclose(file);
        msj("Particiòn Creada exitosamente!");
    } else {
        msj("No hay espacio suficiente para crear la nueva particiòn!");
    }
}

void Controller::createExtendPartition(MBR mbr, string path, char fit, int size, char name[16], char unit) {
    FILE *file;

    file = fopen(path.c_str(), "rb+");

    if(file == NULL) {
        msj("El Disco no existe!");
        return;
    }

    int sizeUsed = 0;
    bool flagName = true;

    for (int i = 0; i < 4; ++i) {
        if(mbr.mbr_partition[i].part_status == '1') {
            sizeUsed = sizeUsed + mbr.mbr_partition[i].part_size;
        }
        if(strcmp(mbr.mbr_partition[i].part_name, name) == 0) {
            flagName = false;
            break;
        }
    }

    if(!flagName) {
        msj("El nombre de la particion a crear ya existe!");
        return;
    }

    if((mbr.mbr_tamano - sizeUsed) >= size ) {
        int index = 0;
        if(fit == 'f') {
            index  = firstFit(mbr);
        } else if(fit == 'b') {
            index = bestFit(mbr);
        } else if(fit == 'w') {
            index = worstFit(mbr);
        }
        /* PARTITION */
        Partition partition;
        partition.part_status = '1';
        partition.part_fit = fit;
        partition.part_type = 'p';
        partition.part_size = size;
        strcpy(partition.part_name, name);
        if(index == 0) {
            partition.part_start = sizeof(MBR);
        } else {
            partition.part_start = mbr.mbr_partition[index-1].part_start + mbr.mbr_partition[index-1].part_size;
        }
        mbr.mbr_partition[index] = partition;

        /* EBR */
        EBR ebr;
        ebr.part_status = '0';
        ebr.part_fit = fit;
        ebr.part_size = 0;
        ebr.part_next = -1;
        strcpy(ebr.part_name, name);
        ebr.part_start = mbr.mbr_partition[index].part_start;

        //setting partition
        mbr.mbr_partition[index] = partition;

        char test = '1';
        fseek(file, 0, SEEK_SET);
        fwrite(&mbr,sizeof(MBR), 1, file);

        fseek(file, ebr.part_start, SEEK_SET);
        fwrite(&ebr, sizeof(EBR), 1, file);

        for (int i = 0; i < (ebr.part_size - (int) sizeof(EBR)); ++i) {
            fwrite(&test, 1, 1, file);
        }
        fclose(file);
        msj("Particiòn Creada exitosamente!");
    } else {
        msj("No hay espacio suficiente para crear la nueva particiòn!");
    }
}

void Controller::createLogicPartition(MBR mbr, string path, char fit, int size, char name[16], char unit) {
    FILE *file;

    file = fopen(path.c_str(), "rb+");

    if(file == NULL) {
        msj("El Disco no existe!");
        return;
    }

    int index = 0;
    bool flag = false;
    for (int i = 0; i < 4; ++i) {
        if(mbr.mbr_partition[i].part_type == 'e'){
            index = i;
            flag = true;
        }
    }

    if(flag) {
        msj("No existe una particion extendida para poder crear una logica!");
        return;
    }

    EBR auxEBR;
    fseek(file, mbr.mbr_partition[index].part_start, SEEK_SET);
    fread(&auxEBR, sizeof(EBR),1, file);

    if(auxEBR.part_next == -1) {
        auxEBR.part_status = '1';
        auxEBR.part_fit = fit;
        auxEBR.part_start = mbr.mbr_partition[index].part_start;
        auxEBR.part_size = size;
        auxEBR.part_next =  mbr.mbr_partition[index].part_start + size;
        strcpy(auxEBR.part_name, name);

        char test = '1';
        fseek(file, auxEBR.part_start, SEEK_SET);
        fwrite(&auxEBR,sizeof(EBR), 1, file);
        for (int i = 0; i < (auxEBR.part_size - (int) sizeof(EBR)); ++i) {
            fwrite(&test, 1, 1, file);
        }
        fclose(file);
    } else {
        while(auxEBR.part_next != -1) {
            fseek(file, auxEBR.part_start + auxEBR.part_size, SEEK_SET);
            fread(&auxEBR, sizeof(EBR),1, file);
        }
        EBR ebr;
        ebr.part_status = '1';
        ebr.part_fit = fit;
        ebr.part_start = auxEBR.part_next;
        ebr.part_size = size;
        ebr.part_next = -1;
        strcpy(ebr.part_name, name);

        char test = '1';
        fseek(file, ebr.part_start, SEEK_SET);
        fwrite(&ebr,sizeof(EBR), 1, file);
        for (int i = 0; i < (ebr.part_size - (int) sizeof(EBR)); ++i) {
            fwrite(&test, 1, 1, file);
        }
        fclose(file);
    }
}







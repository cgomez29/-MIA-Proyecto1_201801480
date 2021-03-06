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
            makeMount(root);
            return;
        }
    } else if(root->type == "UNMOUNT") {
        if(commandChecker->checkUNMOUNT(root)) {
            makeUnMount(root);
            return;
        }
    } else if(root->type == "REP") {
        if(commandChecker->checkREP(root)) {
            executeREP();
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
        msj("El disco ya existe!");
        return;
    }

    if(disk.u == "m") {
        disk.size = disk.size * 1024 * 1024;
    } else {
        // kilobytes
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
    cout<<"Tamaño: "<< mbr.mbr_tamano << " bytes" <<endl;

    char test[1];

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
    string path = "";
    int add = 0;
    string cDelete = "";

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
                type = 'e';
            } else if(aux->value == "l") {
                type = 'l';
            } else {
                //default P
                type = 'p';
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

    // bytes
    if(unit == 'k') {
        size = size * 1024;
    } else if (unit == 'm') {
        size = size * 1024 * 1024;
    }

    if(type == 'p') {
        createPrimaryPartition(auxDisk, path, fit, size, name, unit);
    } else if(type == 'e') {
        createExtendPartition(auxDisk, path, fit, size, name, unit);
    } else if (type == 'l') {
        createLogicPartition(auxDisk, path, fit, size, name, unit);
    } else if (cDelete != "") {
        deletePartition(auxDisk, path, name, cDelete);
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

            return i;
            /*flag = true;
            if(aux != i && mbr.mbr_partition[aux].part_size > mbr.mbr_partition[i].part_size) {
                aux = i;
            }*/
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
            return i;
        } /*else {
            flag = true;
            if(i != aux && mbr.mbr_partition[aux].part_size < mbr.mbr_partition[i].part_size) {
                aux = i;
            }
        }*/
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
        fclose(file);
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
        msj("Particiòn Creada exitosamente!");
    } else {
        msj("No hay espacio suficiente para crear la nueva particiòn!");
    }
    fclose(file);
}

void Controller::createExtendPartition(MBR mbr, string path, char fit, int size, char name[16], char unit) {
    FILE *file;

    file = fopen(path.c_str(), "rb+");

    if(file == NULL) {
        msj("El Disco no existe!");
        return;
    }

    int sizeUsed = 0; /* Size used for partitions */
    bool flagName = true;
    bool existsEBR = false;
    for (int i = 0; i < 4; ++i) {
        if(mbr.mbr_partition[i].part_status == '1') {
            sizeUsed = sizeUsed + mbr.mbr_partition[i].part_size;
            if(mbr.mbr_partition[i].part_type == 'e'){
                existsEBR = true;
                break;
            }
        }
        if(strcmp(mbr.mbr_partition[i].part_name, name) == 0) {
            flagName = false;
            break;
        }
    }

    if(existsEBR) {
        msj("Ya existe una particiòn extendida en este disco!");
        fclose(file);
        return;
    }

    if(!flagName) {
        msj("El nombre de la particion a crear ya existe!");
        fclose(file);
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
        partition.part_type = 'e';
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
        msj("Particiòn Extendida Creada exitosamente!");
    } else {
        msj("No hay espacio suficiente para crear la nueva particiòn!");
    }
    fclose(file);
}

void Controller::createLogicPartition(MBR mbr, string path, char fit, int size, char name[16], char unit) {
    FILE *file;

    file = fopen(path.c_str(), "rb+");

    if(file == NULL) {
        msj("El Disco no existe!");
        return;
    }

    int index = -1;
    bool flag = true;
    for (int i = 0; i < 4; ++i) {
        if(mbr.mbr_partition[i].part_type == 'e'){
            index = i;
            flag = false;
            break;
        }
    }

    if(flag) {
        msj("No existe una particion extendida para poder crear una logica!");
        fclose(file);
        return;
    }

    // Checking space available
    if(mbr.mbr_partition[index].part_size >= size) {
        EBR auxEBR;
        fseek(file, mbr.mbr_partition[index].part_start, SEEK_SET);
        fread(&auxEBR, sizeof(EBR),1, file);

        if(auxEBR.part_next == -1) {
            auxEBR.part_status = '1';
            auxEBR.part_fit = fit;
            auxEBR.part_start = mbr.mbr_partition[index].part_start;
            auxEBR.part_size = size;
            auxEBR.part_next =  mbr.mbr_partition[index].part_start + sizeof(EBR) + size;
            strcpy(auxEBR.part_name, name);

            char test = '1';
            fseek(file, auxEBR.part_start, SEEK_SET);
            fwrite(&auxEBR,sizeof(EBR), 1, file);
            for (int i = 0; i < (auxEBR.part_size - (int) sizeof(EBR)); ++i) {
                fwrite(&test, 1, 1, file);
            }
            msj("Partición lógica creada exitosamente!");
        } else {
            /* Recorre hasta encontrar la ultima particion logica */
            bool checkName = true;
            while(auxEBR.part_next != -1) {
                if(strcmp(auxEBR.part_name, name) == 0){
                    checkName = false;
                    break;
                }
                fseek(file, auxEBR.part_start + auxEBR.part_size, SEEK_SET);
                fread(&auxEBR, sizeof(EBR),1, file);
            }

            if(checkName) { /* Checking name unique */
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
                msj("Partición lógica creada exitosamente!");
            } else {
                msj(string("El nombre: '") + name + "' de la particion lógica a crear ya existe");
            }
        }
    } else {
        msj("No se cuenta con espacio suficiente para crear la partición lógica!");
        fclose(file);
        return;
    }
    fclose(file);
}

void Controller::deletePartition(MBR mbr, string path, char name[16], string commandDelete) {
    FILE *file;

    file = fopen(path.c_str(),"rb+");

    string input;
    while(true) {
        msj("Esta seguro de eliminar la particion? [Y/N]");
        cin >> input;
        if(input == "Y" || input == "y") {
            break;
        } else if(input == "N" || input == "n") {
            return;
        }
    }

    int index = -1;
    for (int i = 0; i < 4; ++i) {
        /*Checking if partition exists */
        if(strcmp(mbr.mbr_partition[i].part_name, name) == 0){
            index = i;
            break;
        }
    }
    if(index != -1) {
        /*Checking if partition is unmount*/
        if(mbr.mbr_partition[index].part_status != '2'){
            /*if it is EBR the partition logic is eliminated*/
            if(mbr.mbr_partition[index].part_type == 'e') {
                /* Deleting logical partitions */
                //TODO
            }
            /* Settings for full*/
            if(commandDelete == "full") {
                fseek(file, mbr.mbr_partition[index].part_start, SEEK_SET);
                for (int i = 0; i < mbr.mbr_partition[index].part_size; ++i) {
                    fwrite(&nulo, 1, 1, file);
                }
            }
            /* Common fast and full settings */
            mbr.mbr_partition[index].part_status = '0';
            mbr.mbr_partition[index].part_start = -1;

            fseek(file, 0, SEEK_SET);
            fwrite(&mbr, sizeof(MBR), 1, file);
        } else {
            msj(string("Para poder eliminar la particion : ") + name + " debe de desmontarla primero!");
        }
    } else {
        msj("ERROR: La particion que desea eliminar no existe!");
    }
    fclose(file);
}

void Controller::makeMount(Node *root) {
    list<Node> :: iterator aux;
    aux = root->childs.begin()->childs.begin();
    int counter = 0;
    string path;
    string name;
    while(counter < root->childs.begin()->count){
        if(aux->type == "PATH"){
            path = aux->value;
        } else if (aux->type == "NAME") {
            name = aux->value;
        }
        aux++;
        counter++;
    }
    executeMount(path, name);
}

/**
 *  Mount partitions
 * */
void Controller::executeMount(string path, string name) {
    FILE *file;

    file = fopen(path.c_str(), "rb+");

    if(file == NULL){
        msj("El disco no existe!");
        return;
    }

    bool existsPartition =  false;

    MBR auxMBR;
    fseek(file,0,SEEK_SET);
    fread(&auxMBR, sizeof(MBR), 1, file);

    int indexEBR = -1;
    for (int i = 0; i < 4; ++i) {
        if(auxMBR.mbr_partition[i].part_name == name) {
            listMount->add("", path, name);
            existsPartition = true;
            break;
        }
        if(auxMBR.mbr_partition[i].part_type == 'e'){
            indexEBR = i;
        }
    }

    EBR auxEBR;
    /*If it is different of -1 then is EBR partition*/
    if(indexEBR != -1){
        fseek(file, auxMBR.mbr_partition[indexEBR].part_start, SEEK_SET);
        fread(&auxEBR, sizeof(EBR), 1, file);
        while(auxEBR.part_next != -1){
            if(auxEBR.part_name == name){
                listMount->add("", path, name);
                existsPartition = true;
                break;
            }
            fseek(file, auxEBR.part_next, SEEK_SET);
            fread(&auxEBR, sizeof(EBR), 1, file);
        }
    }
    fclose(file);
    if(!existsPartition){
        cout << "No existe la partición: " << name << endl;
    }
}

void Controller::makeUnMount(Node *root) {
    string id = "";
    if(root->type == "UNMOUNT") {
        id = root->value;
    }
    executeUnMount(id);
}

void Controller::executeUnMount(string id) {
    // DELETE mount of list simple
    int size = id.length()-1;
    string id2 = id.substr(0,size) + (char) toupper(id[size]);
    listMount->unMount(id2);
}

/**
 * REPORTS
 * */

void Controller::executeREP() {
    list<Node> :: iterator aux;
    aux = root->childs.begin()->childs.begin();
    int counter = 0;
    string name;
    string id;
    string path;
    string ruta;
    while(counter < root->childs.begin()->count) {
        if(aux->type == "NAME"){
            name = aux->value;
        } else if(aux->type == "PATH"){
            path = aux->value;
        } else if(aux->type == "ID"){
            id = aux->value;
        } else if(aux->type == "RUTA"){
            ruta = aux->value;
        }
        aux++;
        counter++;
    }
    if(aux->value == "MBR" || aux->value == "mbr"){
        controllerReport->reportMBR(id,path);
    } else if(aux->value == "DISK" || aux->value == "disk"){
    } else if(aux->value == "INODE" || aux->value == "inode"){
    } else if(aux->value == "Journaling" || aux->value == "journaling"){
    } else if(aux->value == "BLOCK" || aux->value == "block"){
    } else if(aux->value == "bm_inode" || aux->value == "BM_INODE"){
    } else if(aux->value == "bm_block" || aux->value == "BM_BLOCK"){
    } else if(aux->value == "tree" || aux->value == "TREE"){
    } else if(aux->value == "sb" || aux->value == "SB"){
    } else if(aux->value == "file" || aux->value == "FILE"){
    } else if(aux->value == "ls" || aux->value == "ls"){
    }
}



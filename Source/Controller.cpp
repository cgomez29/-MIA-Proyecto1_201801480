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
    } else if(root->type == "MKFS") {
        if(commandChecker->checkMKFS(root)) {
            fileSystem->makeMKFS(root);
            return;
        }
    } else if(root->type == "EXEC") {
        if(commandChecker->checkEXEC(root)) {
            executeEXEC(root);
            return;
        }
    } else if(root->type == "MKDIR") {
        if(commandChecker->checkMKDIR(root)) {
            makeMKDIR(root);
            return;
        }
    } else if(root->type == "LOGIN") {
        if(commandChecker->checkLOGIN(root)) {
            makeLOGIN(root);
            return;
        }
    } else if(root->type == "PAUSE") {
        executePAUSE(root);
    }

    cout << "Comando no valido" << endl;
}

void Controller::makeMKDISK(Node *root) {
    list<Node>:: iterator aux;
    aux = root->childs.begin()->childs.begin();
    int counter=0;
    struct MKDISK disk;
    // default values
    disk.f = "b";
    disk.u = "m";
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

    file = fopen(path.c_str(), "r");
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
    mbr.disk_fit = disk.f[0];
    mbr.mbr_disk_signature = rand()%10000;
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


    //Creando carpetas si no existen
    string directory;
    const size_t last_slash = path.rfind('/');
    if(string::npos != last_slash){
        directory = path.substr(0, last_slash) + "/";
    }

    string cmd = "sudo mkdir -p '" + directory + "'";
    system(cmd.c_str());
    cmd = "sudo chmod -R 777 '" + directory + "'";
    system(cmd.c_str());


    file = fopen(path.c_str(), "wb");

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
            /* Recorre hasta encontrar la ultima particion logica */
            bool checkName = true;

            while(auxEBR.part_next != -1) {
                if(strcmp(auxEBR.part_name, name) == 0){
                    checkName = false;
                    break;
                }
                fseek(file, auxEBR.part_next, SEEK_SET);
                fread(&auxEBR, sizeof(EBR),1, file);
            }

            if(checkName) { /* Checking name unique */
                int part_next;
                if(auxEBR.part_next == -1){
                    part_next = auxEBR.part_start + auxEBR.part_size;
                    auxEBR.part_next = part_next;
                    fseek(file, auxEBR.part_start, SEEK_SET);
                    fwrite(&auxEBR, sizeof(EBR), 1, file);
                }

                EBR ebr;
                ebr.part_status = '1';
                ebr.part_fit = fit;
                ebr.part_start = part_next;
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
            listMount->getInstance()->add("", path, name);
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
                listMount->getInstance()->add("", path, name);
                existsPartition = true;
                break;
            }
            fseek(file, auxEBR.part_next, SEEK_SET);
            fread(&auxEBR, sizeof(EBR), 1, file);
        }
        // checking current EBR
        if(auxEBR.part_name == name){
            listMount->getInstance()->add("", path, name);
            existsPartition = true;
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
    listMount->getInstance()->unMount(id2);
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
    int size = id.length()-1;
    string id2 = id.substr(0,size) + (char) toupper(id[size]);
    Mount* auxMount = listMount->getInstance()->existsMount(id2);
    if(auxMount != NULL){
        if(name == "MBR" || name == "mbr"){
            controllerReport->reportMBR(auxMount->getPath() ,path);
        } else if(name == "DISK" || name == "disk"){
            controllerReport->reportDISK(auxMount->getPath(),path);
        } else if(name == "INODE" || name == "inode"){
        } else if(name == "Journaling" || name == "journaling"){
        } else if(name == "BLOCK" || name == "block"){
        } else if(name == "bm_inode" || name == "BM_INODE"){
        } else if(name == "bm_block" || name == "BM_BLOCK"){
        } else if(name == "tree" || name == "TREE"){
            controllerReport->reportTree(auxMount->getPath(), auxMount->getName(), path);
        } else if(name == "sb" || name == "SB"){
            controllerReport->reportSuperBloque(auxMount->getPath(), auxMount->getName(), path);
        } else if(name == "file" || name == "FILE"){
        } else if(name == "ls" || name == "ls"){
        }
    }
}

void Controller::executeEXEC(Node *root) {
    list<Node> :: iterator aux;
    aux = root->childs.begin()->childs.begin();
    int counter = 0;
    string line;
    while(counter < root->childs.begin()->count) {
        if(aux->type == "PATH"){
            ifstream file_input(aux->value);
            while(getline(file_input, line)){
                readCommand(line);
            }
            msj("Script ejecutado correctamente!");
            break;
        }
        aux++;
        counter++;
    }
}

void Controller::makeMKDIR(Node *root) {
    list<Node> :: iterator aux;
    aux = root->childs.begin()->childs.begin();
    int counter = 0;
    string path = "";
    string p = "f";
    while(counter < root->childs.begin()->count) {
        if(aux->type == "PATH"){
            path = aux->value;
        } else if(aux->type == "P"){
            p = aux->value;
        }
        aux++;
        counter++;
    }

    /* El tercer parametro indica el ID de la particion
     *  que esta siendo utilizada */
    if(userLogin.id != ""){
        fileSystem->executeMKDIR(path, p, userLogin.id);
    } else {
        cout << "\n** NO A INICIADO SESIÓN **\n" << endl;
    }
}

void Controller::makeLOGIN(Node *root) {
    list<Node> :: iterator aux;
    aux = root->childs.begin()->childs.begin();
    int counter = 0;
    while(counter < root->childs.begin()->count) {
        if(aux->type == "USUARIO"){
            userLogin.user = aux->value;
        } else if(aux->type == "PASSWORD") {
            //TODO pendiente
        } else if(aux->type == "ID") {
            userLogin.id = aux->value;
        }
        aux++;
        counter++;
    }
    int size = userLogin.id.length()-1;
    string id2 = userLogin.id.substr(0,size) + (char) toupper(userLogin.id[size]);
    userLogin.id = id2;
    Mount* auxMount = listMount->getInstance()->existsMount(id2);

    if(auxMount == NULL){
        cout << "\n Partición: "<< id2 << " No se encuentra montada!\n" << endl;
    } else {
        cout << "\n** BIENVENIDO **" << endl;
        cout << "\n "<< "USUARIO: " << userLogin.user <<" \n" << endl;
    }
}

void Controller::executePAUSE(Node *root) {
    cout << "\nPresione cualquier tecla para continuar ...\n" <<endl;
    cin.get();
}

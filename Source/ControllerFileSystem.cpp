//
// Created by cgomez on 6/03/21.
//

#include "../Headers/ControllerFileSystem.h"

void ControllerFileSystem::makeMKFS(Node *root) {
    list<Node> :: iterator aux;
    aux = root->childs.begin()->childs.begin();
    int counter = 0;
    //default values
    string id, type = "full", fs = "2fs";

    while(counter < root->childs.begin()->count) {
        if(aux->type == "ID"){
            id = aux->value;
        } else if(aux->type == "TYPE"){
            type = aux->value;
        } else if(aux->type == "FS"){
            fs = aux->value;
        }
        aux++;
        counter++;
    }
    executeMKFS(id, type, fs);
}

void ControllerFileSystem::executeMKFS(string id, string type, string fs) {
    int size = id.length()-1;
    string id2 = id.substr(0,size) + (char) toupper(id[size]);

    Mount* partition =  listMount->getInstance()->existsMountObject(id2);

    if( partition != NULL){
        if(fs == "3fs"){
            formatEXT3(partition->getPath(), partition->getName());
        } else{
            formatEXT2(partition->getPath(), partition->getName());
        }
    }
}

void ControllerFileSystem::formatEXT2(string path, string name) {
    format auxformat = getPartitionStart(path, name);

    SuperBlock sblock;
    /*Cantidad de inodos*/
    int n = floor((auxformat.size - sizeof(SuperBlock))/(1+3+sizeof(InodeTable)+3*sizeof(FolderBlock)));
    int bm_inode_start = auxformat.start + sizeof(SuperBlock);
    int bm_block_start = bm_inode_start + n;
    int inode_start = bm_block_start + 3 * n;
    int block_start = inode_start + n*sizeof(InodeTable);

    sblock.s_filesystem_type = 2; // EXT2 Sistema de archivos utilizado
    sblock.s_inodes_count = n; // Número total de inodos
    sblock.s_blocks_count = 3*n; // Número total de bloques
    sblock.s_free_blocks_count = 3*n; // Número de bloques libres
    sblock.s_free_inodes_count = n; // Número de inodos libres
    sblock.s_mtime = time(0); // Última fecha en el que el sistema fue montado
    sblock.s_umtime = time(0); // Última fecha en el que el sistema fue desmontado
    sblock.s_mnt_count = 1; // Indica cuantas veces se ha montado el sistema
    sblock.s_magic = 0xEF53; // Valor que identifica al sistema de archivos
    sblock.s_inode_size = sizeof(InodeTable); // Tamaño del inodo
    sblock.s_block_size = sizeof(FolderBlock); // Tamaño del bloque
    sblock.s_first_ino = 0; // Primer inodo libre
    sblock.s_first_blo = 0; // Primer bloque libre
    sblock.s_bm_inode_start = bm_inode_start; // Inicio del bitmap de inodos
    sblock.s_bm_block_start = bm_block_start; // Inicio del bitmap de bloques
    sblock.s_inode_start = inode_start; // Inicio de la tabla de inodos
    sblock.s_block_start = block_start; // Inido de la tabla de bloques

    FILE *file;
    file = fopen(path.c_str(), "rb+");

    fseek(file, auxformat.start, SEEK_SET);
    fwrite(&sblock, sizeof(SuperBlock), 1, file);
    char cero = '0';
    for (int i = 0; i < sblock.s_inodes_count; ++i) {
        fwrite(&cero, 1,1, file);
    }
    for (int i = 0; i < sblock.s_blocks_count; ++i) {
        fwrite(&cero, 1,1, file);
    }

    /**
     * Se inicializan o no?*
     *
     * fseek(file, sblock.s_inode_start, SEEK_SET);
    InodeTable inodeTable,*/

    /* CREANDO ARCHIVO users.txt */
    fileSystemInit(path, sblock.s_inode_start, sblock.s_block_start, auxformat.start);
    cout << "\nFormateo EXT2 completado correctamente!!\n" << endl;
    fclose(file);
}

void ControllerFileSystem::formatEXT3(string path, string name) {

}

format ControllerFileSystem::getPartitionStart(string path, string name) {
    format aux;
    FILE *file;

    file = fopen(path.c_str(), "rb+");

    MBR auxMBR;
    fseek(file,0,SEEK_SET);
    fread(&auxMBR, sizeof(MBR), 1, file);

    int indexEBR = -1;
    for (int i = 0; i < 4; ++i) {
        if(auxMBR.mbr_partition[i].part_name == name) {
            aux.start = auxMBR.mbr_partition[i].part_start;
            aux.size = auxMBR.mbr_partition[i].part_size;
            return aux;
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
                aux.start = auxEBR.part_start;
                aux.size = auxEBR.part_size;
                return aux;
            }
            fseek(file, auxEBR.part_next, SEEK_SET);
            fread(&auxEBR, sizeof(EBR), 1, file);
        }
        // checking current EBR
        if(auxEBR.part_name == name){
            aux.start = auxEBR.part_start;
            aux.size = auxEBR.part_size;
            return aux;
        }
    }
    fclose(file);
    return aux;
}

void ControllerFileSystem::fileSystemInit(string path, int inode_start, int block_start, int part_start) {
    FILE *file;
    file = fopen(path.c_str(), "rb+");

    InodeTable root; /**Inodo 0*/
    root.i_uid = 1;
    root.i_gid = 1;
    root.i_size = 0;
    root.i_atime = time(0);
    root.i_ctime = time(0);
    root.i_mtime = time(0);
    for (int i = 0; i < 15; ++i) {
        root.i_block[i] = -1;
    }
    root.i_type = '0';
    root.i_perm = 664;
    root.i_block[0] = 0; /**Apunta al bloque 0*/

    fseek(file, inode_start, SEEK_SET);
    fwrite(&root, sizeof(InodeTable), 1, file);

    update_first_ino(part_start, file);

    /*Creando Inodo 1 de tipo file*/
    InodeTable inodo1; /**Inodo 1*/
    inodo1.i_uid = 1;
    inodo1.i_gid = 1;
    inodo1.i_size = 0;
    inodo1.i_atime = time(0);
    inodo1.i_ctime = time(0);
    inodo1.i_mtime = time(0);
    for (int i = 0; i < 15; ++i) {
        inodo1.i_block[i] = -1;
    }
    inodo1.i_type = '1';
    inodo1.i_perm = 664;
    inodo1.i_block[0] = 1; /**Apunta al bloque 1*/
    fseek(file, inode_start + (int) sizeof(InodeTable), SEEK_SET);
    fwrite(&inodo1, sizeof(InodeTable), 1, file);
    update_first_ino(part_start, file);

    //Escribiendo en bloques
    fseek(file, block_start, SEEK_SET);

    /*Creando bloque*/
    FolderBlock fblock; /**Bloque 0*/
    strcpy(fblock.b_content[0].b_name, ".");
    fblock.b_content[0].b_inodo = 0;

    strcpy(fblock.b_content[1].b_name, "..");
    fblock.b_content[1].b_inodo = 0;

    strcpy(fblock.b_content[2].b_name, "users.txt");
    fblock.b_content[2].b_inodo = 1; /**Apunta al Inodo 1*/

    strcpy(fblock.b_content[3].b_name, " ");
    fblock.b_content[3].b_inodo = -1;

    fwrite(&fblock, sizeof(FolderBlock), 1, file);
    update_first_blo(part_start, file);

    /*Creando blque archivo*/
    FileBlock fileBlock; /**Bloque 1*/
    strcpy(fileBlock.b_content, "G,root,U,root,root,123");
    fseek(file, block_start+(int)sizeof(FileBlock), SEEK_SET);
    fwrite(&fileBlock, sizeof(FileBlock), 1, file);
    update_first_blo(part_start, file);
    fclose(file);
}


/************************** MKDIR *********************************/

void ControllerFileSystem::executeMKDIR(string dir, string p, string id) {
    Mount *partition_mount = listMount->existsMount(id);
    format partition = getPartitionStart(partition_mount->getPath(), partition_mount->getName());

    FILE *file;
    file = fopen(partition_mount->getPath().c_str(), "rb+");
    /* Leyendo el super bloque */
    SuperBlock sb;
    fseek(file, partition.start, SEEK_SET);
    fread(&sb, sizeof(SuperBlock), 1, file);

    /* Inodo root */
    InodeTable root;
    fseek(file, sb.s_inode_start, SEEK_SET);
    fread(&root, sizeof(InodeTable),1, file);

    //Simpre comenzamos desde el inodo raiz
    inodo_actual = 0;

    if(p=="p") {
        //Iniciando el recorrido
        if(dir[dir.length()-1] != '/') {
            dir += '/';
        }
        string nameFolder;
        int counter = 1;
        while(counter < dir.length()) {
            if(dir[counter] != '/'){
                nameFolder += dir[counter];
            } else {
                //Actualizando superbloque por cada carpeta creada i/o buscada
                fseek(file, partition.start, SEEK_SET);
                fread(&sb, sizeof(SuperBlock), 1, file);
                //ejecutar nombre de carpeta
                mkdirInodo(root, file, inodo_actual, sb, partition.start, nameFolder);
                nameFolder = "";
            }
            counter++;
        }
        fclose(file);
    } else {
        //Iniciando el recorrido
        if(dir[dir.length()-1] != '/') {
            dir += '/';
        }
        string nameFolder;
        int counter = 1;
        while(counter < dir.length()) {
            if(dir[counter] != '/'){
                nameFolder += dir[counter];
            } else {
                /* Para poder seguir verificando que las carpetas padre de la que va
                 *  a crear exista se comprueba que la anterior si exista */
                if(existFolderBlock) {
                    //Actualizando superbloque por cada carpeta creada i/o buscada
                    fseek(file, partition.start, SEEK_SET);
                    fread(&sb, sizeof(SuperBlock), 1, file);
                    //ejecutar nombre de carpeta
                    existFolderBlock = false;
                    mkdirInodo(root, file, inodo_actual, sb, partition.start, nameFolder);
                    nameFolder = "";
                } else {
                    cout << "\n ** ERROR: La carpeta \"" << carpetaActualBuscada << "\" No existe! \n";
                    break;
                }
            }
            counter++;
        }
        fclose(file);
    }
}

void ControllerFileSystem::mkdirInodo(InodeTable actual, FILE *file, int numero_inodo,
                                      SuperBlock sb, int part_start, string nameFolder) {

    for (int i = 0; i < 13; ++i) {
        //Leyendo el inodo actual para ver si sufrio algun cambio por un bloque nuevo
        fseek(file, sb.s_inode_start+((int) sizeof(InodeTable)*numero_inodo), SEEK_SET);
        fread(&actual, sizeof(InodeTable), 1, file);

        if(actual.i_block[i] != -1){
            /** Checking if folder block */
            //Actualizando superbloque por cada carpeta creada i/o buscada
            fseek(file, part_start, SEEK_SET);
            fread(&sb, sizeof(SuperBlock), 1, file);
            if(actual.i_type == '0') {
                FolderBlock folderblock;
                // direccionaando a nuevo bloque
                fseek(file, sb.s_block_start+((int) sizeof(FolderBlock)*actual.i_block[i]), SEEK_SET);
                fread(&folderblock, sizeof(FolderBlock), 1, file);
                mkdirBlock(folderblock, file, actual.i_block[i], i, sb, numero_inodo, part_start, nameFolder);
            }
        }
    }

}

void ControllerFileSystem::mkdirBlock(FolderBlock actual, FILE *file, int numero_block, int mainBlock, SuperBlock sb,
                                      int numero_inodo, int part_start, string nameFolder) {
    // Con esta variable sabemos si la carpeta existe si no la creamos
    bool folderExists = false;
    for (int i = 0; i < 4; ++i) {
        if (actual.b_content[i].b_inodo != -1 && strcmp(actual.b_content[i].b_name, ".") != 0 &&
            strcmp(actual.b_content[i].b_name, "..") != 0) {
            // Si el folder existe seguimos con su INODO
            carpetaActualBuscada = nameFolder.c_str();
            if (strcmp(actual.b_content[i].b_name, nameFolder.c_str()) == 0) {
                /**
                 * SI ENTRO ES QUE LA CARPETA EXISTE y no me sigo metiendo sino que salgo por que se en que inodo estoy
                 * */
                inodo_actual = numero_inodo;
                folderExists = true;
                existFolderBlock = true;
                /*InodeTable aux;
                fseek(file, sb.s_inode_start + ((int) sizeof(InodeTable) * actual.b_content[i].b_inodo), SEEK_SET);
                fread(&aux, sizeof(InodeTable), 1, file);
                folderExists = true;
                mkdirInodo(aux, file, actual.b_content[i].b_inodo, sb, part_start, nameFolder);*/

            }
        }
    }

    // Como no se encontro la carpeta, buscamos un espacio vacio para poder crear nuestro nueva carpeta
    int hayEspacio = -1; // -1 es igual a que no hay espacio
    if (!folderExists) {
        for (int i = 0; i < 4; ++i) {
            if (actual.b_content[i].b_inodo == -1) {
                hayEspacio = i;
                break;
            }
        }
    }

    if (hayEspacio != -1) { //Si es diferente a -1 significa que hay un espacio en el bloque
        update_bm_block(sb, file); //actualizado el bip map de blocks
        update_bm_inode(sb, file); //actualizado el bip map de inodos

        //Escribiendo el bloque actual b_content.b_inodo en direccion al nuevo inodo
        actual.b_content[hayEspacio].b_inodo = sb.s_first_ino; // Primer inodo libre - dato del superbloque
        strcpy(actual.b_content[hayEspacio].b_name, nameFolder.c_str());
        fseek(file, sb.s_block_start + ((int) sizeof(FolderBlock) * numero_block), SEEK_SET);
        fwrite(&actual, sizeof(FolderBlock), 1, file);

        /** INODO ACTUAL CREADO */
        inodo_actual = sb.s_first_ino;

        //Creando el nuevo inodo
        InodeTable newInodo;
        newInodo.i_uid = 1;
        newInodo.i_gid = 1;
        newInodo.i_size = 0;
        newInodo.i_atime = time(0);
        newInodo.i_ctime = time(0);
        newInodo.i_mtime = time(0);
        for (int i = 0; i < 15; ++i) {
            newInodo.i_block[i] = -1;
        }
        newInodo.i_type = '0';
        newInodo.i_perm = 664;
        newInodo.i_block[0] = sb.s_first_blo; /**Apunta al bloque 0*/
        fseek(file, sb.s_inode_start + ((int) sizeof(InodeTable) * sb.s_first_ino), SEEK_SET);
        fwrite(&newInodo, sizeof(InodeTable), 1, file);

        /* Escribiendo el nuevo bloque */
        FolderBlock newFolder;
        strcpy(newFolder.b_content[0].b_name, ".");
        newFolder.b_content[0].b_inodo = sb.s_first_ino;
        strcpy(newFolder.b_content[1].b_name, "..");
        newFolder.b_content[1].b_inodo = numero_inodo; // INODO Padre
        for (int i = 2; i < 4; ++i) {
            strcpy(newFolder.b_content[i].b_name, " ");
            newFolder.b_content[i].b_inodo = -1;
        }
        fseek(file, sb.s_block_start + ((int) sizeof(FolderBlock) * sb.s_first_blo), SEEK_SET);
        fwrite(&newFolder, sizeof(FolderBlock), 1, file);

        update_first_blo(part_start, file);
        update_first_ino(part_start, file);

    } else {
        //Escribimos el inodo anterior para que apunte al bloque nuevo
        InodeTable inodoAnterior;
        fseek(file, sb.s_inode_start + ((int) sizeof(InodeTable) * numero_inodo), SEEK_SET);
        fread(&inodoAnterior, sizeof(InodeTable), 1, file);

        for (int i = 0; i < 12; ++i) {
            if (inodoAnterior.i_block[i] == -1) { //Buscamos el primer espacio libre en el inodo
                inodoAnterior.i_block[i] = sb.s_first_blo; //Asignano el primer bloque nuevo disponible
                break;
            }
        }
        fseek(file, sb.s_inode_start + ((int) sizeof(InodeTable) * numero_inodo), SEEK_SET);
        fwrite(&inodoAnterior, sizeof(InodeTable), 1, file);

        update_bm_block(sb, file); //actualizado el bip map de blocks

        //Creamos el nuevo bloque vacio
        FolderBlock newFolder;
        for (int i = 0; i < 4; ++i) {
            strcpy(newFolder.b_content[i].b_name, "");
            newFolder.b_content[i].b_inodo = -1;
        }
        fseek(file, sb.s_block_start + ((int) sizeof(FolderBlock) * sb.s_first_blo), SEEK_SET);
        fwrite(&newFolder, sizeof(FolderBlock), 1, file);

        update_first_blo(part_start, file);

    }

}

//Actualiza el bip map y Retorna el numero de bip map bloque correspondiete
void ControllerFileSystem::update_bm_block(SuperBlock sb, FILE *file) {
    char value;
    char uno = '1';
    fseek(file, sb.s_bm_block_start, SEEK_SET);
    fread(&value, 1, 1, file);

    while(value != '0'){
        fseek(file, sb.s_bm_block_start, SEEK_SET);
        fread(&value, 1, 1, file);
    }
    fwrite(&uno, 1, 1, file);
}

//Actualiza el bip map y Retorna el numero de bip map inodo correspond7iete
void ControllerFileSystem::update_bm_inode(SuperBlock sb, FILE *file) {
    char value;
    char uno = '1';
    fseek(file, sb.s_bm_block_start, SEEK_SET);
    fread(&value, 1, 1, file);

    while(value != '0'){
        fseek(file, sb.s_bm_block_start, SEEK_SET);
        fread(&value, 1, 1, file);
    }
    fwrite(&uno, 1, 1, file);
}

//Actualiza el primer inodo libre del superbloque
void ControllerFileSystem::update_first_ino(int partition_start, FILE *file) {
    SuperBlock sb;
    fseek(file, partition_start, SEEK_SET);
    fread(&sb, sizeof(SuperBlock), 1, file);
    sb.s_first_ino = sb.s_first_ino + 1;
    fseek(file, partition_start, SEEK_SET);
    fwrite(&sb, sizeof(SuperBlock), 1, file);
}

//Actualiza el primer bloque libre del superbloque
void ControllerFileSystem::update_first_blo(int partition_start, FILE *file) {
    SuperBlock sb;
    fseek(file, partition_start, SEEK_SET);
    fread(&sb, sizeof(SuperBlock), 1, file);
    sb.s_first_blo = sb.s_first_blo + 1;
    fseek(file, partition_start, SEEK_SET);
    fwrite(&sb, sizeof(SuperBlock), 1, file);
}



/***********************************************************************/


/**
 *  > /users.txt
 * Inode 112
 * Bloques 64
 *
 *
 * MKDIR pag#23
 *
 * Depues de formatear y haber creado el users.txt se debe de logeat para poder usar MKDIR
 *
 * */
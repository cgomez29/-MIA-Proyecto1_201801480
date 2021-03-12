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
    sblock.s_mnt_count = 0; // Indica cuantas veces se ha montado el sistema
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
    fileSystemInit(path, sblock.s_inode_start, sblock.s_block_start);
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

void ControllerFileSystem::fileSystemInit(string path, int inode_start, int block_start) {
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
    fwrite(&inodo1, sizeof(InodeTable), 1, file);

    //Escribiendo en bloques
    fseek(file, block_start, SEEK_SET);

    /*Creando bloque*/
    FolderBlock fblock; /**Bloque 0*/
    Content content;

    strcpy(fblock.b_content[0].b_name, ".");
    fblock.b_content[0].b_inodo = -1;

    strcpy(fblock.b_content[1].b_name, "..");
    fblock.b_content[1].b_inodo = -1;

    strcpy(fblock.b_content[2].b_name, "users.txt");
    fblock.b_content[2].b_inodo = 1; /**Apunta al Inodo 1*/

    strcpy(fblock.b_content[2].b_name, "");
    fblock.b_content[3].b_inodo = -1;

    fwrite(&fblock, sizeof(FolderBlock), 1, file);

    /*Creando blque archivo*/
    FileBlock fileBlock; /**Bloque 1*/
    strcpy(fileBlock.b_content, "G,root,U,root,root,123");
    fwrite(&fileBlock, sizeof(FileBlock), 1, file);
    fclose(file);
}

/**
 *  > /users.txt
 * Inode 112
 * Bloques 64
 * */
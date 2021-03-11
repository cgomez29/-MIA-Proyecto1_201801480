//
// Created by cgomez on 6/03/21.
//

#ifndef PROYECTO1_CONTROLLERFILESYSTEM_H
#define PROYECTO1_CONTROLLERFILESYSTEM_H

#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <cctype>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "Node.h"
#include "CommandChecker.h"
#include "SimpleList.h"
#include "Mount.h"
#include "Structure.h"

extern Node * root;

using namespace std;

struct SuperBlock {
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    time_t s_mtime;
    time_t s_umtime;
    int s_mnt_count;
    int s_magic;
    int s_inode_size;
    int s_block_size;
    int s_first_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
};

struct InodeTable {
    int i_uid = -1; //UID del usuario porpietario del archivo o carpeta
    int i_gid = -1; //GID del grupo al que pertence el archivo o carpeta
    int i_size = -1; //Tamaño del archivo
    time_t i_atime; // Fecha en que se leyo el inodo
    time_t i_ctime; // fecha en que se creo el inodo
    time_t i_mtime; // Ultima fecha en la que se modifico el inodo
    int i_block[15]; // Bloques
    char i_type = -1; // Archivo = 1, Carpeta = 0
    int i_perm; // Permisos
};
/* FOLDER BLOCK */
struct Content{
    char b_name[12];
    int b_inodo;
};

struct FolderBlock{
    Content b_content[4];
};

/* FILE BLOCK */

struct FileBlock {
    char b_content[64];
};

/*POINTER BLOCK*/

struct PointerBlock{
    int b_pointers[16];
};


struct Partition
{
    char part_status = '0';
    char part_type = 'p';
    char part_fit = 'w'; //wf
    int part_start;
    int part_size;
    char part_name[16];
};

struct MBR
{
    int mbr_tamano;
    time_t mbr_fecha_creacion;
    int mbr_disk_signature;
    char disk_fit;
    Partition mbr_partition[4];

};

struct EBR
{
    char part_status;
    char part_fit;
    int part_start;
    int part_size;
    int part_next;
    char part_name[16];
};

class ControllerFileSystem {
private:
    SimpleList* listMount = SimpleList::getInstance();

public:
    /**
     * FILE SYSTEM
     * */
    void makeMKFS(Node *root);
    void executeMKFS(string, string, string);
    void formatEXT2(string path, string name);
    void formatEXT3(string path, string name);
    format getPartitionStart(string path, string name);

    void fileSystemInit(string path, int inode_start, int block_start);
};


#endif //PROYECTO1_CONTROLLERFILESYSTEM_H

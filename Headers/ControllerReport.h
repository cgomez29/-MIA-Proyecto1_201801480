//
// Created by cgomez on 4/03/21.
//

#ifndef PROYECTO1_CONTROLLERREPORT_H
#define PROYECTO1_CONTROLLERREPORT_H

#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include "Structure.h"

using namespace std;

class ControllerReport {
private:
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

    string getTypeExt(string);
    void generateDOT(string, string, string);

public:
    void reportMBR(string diskPath, string path);
    void reportDISK(string diskPath, string path);
    void reportSuperBloque(string diskPath, string part_name, string path);
    format getPartitionStart(string path, string name);
    string getNameDisk(string path);
};


#endif //PROYECTO1_CONTROLLERREPORT_H

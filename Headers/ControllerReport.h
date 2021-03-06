//
// Created by cgomez on 4/03/21.
//

#ifndef PROYECTO1_CONTROLLERREPORT_H
#define PROYECTO1_CONTROLLERREPORT_H

#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>

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
    string getTypeExt(string);
    void generateDOT(string, string, string);

public:
    void reportMBR(string diskPath, string path);
    void reportDISK(MBR mbr);

};


#endif //PROYECTO1_CONTROLLERREPORT_H

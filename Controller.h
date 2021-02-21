//
// Created by cgomez on 20/02/21.
//

#ifndef PROYECTO1_CONTROLLER_H
#define PROYECTO1_CONTROLLER_H

#include <iostream>
#include <fstream>
#include "Node.h"
#include "parser.h"
#include "scanner.h"
#include "CommandChecker.h"
extern Node * root;

using namespace std;

class Controller {
public:
    void run();
private:
    struct MKDISK
    {
        int size = 0;
        string f = "bf";
        string u = "m";
        string path = "";
    };

    struct Partition
    {
        char part_status = '0';
        char part_type = 'p';
        char part_fit = 'w'; //wf
        int part_start;
        int part_size;
        char part_name[16];
        char part_unit = 'k';
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

    CommandChecker *commandChecker = new CommandChecker();
    void readCommand(string input);
    void command(Node *root);
    void makeMKDISK(Node *root);
    void executeMKDISK(MKDISK disk);
    void makeRMDISK(Node *root);
    void executeRMDISK(string path);
    void makeFDISK(Node *root);
    void executeFDISK(Partition part, string path);
    static void msj(string mensaje);
};


#endif //PROYECTO1_CONTROLLER_H

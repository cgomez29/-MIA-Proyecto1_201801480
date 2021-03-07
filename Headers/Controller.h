//
// Created by cgomez on 20/02/21.
//

#ifndef PROYECTO1_CONTROLLER_H
#define PROYECTO1_CONTROLLER_H

#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <cctype>
#include "Node.h"
#include "../Analizador/parser.h"
#include "../Analizador/scanner.h"
#include "CommandChecker.h"
#include "SimpleList.h"
#include "ControllerReport.h"
#include "ControllerFileSystem.h"

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
    ControllerReport *controllerReport = new ControllerReport();
    ControllerFileSystem *fileSystem = new ControllerFileSystem();
    SimpleList* listMount = SimpleList::getInstance();

    static void msj(string mensaje);
    char nulo = '\0';

    void readCommand(string input);
    void command(Node *root);
    void makeMKDISK(Node *root);
    void executeMKDISK(MKDISK disk);
    void makeRMDISK(Node *root);
    void executeRMDISK(string path);
    //FDISK
    void makeFDISK(Node *root);
    void executeFDISK(string path, char type, char fit, int size, char name[16], char unit, int add, string cDelete);
    int firstFit(MBR mbr);
    int bestFit(MBR mbr);
    int worstFit(MBR mbr);
    void createPrimaryPartition(MBR mbr, string path, char fit, int size, char name[16], char unit);
    void createExtendPartition(MBR mbr, string path, char fit, int size, char name[16], char unit);
    void createLogicPartition(MBR mbr, string path, char fit, int size, char name[16], char unit);
    void deletePartition(MBR mbr, string path, char name[16], string commandDelete);
    void deleteEBR(MBR mbr);
    void makeMount(Node *root);
    void executeMount(string path, string name);
    void makeUnMount(Node *root);
    void executeUnMount(string id);

    /**
     * REPORTS
     * */
    void executeREP();


};


#endif //PROYECTO1_CONTROLLER_H

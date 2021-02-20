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
    struct MBR
    {
        int mbr_tamano;
        time_t mbr_fecha_creacion;
        int mbr_disk_signature;
        char disk_fit;
    };


    CommandChecker *commandChecker = new CommandChecker();
    void readCommand(string input);
    void command(Node *root);
    void makeMKDISK(Node *root);
    void executeMKDISK(MKDISK disk);

    static void msj(string mensaje);
};


#endif //PROYECTO1_CONTROLLER_H

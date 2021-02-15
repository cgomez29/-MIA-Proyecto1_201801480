#include <iostream>
#include <string.h>
#include <fstream>
#include "Node.h"
#include "parser.h"
#include "scanner.h"
#include "CommandChecker.h"


extern int yyparse();

extern Node * root;

CommandChecker *commandChecker = new CommandChecker();


using namespace std;

void readCommand(string input);
void command(Node *root);
void executeMKDISK();

int main() {
    string input;
    while(true) {
        cout << "Ingrese un comando:" << endl;
        cout << ">>" ;
        getline(cin, input);
        readCommand(input);
    }

}

void readCommand(string input) {
    if(input[0] != '#' && input.size() > 2){
        YY_BUFFER_STATE bufferState = yy_scan_string(input.c_str());
        if(yyparse()==0){
            command(root);
        }
        yy_delete_buffer(bufferState);
    }
}

void command(Node *root) {
    if(root->type == "MKDISK") {
        if(commandChecker->checkMKDISK(root)) {
            executeMKDISK();
            return;
        }
    }
    cout << "Comando no valido" << endl;
}


struct MKDISK
{
    int size;
    string f = "bf";
    char u = 'm';
    string path;
};

void executeMKDISK() {
    FILE *file;
    file = fopen("efe.dk", "wb");

    char load[1024*1024];

    if(file==NULL) {
        exit(1);
    }
    fwrite(load, sizeof(load),1, file);
    fclose(file);
    cout << "ejecuntado comando." << endl ;
}
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
        commandChecker->checkMKDISK(root);
    }
}

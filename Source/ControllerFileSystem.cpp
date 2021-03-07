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
    sblock.s_filesystem_type = 2;
    /*Cantidad de inodos*/
    int n = floor((auxformat.size - sizeof(SuperBlock))/(1+3+sizeof(InodeTable)+3*sizeof(FileBlock)));
    sblock.s_inodes_count = n;
    sblock.s_blocks_count = 3*n;
    //INODOS n*45
    sblock.s_inode_size = n*45;
    //BLOQUES n*3*64
    sblock.s_block_size = n*3*64;

    sblock.s_magic = 0xEF53;

    cout << "\n"<< to_string(sblock.s_inodes_count)<< "\n" << endl;
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

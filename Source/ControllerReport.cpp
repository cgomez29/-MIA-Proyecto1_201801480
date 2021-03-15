//
// Created by cgomez on 4/03/21.
//

#include "../Headers/ControllerReport.h"

void ControllerReport::reportMBR(string diskPath, string path) {
    FILE *file;
    file = fopen(diskPath.c_str(), "rb+");

    string nameDisk = getNameDisk(diskPath);

    MBR auxMBR;
    fseek(file,0,SEEK_SET);
    fread(&auxMBR, sizeof(MBR), 1, file);

    string content = "digraph {\n"
                     "    \n"
                     " node[ shape=plaintext, fontname = \"Arial\" ];\n"
                     " \n"
                     "    labelloc=\"t\";\n"
                     "    label=\"Reporte \";\n"
                     "    fontsize  = 25; \n"
                     "    fontcolor = aquamarine4;\n"
                     " \n"
                     " subgraph cluster_0 {\n"
                     "     \n"
                     "        tbl [\n"
                     "    label=<\n"
                     "      <table cellspacing='0' CELLPADDING=\"5\">\n"
                     "        <tr><td bgcolor=\"lightblue\"  >Nombre</td><td bgcolor=\"lightblue\">Valor</td></tr>\n"
                     "        <tr><td>mbr_tamaño</td><td> " + to_string(auxMBR.mbr_tamano) + " </td></tr>\n"
                     "        <tr><td>mbr_fecha_creación</td><td> " + asctime(gmtime(&auxMBR.mbr_fecha_creacion)) + " </td></tr>\n"
                     "        <tr><td>mbr_disk_asignature</td><td> " + to_string(auxMBR.mbr_disk_signature) + " </td></tr>\n"
                     "        <tr><td>Disk_fit</td><td> " + auxMBR.disk_fit + "  </td></tr>\n"
                     "        \n";

    int indexEBR = -1;
    for (int i = 0; i < 4; ++i) {
        if(auxMBR.mbr_partition[i].part_status != '0'){
            content += "        <tr><td>part_status_" + to_string(i+1) + " </td><td> "+ auxMBR.mbr_partition[i].part_status +" </td></tr>\n"
                       "        <tr><td>part_type_" + to_string(i+1) + " </td><td> "+ auxMBR.mbr_partition[i].part_type +" </td></tr>\n"
                       "        <tr><td>part_fit_" + to_string(i+1) + " </td><td> "+ auxMBR.mbr_partition[i].part_fit +" </td></tr>\n"
                       "        <tr><td>part_start_" + to_string(i+1) + " </td><td> "+ to_string(auxMBR.mbr_partition[i].part_start) +" </td></tr>\n"
                       "        <tr><td>part_size_" + to_string(i+1) + " </td><td> "+ to_string(auxMBR.mbr_partition[i].part_size) +" </td></tr>\n"
                       "        <tr><td>part_name_" + to_string(i+1) + " </td><td> "+ auxMBR.mbr_partition[i].part_name +" </td></tr>\n"
                       "        \n";
        }
        if(auxMBR.mbr_partition[i].part_type == 'e'){
            indexEBR = i;
        }
    }

    content += "      </table>\n"
               "        \n"
               "    >];\n"
               "    labelloc=\"t\";\n"
               "    label=\"MBR " + nameDisk + " \";\n"
               "    fontsize  = 20;\n"
               "    };\n"
               "\n";

    EBR auxEBR;
    /*If it is different of -1 then is EBR partition*/
    if(indexEBR != -1) {
        fseek(file, auxMBR.mbr_partition[indexEBR].part_start, SEEK_SET);
        fread(&auxEBR, sizeof(EBR), 1, file);
        int counter = 1;
        while (auxEBR.part_next != -1) {
            if (auxEBR.part_status != '0') {
                content += "subgraph cluster_" + to_string(counter) + " {\n"
                          "     \n"
                          "    \n"
                          "    \n"
                          "  tbl" + to_string(counter) + " [\n"
                          "    label= < \n"
                          "      <table cellspacing='0' CELLPADDING=\"5\">\n"
                          "        <tr><td bgcolor=\"lightblue\"  >Nombre</td><td bgcolor=\"lightblue\">Valor</td></tr>\n"
                          "        \n"
                          "        <tr><td>part_status_" + to_string(counter) + " </td><td> " + auxEBR.part_status + " </td></tr>\n"
                          "        <tr><td>part_fit_" +to_string(counter) + " </td><td> " + auxEBR.part_fit + " </td></tr>\n"
                          "        <tr><td>part_start_" +to_string(counter) + " </td><td> " + to_string(auxEBR.part_start) + " </td></tr>\n"
                          "        <tr><td>part_size_" +to_string(counter) + " </td><td> " + to_string(auxEBR.part_size) + " </td></tr>\n"
                          "        <tr><td>part_next_" +to_string(counter) + " </td><td> " + to_string(auxEBR.part_next) + " </td></tr>\n"
                          "        <tr><td>part_name_" +to_string(counter) + " </td><td> " + auxEBR.part_name + " </td></tr>\n"
                          "        \n"
                          "      </table>\n"
                          "        \n"
                          "    >];\n"
                          "    labelloc=\"t\";\n"
                          "    label=\"EBR_" +
                           to_string(counter) + "  \";\n"
                           "    fontsize  = 20;\n"
                           "    }\n";

            }
            counter++;
            fseek(file, auxEBR.part_next, SEEK_SET);
            fread(&auxEBR, sizeof(EBR), 1, file);
        }
        /*Escribe el EBR actual*/
        if (auxEBR.part_status != '0') {
            content += "subgraph cluster_" + to_string(counter) + " {\n"
                       "     \n"
                       "    \n"
                       "    \n"
                       "  tbl" + to_string(counter) + " [\n"
                       "    label= < \n"
                       "      <table cellspacing='0' CELLPADDING=\"5\">\n"
                       "        <tr><td bgcolor=\"lightblue\"  >Nombre</td><td bgcolor=\"lightblue\">Valor</td></tr>\n"
                       "        \n"
                       "        <tr><td>part_status_" +
                       to_string(counter) + " </td><td> " + auxEBR.part_status + " </td></tr>\n"
                                                                                 "        <tr><td>part_fit_" +
                       to_string(counter) + " </td><td> " + auxEBR.part_fit + " </td></tr>\n"
                                                                              "        <tr><td>part_start_" +
                       to_string(counter) + " </td><td> " + to_string(auxEBR.part_start) + " </td></tr>\n"
                                                                                           "        <tr><td>part_size_" +
                       to_string(counter) + " </td><td> " + to_string(auxEBR.part_size) + " </td></tr>\n"
                                                                                          "        <tr><td>part_next_" +
                       to_string(counter) + " </td><td> " + to_string(auxEBR.part_next) + " </td></tr>\n"
                                                                                          "        <tr><td>part_name_" +
                       to_string(counter) + " </td><td> " + auxEBR.part_name + " </td></tr>\n"
                       "        \n"
                       "      </table>\n"
                       "        \n"
                       "    >];\n"
                       "    labelloc=\"t\";\n"
                       "    label=\"EBR_" + to_string(counter) +
                       "  \";\n"
                       "    fontsize  = 20;\n"
                       "    }\n";

        }
    }
    fclose(file);
    content +=       "}";
    generateDOT("ReportMBR.txt", path, content);
}

void ControllerReport::reportDISK(string diskPath, string path) {
    FILE *file;
    file = fopen(diskPath.c_str(), "rb+");

    MBR auxMBR;
    fseek(file,0,SEEK_SET);
    fread(&auxMBR, sizeof(MBR), 1, file);

    string content = "digraph structs {\n"
                     "    rankdir=LR\n"
                     "    node [shape=plaintext]\n"
                     "    \n"
                     "    a [label=<\n"
                     "<TABLE BORDER=\"2\" CELLBORDER=\"1\" CELLSPACING=\"5\" CELLPADDING=\"5\">\n"
                     "  <TR>\n"
                     "    <TD ROWSPAN=\"3\" WIDTH=\"10\">MBR</TD>\n"
                     "  \n";                                                                                                                                                                                                                                                                                                                       "        \n";

    int indexEBR = -1;
    for (int i = 0; i < 4; ++i) {
        if(auxMBR.mbr_partition[i].part_status != '0'){

            if(auxMBR.mbr_partition[i].part_type != 'e'){
                long porcentaje = auxMBR.mbr_partition[i].part_size*100/auxMBR.mbr_tamano;
                content += "    <TD ROWSPAN=\"3\" WIDTH=\"10\">Primaria \n" +
                        to_string(porcentaje) + "%</TD>\n";
            }
        }
        if(auxMBR.mbr_partition[i].part_type == 'e'){
            content += "    <TD COLSPAN=\"20\" WIDTH=\"10\">\n"
                       "        Extendida\n"
                       "    </TD>\n"
                       "  \n";
            indexEBR = i;
        }
    }

    content += "  </TR>\n"
               "  \n";

    EBR auxEBR;
    int counter = 1;
    /*If it is different of -1 then is EBR partition*/
    if(indexEBR != -1){
        long porcentaje =0;
        fseek(file, auxMBR.mbr_partition[indexEBR].part_start, SEEK_SET);
        fread(&auxEBR, sizeof(EBR), 1, file);
        content += "  \n"
                   "  <TR >\n";
        while(auxEBR.part_next != -1){
            porcentaje = auxEBR.part_size*100/auxMBR.mbr_tamano;

            content += "    <TD > EBR| Lógica " + to_string(porcentaje) + "%</TD>\n"
                       "  \n";
            counter++;
            fseek(file, auxEBR.part_next, SEEK_SET);
            fread(&auxEBR, sizeof(EBR), 1, file);
        }
        /*Escribe el EBR actual*/
        porcentaje = auxEBR.part_size*100/auxMBR.mbr_tamano;
        content +="    <TD > EBR| Lógica " + to_string(porcentaje) + "%</TD>\n"
                   "  </TR>\n"
                   "  \n";

    }
    content += "\n"
               "</TABLE>>];\n"
               "}";
    fclose(file);
    generateDOT("ReportDISK.txt", path, content);
}

string ControllerReport::getTypeExt(string path) {
    string ext = path.substr(path.find(".")+1, path.size());
    if(ext == "pdf"){
        return "-Tpdf";
    }else if(ext == "png"){
        return "-Tpng";
    }else if(ext == "jpg"){
        return "-Tjpg";
    }
    return "-Tpng";
}

void ControllerReport::generateDOT(string name, string path, string dot) {
    string directory;

    const size_t last_slash = path.rfind('/');
    if(string::npos != last_slash){
        directory = path.substr(0, last_slash) + "/";
    }

    string cmd = "sudo mkdir -p '" + directory + "'";
    system(cmd.c_str());
    cmd = "sudo chmod -R 777 '" + directory + "'";
    system(cmd.c_str());

    name = directory + "/" + name;

    ofstream file;
    file.open(name, ios_base::out | ios_base::trunc);
    if(file.is_open()){
        file << dot;
        file.close();
    }

    cmd = "dot " + getTypeExt(path) + " " + name + " -o " + path;
    system(cmd.c_str());
    //For open file
    //cmd = "xdg-open '" + path + "'";
    //system(cmd.c_str());
    cout << "\n REPORTE GENERADO \n" << endl;
}

void ControllerReport::reportSuperBloque(string diskPath, string part_name, string path) {
    /* return the partition start and size */
    format partition = getPartitionStart(diskPath, part_name);

    FILE *file;
    file = fopen(diskPath.c_str(), "rb+");
    SuperBlock auxSB;
    fseek(file, partition.start, SEEK_SET);
    fread(&auxSB, sizeof(SuperBlock), 1, file);

    stringstream content;
    content << "digraph {\n"
               "    \n"
               " node[ shape=plaintext, fontname = \"Arial\" ];\n"
               " \n"
               "    labelloc=\"t\";\n"
               "    label=\"Reporte SUPER BLOQUE \";\n"
               "    fontsize  = 25; \n"
               "    fontcolor = darkslategray4;\n"
               " \n"
               " subgraph cluster_0 {\n"
               "     \n"
               "        tbl [\n"
               "    label=<\n"
               "      <table cellspacing='0' CELLPADDING=\"5\">\n"
               "        <tr><td bgcolor=\"gray82\"  >Nombre</td><td bgcolor=\"gray82\">Valor</td></tr>\n"
               "        <tr><td>s_filesystem_type</td><td> " << auxSB.s_filesystem_type << " </td></tr>\n"
               "        <tr><td>s_inodes_count</td><td> "<< auxSB.s_inodes_count <<" </td></tr>\n"
               "        <tr><td>s_blocks_count</td><td> "<< auxSB.s_blocks_count <<" </td></tr>\n"
               "        <tr><td>s_free_blocks_count</td><td> "<< auxSB.s_free_blocks_count <<" </td></tr>\n"
               "        \n"
               "        <tr><td>s_free_inodes_count </td><td> "<< auxSB.s_free_inodes_count <<" </td></tr>\n"
               "        <tr><td>s_mtime </td><td> "<< asctime(gmtime(&auxSB.s_mtime)) <<" </td></tr> \n"
               "        <tr><td>s_umtime </td><td> "<< asctime(gmtime(&auxSB.s_umtime)) <<" </td></tr>\n"
               "        <tr><td>s_mnt_count </td><td> "<< auxSB.s_mnt_count <<" </td></tr>\n"
               "        <tr><td>s_magic </td><td> "<< auxSB.s_magic <<" </td></tr>\n"
               "        <tr><td>s_inode_size  </td><td> "<< auxSB.s_inode_size<<" </td></tr>\n"
               "        <tr><td>s_block_size  </td><td> "<< auxSB.s_block_size <<" </td></tr>\n"
               "        <tr><td>s_first_ino </td><td> "<< auxSB.s_first_ino <<" </td></tr>\n"
               "        <tr><td>s_first_blo </td><td> "<< auxSB.s_first_blo <<" </td></tr>\n"
               "        <tr><td>s_bm_inode_start </td><td> "<< auxSB.s_bm_inode_start <<" </td></tr>\n"
               "        <tr><td>s_bm_block_start </td><td> "<< auxSB.s_bm_block_start <<" </td></tr>\n"
               "        <tr><td>s_inode_start </td><td> "<< auxSB.s_inode_start <<" </td></tr>\n"
               "        <tr><td>s_block_start </td><td> "<< auxSB.s_block_start <<" </td></tr>\n"
               "        \n"
               "      </table>\n"
               "        \n"
               "    >];\n"
               "    labelloc=\"t\";\n"
               "    label=\"Particion: "<< part_name <<" disco: " << getNameDisk(diskPath) << " \";\n"
               "    fontsize  = 20;\n"
               "    };\n"
               "}";

    fclose(file);
    generateDOT("ReportSB.txt", path, content.str());
}

format ControllerReport::getPartitionStart(string path, string name) {
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
                // A las particiones logicas le sumamos lo que ocupa el EBR y se lo restamos al size
                aux.start = auxEBR.part_start +(int) sizeof(EBR) ;
                aux.size = auxEBR.part_size-(int) sizeof(EBR) ;
                return aux;
            }
            fseek(file, auxEBR.part_next, SEEK_SET);
            fread(&auxEBR, sizeof(EBR), 1, file);
        }
        // checking current EBR
        if(auxEBR.part_name == name){
            aux.start = auxEBR.part_start+(int) sizeof(EBR) ;
            aux.size = auxEBR.part_size-(int) sizeof(EBR) ;
            return aux;
        }
    }
    fclose(file);
    return aux;
}

string ControllerReport::getNameDisk(string path) {
    string nameDisk;
    const size_t last_slash = path.rfind('/');
    if(string::npos != last_slash){
        nameDisk = path.substr(last_slash+1,path.length()-1);
    }
    return nameDisk;
}

void ControllerReport::reportTree(string diskPath, string part_name, string path) {
    /* return the partition start and size */
    format partition = getPartitionStart(diskPath, part_name);

    FILE *file;
    file = fopen(diskPath.c_str(), "rb+");
    SuperBlock sb;
    fseek(file, partition.start, SEEK_SET);
    fread(&sb, sizeof(SuperBlock), 1, file);

    stringstream content;
    content << "digraph {\n"
               " node[ shape = none, fontname = \"Arial\" ];";

    /*ROOT*/
    InodeTable inodo;
    fseek(file, sb.s_inode_start, SEEK_SET);
    fread(&inodo, sizeof(InodeTable),1, file);

    graphTreeInodo(&content, inodo, file, sb.s_inode_start, sb.s_block_start, 0);

    content << "\n}" << endl;

    fclose(file);
    generateDOT("ReportTREE.txt", path, content.str());
}

void ControllerReport::graphTreeInodo(stringstream *cadena, InodeTable padre, FILE *file, int inode_start, int block_start,
                                      int numero_inodo) {
    *cadena << "\nino"<<numero_inodo<<" [\n"
               "    label=<\n"
               "    <table color='black' cellspacing='0'>\n"
               "            <tr><td bgcolor=\"gray\" PORT=\""<<numero_inodo<<"""\">Inodo</td><td bgcolor=\"gray\">"<<numero_inodo<<" </td></tr>\n"
               "            <tr><td>i_uid</td><td> "<< padre.i_uid <<" </td></tr>\n"
               "            <tr><td>i_gid</td><td> "<< padre.i_gid <<" </td></tr>\n"
               "            <tr><td>i_size</td><td> "<< padre.i_size <<" </td></tr>\n"
               "            <tr><td>i_atime</td><td> "<< asctime(gmtime(&padre.i_atime)) <<" </td></tr>\n"
               "            <tr><td>i_ctime</td><td> "<< asctime(gmtime(&padre.i_ctime)) <<" </td></tr>\n"
               "            <tr><td>i_mtime </td><td> "<< asctime(gmtime(&padre.i_mtime)) <<" </td></tr>\n";

    for (int i = 0; i < 15; ++i) {
        if(padre.i_block[i] != -1) {
            *cadena << "<tr><td>i_block[" << i+1 << "] </td><td PORT=\"c"<< padre.i_block[i] <<"\">"<<padre.i_block[i]<<"</td></tr>\n";
        } else {
            *cadena << "<tr><td>i_block[" << i+1 << "] </td><td>"<<padre.i_block[i]<<"</td></tr>\n";
        }
    }
    *cadena << "            <tr><td> i_type </td><td> "<< padre.i_type <<" </td></tr>\n"
               "            <tr><td>i_perm </td><td> "<< padre.i_perm <<" </td></tr>\n"
               "    </table>        \n"
               "    >];\n";

    for (int i = 0; i < 15; ++i) {
        if (padre.i_block[i] != -1) {
            *cadena << "ino" << numero_inodo << ":c" << padre.i_block[i] << " -> bl" << padre.i_block[i] << ":" << padre.i_block[i]
                    << " ;\n";
        }
    }

    for (int i = 0; i < 13; ++i) {
        if(padre.i_block[i] != -1) {
            if(padre.i_type == '0'){
                FolderBlock aux;
                fseek(file, block_start+((int) sizeof(FolderBlock)*padre.i_block[i]), SEEK_SET);
                fread(&aux, sizeof(FolderBlock), 1, file);
                graphTreeFolderBlock(cadena, aux, file, inode_start, block_start, padre.i_block[i]);
            } else {
                FileBlock fileBlock;
                fseek(file, block_start+((int) sizeof(FileBlock)*padre.i_block[i]), SEEK_SET);
                fread(&fileBlock, sizeof(FileBlock), 1, file);
                *cadena << "bl"<< numero_inodo <<"[\n"
                           "    label=<\n"
                           "        <table color='black' cellspacing='0'>\n"
                           "            <tr><td bgcolor=\"gold2\" PORT=\"1\">Bloque  </td><td bgcolor=\"gold2\">"<<padre.i_block[i]<<"</td></tr>\n"
                           "            <tr><td> b_content </td><td> "<< fileBlock.b_content <<" </td></tr>\n"
                           "       \n"
                           "        </table>\n"
                           "    >];\n";
            }
        }
    }
}

void ControllerReport::graphTreeFolderBlock(stringstream *cadena, FolderBlock actual, FILE *file, int inode_start,
                                            int block_start, int numero_block) {
    *cadena << "bl"<< numero_block <<"  [\n"
               "    label=<\n"
               "        <table color='black' cellspacing='0'>\n"
               "            <tr><td bgcolor=\"greenyellow\" PORT=\""<<numero_block<<"\">""Bloque  </td><td bgcolor=\"greenyellow\">"<<numero_block<<" </td></tr>\n"
               "            <tr><td>Name</td><td>Inodo </td></tr>\n";
    for (int i = 0; i < 4; ++i) {
        if(actual.b_content[i].b_inodo != -1){
            *cadena << "    <tr><td> "<<actual.b_content[i].b_name<<" </td><td PORT=\"b"<< actual.b_content[i].b_inodo <<
            "\"> "<<actual.b_content[i].b_inodo<<"</td></tr>\n";
        } else {
            *cadena << "    <tr><td> "<<actual.b_content[i].b_name<<" </td><td>"<<actual.b_content[i].b_inodo<<"</td></tr>\n";
        }
    }
    *cadena << "        </table>\n"
               "    >];\n";

    for (int i = 0; i < 4; ++i) {
        if(actual.b_content[i].b_inodo != -1 && strcmp(actual.b_content[i].b_name, ".") != 0 && strcmp(actual.b_content[i].b_name, "..") != 0){
            InodeTable aux;
            fseek(file, inode_start + ((int) sizeof(InodeTable) * actual.b_content[i].b_inodo), SEEK_SET);
            fread(&aux, sizeof(InodeTable), 1, file);
            *cadena << "bl"<<numero_block<<":b"<<actual.b_content[i].b_inodo << " -> ino"<< actual.b_content[i].b_inodo <<":"<< actual.b_content[i].b_inodo<< " ;\n";
            graphTreeInodo(cadena, aux, file, inode_start, block_start, actual.b_content[i].b_inodo);
        }
    }
}

void ControllerReport::reporBMBlock(string diskPath, string part_name, string path) {
    format partition = getPartitionStart(diskPath, part_name);

    FILE *file;
    file = fopen(diskPath.c_str(),"rb+");
    SuperBlock sb;

    fseek(file, partition.start, SEEK_SET);
    fread(&sb, sizeof(SuperBlock), 1, file);

    char value;
    fseek(file, sb.s_bm_block_start, SEEK_SET);
    stringstream content;
    int count = 0;
    for (int i = 0; i < sb.s_blocks_count; ++i) {
        fseek(file, sb.s_bm_block_start+i, SEEK_SET);
        fread(&value, 1, 1, file);
        if(count == 0){
            content << "|";
        }
        count++;
        content << " " << value << " |";
        if(count == 20){
            content << "\n";
            count = 0;
        }
    }
    fclose(file);
    generateTXT(path, content.str());
}


void ControllerReport::generateTXT( string path, string dot) {
    string directory;

    const size_t last_slash = path.rfind('/');
    if(string::npos != last_slash){
        directory = path.substr(0, last_slash) + "/";
    }

    string name = path.substr(last_slash + 1, path.size());
    int punto = name.find('.');
    //Asegurandonos que solo genere un archivo .txt
    string nameDoc = name.substr(0, punto)+".txt";


    string cmd = "sudo mkdir -p '" + directory + "'";
    system(cmd.c_str());
    cmd = "sudo chmod -R 777 '" + directory + "'";
    system(cmd.c_str());

    nameDoc = directory + "/" + nameDoc;

    ofstream file;
    file.open(nameDoc, ios_base::out | ios_base::trunc);
    if(file.is_open()){
        file << dot;
        file.close();
    }
    cout << "\n REPORTE GENERADO \n" << endl;
}

void ControllerReport::reporBMInode(string diskPath, string part_name, string path) {
    format partition = getPartitionStart(diskPath, part_name);

    FILE *file;
    file = fopen(diskPath.c_str(),"rb+");
    SuperBlock sb;

    fseek(file, partition.start, SEEK_SET);
    fread(&sb, sizeof(SuperBlock), 1, file);

    char value;
    fseek(file, sb.s_bm_inode_start, SEEK_SET);
    stringstream content;
    int count = 0;
    for (int i = 0; i < sb.s_inodes_count; ++i) {
        fseek(file, sb.s_bm_inode_start+i, SEEK_SET);
        fread(&value, 1, 1, file);
        if(count == 0){
            content << "|";
        }
        count++;
        content << " " << value << " |";
        if(count == 20){
            content << "\n";
            count = 0;
        }
    }
    fclose(file);
    generateTXT(path, content.str());
}

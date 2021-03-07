//
// Created by cgomez on 4/03/21.
//

#include "../Headers/ControllerReport.h"

void ControllerReport::reportMBR(string diskPath, string path) {
    FILE *file;
    file = fopen(diskPath.c_str(), "rb+");

    string nameDisk;

    const size_t last_slash = diskPath.rfind('/');
    if(string::npos != last_slash){
        nameDisk = diskPath.substr(last_slash+1,diskPath.length()-1);
    }

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
    if(indexEBR != -1){
        fseek(file, auxMBR.mbr_partition[indexEBR].part_start, SEEK_SET);
        fread(&auxEBR, sizeof(EBR), 1, file);
        int counter = 1;
        while(auxEBR.part_next != -1){
            content += "subgraph cluster_" + to_string(counter) + " {\n"
                       "     \n"
                       "    \n"
                       "    \n"
                       "  tbl2 [\n"
                       "    label= < \n"
                       "      <table cellspacing='0' CELLPADDING=\"5\">\n"
                       "        <tr><td bgcolor=\"lightblue\"  >Nombre</td><td bgcolor=\"lightblue\">Valor</td></tr>\n"
                       "        \n"
                       "        <tr><td>part_status_"+ to_string(counter) +" </td><td> " + auxEBR.part_status +" </td></tr>\n"
                       "        <tr><td>part_fit_"+ to_string(counter) +" </td><td> "+ auxEBR.part_fit +" </td></tr>\n"
                       "        <tr><td>part_start_"+ to_string(counter) +" </td><td> "+ to_string(auxEBR.part_start) +" </td></tr>\n"
                       "        <tr><td>part_size_"+ to_string(counter) +" </td><td> "+ to_string(auxEBR.part_size) +" </td></tr>\n"
                       "        <tr><td>part_next_"+ to_string(counter) +" </td><td> "+ to_string(auxEBR.part_next) +" </td></tr>\n"
                       "        <tr><td>part_name_"+ to_string(counter) +" </td><td> "+ auxEBR.part_name +" </td></tr>\n"
                       "        \n"
                       "      </table>\n"
                       "        \n"
                       "    >];\n"
                       "    labelloc=\"t\";\n"
                       "    label=\"EBR_"+ to_string(counter)+"  \";\n"
                       "    fontsize  = 20;\n"
                       "    }\n";
            counter++;

            fseek(file, auxEBR.part_next, SEEK_SET);
            fread(&auxEBR, sizeof(EBR), 1, file);
        }
        /*Escribe el EBR actual*/
        content += "subgraph cluster_" + to_string(counter) + " {\n"
                   "     \n"
                   "    \n"
                   "    \n"
                   "  tbl2 [\n"
                   "    label= < \n"
                   "      <table cellspacing='0' CELLPADDING=\"5\">\n"
                   "        <tr><td bgcolor=\"lightblue\"  >Nombre</td><td bgcolor=\"lightblue\">Valor</td></tr>\n"
                   "        \n"
                   "        <tr><td>part_status_"+ to_string(counter) +" </td><td> " + auxEBR.part_status +" </td></tr>\n"
                   "        <tr><td>part_fit_"+ to_string(counter) +" </td><td> "+ auxEBR.part_fit +" </td></tr>\n"
                   "        <tr><td>part_start_"+ to_string(counter) +" </td><td> "+ to_string(auxEBR.part_start) +" </td></tr>\n"
                   "        <tr><td>part_size_"+ to_string(counter) +" </td><td> "+ to_string(auxEBR.part_size) +" </td></tr>\n"
                   "        <tr><td>part_next_"+ to_string(counter) +" </td><td> "+ to_string(auxEBR.part_next) +" </td></tr>\n"
                   "        <tr><td>part_name_"+ to_string(counter) +" </td><td> "+ auxEBR.part_name +" </td></tr>\n"
                   "        \n"
                   "      </table>\n"
                   "        \n"
                   "    >];\n"
                   "    labelloc=\"t\";\n"
                   "    label=\"EBR_"+ to_string(counter)+"  \";\n"
                   "    fontsize  = 20;\n"
                   "    }\n";
    }
    fclose(file);
    content +=       "}";
    generateDOT("ReportMBR.txt", path, content);
}

void ControllerReport::reportDISK(string diskPath, string path) {
    FILE *file;
    file = fopen(diskPath.c_str(), "rb+");

    string nameDisk;
    int total =0;
    const size_t last_slash = diskPath.rfind('/');
    if(string::npos != last_slash){
        nameDisk = diskPath.substr(last_slash+1,diskPath.length()-1);
    }

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
    string ext = path.substr(path.find("."), path.size());
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
    cmd = "xdg-open '" + path + "'";
    system(cmd.c_str());
    cout << "Reporte generado" << endl;
}

string ControllerReport::reportDISKEBR(int indexEBR, ControllerReport::MBR auxMBR) {
    return std::string();
}

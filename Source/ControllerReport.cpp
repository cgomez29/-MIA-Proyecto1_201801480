//
// Created by cgomez on 4/03/21.
//

#include "../Headers/ControllerReport.h"

void ControllerReport::reportMBR(string id, string path) {
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
                     "        <tr><td>mbr_tamaño</td><td>   </td></tr>\n"
                     "        <tr><td>mbr_fecha_creación</td><td>   </td></tr>\n"
                     "        <tr><td>mbr_disk_asignature</td><td>   </td></tr>\n"
                     "        <tr><td>Disk_fit</td><td>   </td></tr>\n"
                     "        \n"
                     "        <tr><td>part_status_ </td><td>   </td></tr>\n"
                     "        <tr><td>part_type_ </td><td>   </td></tr>\n"
                     "        <tr><td>part_fit_ </td><td>   </td></tr>\n"
                     "        <tr><td>part_start_ </td><td>   </td></tr>\n"
                     "        <tr><td>part_size_ </td><td>   </td></tr>\n"
                     "        <tr><td>part_name_ </td><td>   </td></tr>\n"
                     "        \n"
                     "      </table>\n"
                     "        \n"
                     "    >];\n"
                     "    labelloc=\"t\";\n"
                     "    label=\"MBR \";\n"
                     "    fontsize  = 20;\n"
                     "    };\n"
                     "\n"
                     "subgraph cluster_1 {\n"
                     "     \n"
                     "    \n"
                     "    \n"
                     "  tbl2 [\n"
                     "    label= < \n"
                     "      <table cellspacing='0' CELLPADDING=\"5\">\n"
                     "        <tr><td bgcolor=\"lightblue\"  >Nombre</td><td bgcolor=\"lightblue\">Valor</td></tr>\n"
                     "        \n"
                     "        <tr><td>part_status_ </td><td>   </td></tr>\n"
                     "        <tr><td>part_fit_ </td><td>   </td></tr>\n"
                     "        <tr><td>part_start_ </td><td>   </td></tr>\n"
                     "        <tr><td>part_size_ </td><td>   </td></tr>\n"
                     "        <tr><td>part_next_ </td><td>   </td></tr>\n"
                     "        <tr><td>part_name_ </td><td>   </td></tr>\n"
                     "        \n"
                     "      </table>\n"
                     "        \n"
                     "    >];\n"
                     "    labelloc=\"t\";\n"
                     "    label=\"EBR \";\n"
                     "    fontsize  = 20;\n"
                     "    }\n"
                     "}";

    generateDOT("ReporteMBR.txt", "/home/cgomez/Escritorio/REPORTES/reporte.png", content);
}

void ControllerReport::reportDISK(ControllerReport::MBR mbr) {
    FILE *filee;
    fopen("/home/cgomez/Escritorio/Disco1.dk", "rb+");

    fseek(filee, 0, SEEK_SET);
    fread(&mbr, sizeof(MBR),1, filee);

    string cadena = "digraph a {\n"
                    "    rankdir=LR\n"
                    "    node [shape=plaintext]\n"
                    "    \n"
                    "    a [label=<\n"
                    "<TABLE BORDER=\"2\" CELLBORDER=\"1\" CELLSPACING=\"5\" CELLPADDING=\"5\">\n"
                    "  <TR>\n"
                    "    <TD ROWSPAN=\"3\" WIDTH=\"10\">MBR</TD>\n";


    for (int i = 0; i < 4; ++i) {
        if(mbr.mbr_partition[i].part_status == '1'){
            if(mbr.mbr_partition[i].part_type == 'e'){
                cadena+=     "    <TD COLSPAN=\"3\" WIDTH=\"10\">\n"
                             "        Extendida\n"
                             "    </TD>\n"
                             "  </TR>\n"
                             "  \n";

                EBR auxEBR;
                fseek(filee, mbr.mbr_partition[i].part_start, SEEK_SET);
                fread(&auxEBR, sizeof(EBR), 1, filee);

                while(auxEBR.part_next != -1){
                    cadena+= "  <TR >\n"
                             "    <TD >Lógica</TD>\n"
                             "  </TR>\n";
                }
            } else {
                cadena += "    <TD ROWSPAN=\"3\" WIDTH=\"10\">"+ string(mbr.mbr_partition[i].part_name) +"</TD>\n";
            }
        }
    }

    cadena +="  \n"
             "\n"
             "</TABLE>>];\n"
             "\n"
             "\n"
             "}";

    string comando = "dot -Tpng  MBR.dot -o MBR.png";
    string path = "MBR.png";

    ofstream file("MBR.dot");
    file << cadena.c_str();
    file.close();

    //cerrando archivo de prueba
    fclose(filee);

    system(comando.c_str());
    system(path.c_str());
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

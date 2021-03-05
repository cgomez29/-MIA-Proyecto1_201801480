#include <stdio.h>
#include "Headers/Controller.h"
#include "Headers/ControllerReport.h"

#include "Headers/SimpleList.h"

int main() {
    Controller *controller = new Controller();
    controller->run();

    //ControllerReport *c = new ControllerReport();
    //c->reportMBR("","");


    //controller->generateDOT();
    /*SimpleList *s = new SimpleList();

    s->add("f", "f/efe", "efe");
    s->add("f1", "f/efe", "efe2");
    s->add("f3", "f/efe", "efe3");
    s->add("f4", "f/efe", "efe4");
    s->add("f5", "f/efe", "efe5");
    s->add("f6", "f/efe", "efe6");
    s->printList();

    s->unMount("f4");

    std::cout << "--------------------" << endl;

    s->printList();*/
}

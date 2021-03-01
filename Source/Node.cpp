//
// Created by cgomez on 11/02/21.
//

#include "../Headers/Node.h"


Node::Node(string _type, char _value[])
{
    type = _type;
    value = _value;
    if(value[0] == '"'){
        string aux;
        for(int i=0; i < value.size(); i++){
            if(value[i] != '"'){
                aux += value[i];
            }
        }
        value = aux;
    }
    count = 0;
}

void Node::add(Node newNode){
    childs.push_back(newNode);
    count++;
}

//
// Created by cgomez on 11/02/21.
//

#ifndef PROYECTO1_NODE_H
#define PROYECTO1_NODE_H

#include <iostream>
#include <list>

using namespace std;

class Node
{
public:
    Node(string _type, char _value[]);
    string type;
    string value;
    int count;
    list<Node> childs;
    void add(Node newNode);
};


#endif //PROYECTO1_NODE_H

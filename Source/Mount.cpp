//
// Created by cgomez on 28/02/21.
//

#include "../Headers/Mount.h"

Mount::Mount() {
    this->next = nullptr;
}

Mount::Mount(string _id, string _path, string _name) {
    this->id = _id;
    this->path = _path;
    this->name = _name;
}

void Mount::setNext(Mount *next) {
    this->next = next;
}

Mount *Mount::getNext() {
    return this->next;
}

const string &Mount::getId() const {
    return id;
}

void Mount::setId(const string &id) {
    Mount::id = id;
}

const string &Mount::getPath() const {
    return path;
}

void Mount::setPath(const string &path) {
    Mount::path = path;
}

const string &Mount::getName() const {
    return name;
}

void Mount::setName(const string &name) {
    Mount::name = name;
}

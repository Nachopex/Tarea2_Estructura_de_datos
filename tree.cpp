#include "tree.hpp"
#include <algorithm>
#include <functional>
#include <string>

// Node
Tree::Node::Node(std::string titulo, std::string ISBN, int year_publicacion, int id, std::string idioma, std::string descripcion, double rating_promedio, int num_paginas, Node* p) {
    this->id = id;
    this->titulo = titulo;
    this->ISBN = ISBN;
    this->year_publicacion = year_publicacion;
    this->idioma = idioma;
    this->descripcion = descripcion;
    this->rating_promedio = rating_promedio;
    this->num_paginas = num_paginas;
    parent = p;
}

// Constructor
Tree::Tree() {
    rootNode = nullptr;
    treeSize = 0;
}

bool Tree::isEmpty() {
    return treeSize == 0;
}

int Tree::size() {
    return treeSize;
}

int Tree::root() {
    if (!rootNode) throw std::runtime_error("Árbol vacío");
    return rootNode->id;
}

Tree::Node* Tree::search(Node* node, int id) {
    if (!node) return nullptr;
    if (node->id == id) return node;

    for (auto child : node->children) {
        Node* found = search(child, id);
        if (found) return found;
    }
    return nullptr;
}

bool Tree::insert(int parentValue, std::string titulo, std::string ISBN, int year_publicacion, int id, std::string idioma, std::string descripcion, double rating_promedio, int num_paginas) {
    if (!rootNode) {
        rootNode = new Node(titulo, ISBN, year_publicacion, id, idioma, descripcion, rating_promedio, num_paginas);
        treeSize++;
        return true;
    }

    Node* parentNode = search(rootNode, parentValue);
    if (!parentNode) return false;

    Node* newNode = new Node(titulo, ISBN, year_publicacion, id, idioma, descripcion, rating_promedio, num_paginas, parentNode);
    parentNode->children.push_back(newNode);
    treeSize++;
    return true;
}

int Tree::parent(int id) {
    Node* node = search(rootNode, id);
    if (!node || !node->parent)
        throw std::runtime_error("No tiene padre");

    return node->parent->id;
}

std::vector<int> Tree::children(int id) {
    Node* node = search(rootNode, id);
    std::vector<int> result;

    if (!node) return result;

    for (auto child : node->children)
        result.push_back(child->id);

    return result;
}

void Tree::deleteSubtree(Node* node) {
    if (!node) return;
    for (auto child : node->children)
        deleteSubtree(child);
    delete node;
}

bool Tree::remove(int id) {
    Node* node = search(rootNode, id);
    if (!node) return false;

    if (node == rootNode) {
        deleteSubtree(rootNode);
        rootNode = nullptr;
        treeSize = 0;
        return true;
    }

    Node* parent = node->parent;
    auto& siblings = parent->children;

    siblings.erase(
        std::remove(siblings.begin(), siblings.end(), node),
        siblings.end()
    );

    deleteSubtree(node);
    treeSize--;
    return true;
}


void Tree::preOrder(Node* node, std::vector<int>& result) {
    if (!node) return;
    result.push_back(node->id);
    for (auto child : node->children)
        preOrder(child, result);
}

std::vector<int> Tree::preOrder() {
    std::vector<int> result;
    preOrder(rootNode, result);
    return result;
}

double Tree::obtenerRating(int id){
    Node* nodo=search(rootNode,id);
    if(!nodo) return -1; 
    return nodo->rating_promedio;
}

int Tree::obtenerAnoPublicacion(int id){
    Node* nodo =search(rootNode,id);
    if(!nodo) return -1;
    return nodo->year_publicacion;
}

Tree::~Tree() {
    deleteSubtree(rootNode);
}
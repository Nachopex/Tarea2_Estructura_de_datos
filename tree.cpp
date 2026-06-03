#include "tree.hpp"
#include <algorithm>
#include <functional>
#include <string>

// Constructor de la estructura Node. Inicializa un nodo con los datos de un libro y su nodo padre.
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

// Constructor de la clase Tree. Inicializa un árbol vacío.
Tree::Tree() {
    rootNode = nullptr;
    treeSize = 0;
}

// Verifica si el árbol está vacío. Retorna true si no tiene elementos, false en caso contrario.
bool Tree::isEmpty() {
    return treeSize == 0;
}

// Retorna la cantidad total de nodos (libros) actualmente en el árbol.
int Tree::size() {
    return treeSize;
}

// Retorna el ID del nodo raíz del árbol. Lanza una excepción si el árbol está vacío.
int Tree::root() {
    if (!rootNode) throw std::runtime_error("Árbol vacío");
    return rootNode->id;
}

// Busca un nodo por su ID de forma recursiva a partir de un nodo inicial dado.
// Retorna un puntero al nodo si se encuentra, o nullptr si no existe.
Tree::Node* Tree::search(Node* node, int id) {
    if (!node) return nullptr;
    if (node->id == id) return node;

    for (auto child : node->children) {
        Node* found = search(child, id);
        if (found) return found;
    }
    return nullptr;
}

// Inserta un nuevo nodo (libro) en el árbol como hijo del nodo con el ID especificado (parentValue).
// Retorna true si se insertó con éxito, o false si el nodo padre no existe.
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

// Retorna el ID del padre de un nodo dado su ID.
// Lanza una excepción si el nodo no existe o si es la raíz (no tiene padre).
int Tree::parent(int id) {
    Node* node = search(rootNode, id);
    if (!node || !node->parent)
        throw std::runtime_error("No tiene padre");

    return node->parent->id;
}

// Retorna un vector con los IDs de todos los hijos directos de un nodo dado su ID.
std::vector<int> Tree::children(int id) {
    Node* node = search(rootNode, id);
    std::vector<int> result;

    if (!node) return result;

    for (auto child : node->children)
        result.push_back(child->id);

    return result;
}

// Función auxiliar que elimina recursivamente un nodo y toda su descendencia (subárbol) para liberar memoria.
void Tree::deleteSubtree(Node* node) {
    if (!node) return;
    for (auto child : node->children)
        deleteSubtree(child);
    delete node;
}

// Elimina un nodo del árbol dado su ID, junto con todos sus hijos.
// Retorna true si se eliminó con éxito, o false si el nodo no se encontró.
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

// Función auxiliar recursiva para realizar un recorrido preorder y almacenar los IDs en el vector referenciado.
void Tree::preOrder(Node* node, std::vector<int>& result) {
    if (!node) return;
    result.push_back(node->id);
    for (auto child : node->children)
        preOrder(child, result);
}

// Realiza un recorrido preorder del árbol y retorna un vector con los IDs de los nodos visitados en ese orden.
std::vector<int> Tree::preOrder() {
    std::vector<int> result;
    preOrder(rootNode, result);
    return result;
}

// Retorna el rating promedio de un libro dado su ID. Retorna -1 si el libro no se encuentra.
double Tree::obtenerRating(int id){
    Node* nodo=search(rootNode,id);
    if(!nodo) return -1; 
    return nodo->rating_promedio;
}

// Retorna el año de publicación de un libro dado su ID. Retorna -1 si el libro no se encuentra.
int Tree::obtenerAnoPublicacion(int id){
    Node* nodo =search(rootNode,id);
    if(!nodo) return -1;
    return nodo->year_publicacion;
}

// Destructor de la clase Tree. Libera toda la memoria ocupada por el árbol eliminando la raíz y su subárbol.
Tree::~Tree() {
    deleteSubtree(rootNode);
}
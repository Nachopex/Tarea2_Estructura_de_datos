#include <vector>
#include <iostream>
#include <string>
class Tree {
private:
    struct Node {
        int id;
        std::string titulo;
        std::string ISBN;
        int year_publicacion;
        std::string idioma;
        std::string descripcion;
        double rating_promedio;
        int num_paginas;

        Node* parent;
        std::vector<Node*> children;

        Node(std::string titulo, std::string ISBN, int year_publicacion, int id = -1, std::string idioma = "", std::string descripcion = "", double rating_promedio = 0, int num_paginas = 0, Node* p = nullptr);
    };

    Node* rootNode;
    int treeSize;

    void preOrder(Node* node, std::vector<int>& result);
    void deleteSubtree(Node* node); //si se borra un nodo, se borran todos sus hijos y luego ese nodo.

public:
    Tree();

    bool isEmpty();
    int size();

    int root();

    int parent(int value);
    std::vector<int> children(int value);

    bool insert(int parentValue, std::string titulo, std::string ISBN, int year_publicacion, int id, std::string idioma, std::string descripcion, double rating_promedio, int num_paginas);
    bool remove(int value);
    
    Node* search(Node* node, int value);

    std::vector<int> preOrder();

    double obtenerRating(int id);

    int obtenerAnoPublicacion(int id);

    ~Tree();
};
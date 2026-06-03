#include <vector>
#include <iostream>
#include <string>

// Clase que representa un árbol n-ario diseñado para almacenar y organizar información de libros.
class Tree {
private:
    // Estructura interna que representa un nodo del árbol. Cada nodo contiene los metadatos de un libro.
    struct Node {
        int id;
        std::string titulo;
        std::string ISBN;
        int year_publicacion;
        std::string idioma;
        std::string descripcion;
        double rating_promedio;
        int num_paginas;

        Node* parent; // Puntero al nodo padre.
        std::vector<Node*> children; // Lista de punteros a los nodos hijos.

        // Constructor del nodo para inicializar los datos del libro y su relación jerárquica.
        Node(std::string titulo, std::string ISBN, int year_publicacion, int id = -1, std::string idioma = "", std::string descripcion = "", double rating_promedio = 0, int num_paginas = 0, Node* p = nullptr);
    };

    Node* rootNode; // Puntero al nodo raíz del árbol.
    int treeSize;   // Contador de la cantidad total de nodos (libros) en el árbol.

    // Función auxiliar recursiva para poblar el vector con un recorrido preorder.
    void preOrder(Node* node, std::vector<int>& result);
    
    // Función auxiliar para liberar memoria. Si se borra un nodo, se borran todos sus hijos y luego ese nodo.
    void deleteSubtree(Node* node); 

public:
    // Constructor por defecto. Inicializa un árbol vacío.
    Tree();

    // Verifica si el árbol no tiene ningún nodo.
    bool isEmpty();
    
    // Retorna la cantidad actual de nodos en el árbol.
    int size();

    // Retorna el ID del nodo raíz.
    int root();

    // Retorna el ID del nodo padre de un nodo dado.
    int parent(int value);
    
    // Retorna una lista con los IDs de todos los hijos de un nodo dado.
    std::vector<int> children(int value);

    // Inserta un nuevo libro en el árbol como hijo del nodo con ID 'parentValue'.
    bool insert(int parentValue, std::string titulo, std::string ISBN, int year_publicacion, int id, std::string idioma, std::string descripcion, double rating_promedio, int num_paginas);
    
    // Elimina un nodo específico y todos sus descendientes del árbol dado su ID.
    bool remove(int value);
    
    // Busca y retorna un puntero al nodo que coincida con el ID ('value').
    Node* search(Node* node, int value);

    // Inicia un recorrido preorder desde la raíz y retorna un vector con los IDs visitados.
    std::vector<int> preOrder();

    // Retorna el rating promedio del libro asociado al ID indicado.
    double obtenerRating(int id);

    // Retorna el año de publicación del libro asociado al ID indicado.
    int obtenerAnoPublicacion(int id);

    // Destructor de la clase. Libera toda la memoria ocupada por los nodos del árbol.
    ~Tree();
};
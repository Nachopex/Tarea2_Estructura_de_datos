/* Archivo: main.cpp
 Programa para revisar archivos XML de libros y construir una estructura de árbol.
 Utiliza la librería tinyxml2 para extraer metadatos de libros desde una 
 carpeta específica y almacenarlos en un árbol n-ario.
*/
#include <iostream>

#include <dirent.h>  // Librería para leer directorios
#include "tinyxml2.h"
#include <string>
#include <vector>

#include "tree.hpp"

using namespace tinyxml2;

// Lee un directorio de archivos XML y construye un árbol con los datos extraídos.
// Retorna un puntero al árbol dinámico creado y poblado con los datos.
Tree* crear_arbol_XML(){
    Tree* tree = new Tree();
    // Crear un nodo raiz
    tree->insert(0, "nodo_raiz", "0", 0, 0, "", "", 0.0, 0);

    
    // Datos a utilizar de los XML
    int id;
    int id_padre;
    std::string titulo;
    std::string ISBN;
    int year_publicacion;
    std::string idioma;
    std::string descripcion;
    double rating_promedio;
    int num_paginas;
    
    // ruta de los XMLs
    std::string path = "books_xml";
    DIR *dir;
    struct dirent *read;


    // Intentamos abrir la carpeta
    if ((dir = opendir(path.c_str())) != NULL) {
        
        // Leemos archivo por archivo
        while ((read = readdir(dir)) != NULL) {

            std::string archivo = read->d_name;
            
            // Ignoramos los directorios del sistema "." y ".."
            if (archivo != "." && archivo != "..") {
                
                
                std::string ruta_completa = path + "/" + archivo;
                XMLDocument doc;
                XMLError error = doc.LoadFile(ruta_completa.c_str());

                // Si se carga con éxito, procedemos a extraer datos
                if (error == XML_SUCCESS) {
                    std::cout << "Exito al cargar " << ruta_completa << "\n";
                    
                    // Tomamos todos los datos del libro

                    XMLElement* GoodreadsResponse = doc.FirstChildElement("GoodreadsResponse");
                    if(GoodreadsResponse == nullptr){
                        continue;
                    }
                    XMLElement* book = GoodreadsResponse->FirstChildElement("book");
                    if(book == nullptr){
                        continue;
                    }
                    XMLElement* element;
                    
                    // Extraemos el título
                    element = book->FirstChildElement("title");
                    if(element != nullptr && element->GetText() != nullptr) {
                        titulo = element->GetText();
                    }
                    else{
                        titulo = "Sin titulo";
                    }
                    
                    // Extraemos el ISBN
                    element = book->FirstChildElement("isbn");
                    if(element != nullptr && element->GetText() != nullptr){
                        ISBN = element->GetText();
                    }
                    else{
                        ISBN = "0";
                    }
                    
                    // Extraemos el año de publicación
                    element = book->FirstChildElement("publication_year");
                    if(element != nullptr){
                        element->QueryIntText(&year_publicacion);
                    }
                    else{
                        year_publicacion = 0;
                    }
                    
                    // Extraemos el ID del libro (será el padre de los similares)
                    element = book->FirstChildElement("id");
                    if(element != nullptr){
                        element->QueryIntText(&id_padre);
                    }
                    else{
                        id_padre = -2;
                    }
                    
                    // Extraemos el código de idioma
                    element = book->FirstChildElement("language_code");
                    if(element != nullptr && element->GetText() != nullptr){
                        idioma = element->GetText();
                    }
                    else{
                        idioma = "N/A";
                    }
                    
                    // Extraemos la descripción
                    element = book->FirstChildElement("description");
                    if(element != nullptr && element->GetText() != nullptr){
                        descripcion = element->GetText();
                    }
                    else{
                        descripcion = "N/A";
                    }
                    
                    // Extraemos el rating promedio
                    element = book->FirstChildElement("average_rating");
                    if(element != nullptr){
                        element->QueryDoubleText(&rating_promedio);
                    }
                    else{
                        rating_promedio = 0.0;
                    }
                    
                    // Extraemos el número de páginas
                    element = book->FirstChildElement("num_pages");
                    if(element != nullptr){
                        element->QueryIntText(&num_paginas);
                    }
                    else{
                        num_paginas = 0;
                    }
                    
                    // Insertamos el libro principal en la raíz del árbol (id 0)
                    tree->insert(0,titulo, ISBN, year_publicacion, id_padre, idioma, descripcion, rating_promedio, num_paginas);

                    // Buscamos si tiene libros similares
                    XMLElement* verificacion_similar_book = book->FirstChildElement("similar_books");

                    if(verificacion_similar_book != nullptr){
                        XMLElement* similar_book = verificacion_similar_book->FirstChildElement("book");
                        while(similar_book != nullptr){
                            
                            // Tomamos todos los datos de los libros similares
                            element = similar_book->FirstChildElement("title");
                            if(element != nullptr && element->GetText() != nullptr) {
                                titulo = element->GetText();
                            }
                            else{
                                titulo = "Sin titulo";
                            }
                            element = similar_book->FirstChildElement("isbn");
                            if(element != nullptr && element->GetText() != nullptr){
                                ISBN = element->GetText();
                            }
                            else{
                                ISBN = "0";
                            }
                            element = similar_book->FirstChildElement("publication_year");
                            if(element != nullptr){
                                element->QueryIntText(&year_publicacion);
                            }
                            else{
                                year_publicacion = 0;
                            }
                            element = similar_book->FirstChildElement("id");
                            if(element != nullptr){
                                element->QueryIntText(&id);
                            }
                            else{
                                continue;
                            }
                            
                            // Insertamos el libro similar bajo el ID del libro padre
                            tree->insert(id_padre, titulo, ISBN, year_publicacion, id, "N/A", "N/A", 0.0, 0);
                            
                            // Pasamos al siguiente libro similar en la lista
                            similar_book = similar_book->NextSiblingElement("book");
                        }
                    }
                    
                    
                } else {
                    std::cout << "Error al cargar " << ruta_completa << "\n";
                }
            }
        }
        closedir(dir);
    } else {
        std::cout << "Error: No se pudo abrir la carpeta " << path << "\n";
    }

    return tree;
}

// Recorre el árbol recursivamente para imprimir los IDs de los libros y contarlos.
void listar(Tree* tree, int id, int* contador){

    // Si no es la raíz, imprimimos y sumamos al contador
    if (id != 0) {
        std::cout << "ID Libro: " << id << "\n";
        *contador = *contador+1;
    }
    
    // Si es la raíz, iteramos por sus hijos
    if (id == 0){
        for(int id_hijo : tree->children(id)){
            listar(tree, id_hijo, contador);
        }
    }
}

// Función auxiliar que verifica si un libro es "precursor". 
// Un libro es precursor si todos sus libros similares tienen un año de publicación estrictamente mayor al suyo.
// Retorna true si cumple la condición, false en caso contrario o si los datos son inválidos.
bool es_precursor(Tree* tree, int id){
    // primero obtenemos los libros similares (hijos)
    std::vector<int> hijos=tree->children(id);
    if(hijos.empty()){
        return false;
    }
    // si no tiene hijos, no es precursor

    int year_libro=tree->obtenerAnoPublicacion(id);
    // por cada libro similar verificamos que el año de publicación sea <= al libro original
    for(int id_hijo : hijos){
        int year_similar=tree->obtenerAnoPublicacion(id_hijo);
        // verificamos que sea válido
        if(year_similar <=0) return false; 
        if(year_similar <= year_libro){
            return false;
        }
    }
    return true;
}

// Busca y lista por consola los IDs de todos los libros principales que son precursores.
// Itera sobre los hijos directos de la raíz y muestra el total de precursores encontrados al final.
void precursores(Tree* tree) {
    if(tree->isEmpty()) {
        std::cout << "El árbol está vacío. \n";
        return;
    }
    // con este vector guardo los id de los precursores
    std::vector<int> ids_precursores;
    
    // Vemos si por cada hijo de la raiz, hay precursores
    for(int id_hijo : tree->children(0)){
        if(es_precursor(tree,id_hijo)){
            ids_precursores.push_back(id_hijo);
        }
    }

    if(ids_precursores.empty()){
        std::cout << "No se encontraron precursores. \n";
    }else{
        std::cout << "IDs de precursores encontrados: \n";
        for(int id : ids_precursores){
            std::cout << id << "\n";
        } 
        std::cout << "Precursores encontrados en total: " << ids_precursores.size() << std::endl;
    }    
}

// Función auxiliar que evalúa si el rating de un libro específico es menor o igual a 'r' y mayor o igual a 0.0. 
// Si cumple la condición, agrega su ID al vector referenciado para ser borrado posteriormente.
void buscar_ratings(Tree* tree, int id,double r, std::vector<int>& ids_a_borrar){  
    double rating=tree->obtenerRating(id);
    if(rating <=r && rating >= 0.0 ){
        ids_a_borrar.push_back(id);
        return;
    }
}

// Elimina del árbol todos los libros principales cuyo rating promedio sea menor o igual al valor 'r'.
// Recopila los IDs primero para no alterar el árbol mientras se itera, y luego procede a eliminarlos.
void borrar_ratings(Tree* tree, double r){
    if(tree->isEmpty()) {
        std::cout << "El árbol está vacío. \n";
        return;
    }
    
    std::vector<int> ids_a_borrar;

    for(int id_hijo : tree->children(0)){
        buscar_ratings(tree,id_hijo,r,ids_a_borrar);
    }

    for(int id : ids_a_borrar){
        if(tree->remove(id)){
            std::cout << "Se borró libro con ID: " << id << std::endl;
        }
    }
    std::cout << "Libros ha eliminar en total: " << ids_a_borrar.size() << std::endl;
}

// Función principal del programa. Construye el árbol cargando los datos de los archivos XML 
// e inicia un menú interactivo en consola para ejecutar las distintas operaciones solicitadas.
int main() {
    Tree* tree = crear_arbol_XML();
    if(tree->isEmpty()) {
        std::cout << "No se pudieron cargar los libros\n";
        return 1;
    }
    
    int opcion;
    double rating;
    
    do {
        std::cout << "\n=== Opciones ===\n";
        std::cout << "1. Listar libros (preorder)\n";
        std::cout << "2. Borrar libros con rating <= r\n";
        std::cout << "3. Mostrar libros precursores\n";
        std::cout << "4. Salir\n";
        std::cout << "Opcion: ";
        std::cin >> opcion;
        
        switch(opcion) {
            case 1: {
                int contador = 0;
                listar(tree, tree->root(), &contador);
                std::cout << "Total de libros listados: " << contador << "\n";
                break;
            }
            case 2:
                std::cout << "Ingrese rating limite (r): ";
                std::cin >> rating;
                if(rating < 0.0 || rating > 5.0){
                    std::cout << "Rating invalido. Debe estar entre 0.0 y 5.0\n";
                    break;
                }
                borrar_ratings(tree, rating);
                break;
            case 3:
                precursores(tree);
                break;
            case 4:
                std::cout << "Saliendo...\n";
                break;
            default:
                std::cout << "\nOpcion invalida\n";
        }
    } while(opcion != 4);
    
    delete tree;
    return 0;
}
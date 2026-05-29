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

// Función principal. Inicia el proceso, lista y muestra el conteo total.
int main() {
    Tree* tree = crear_arbol_XML();
    int contador = 0;
    listar(tree, tree->root(), &contador);
    std::cout << "libros totales contados en listar =  " << contador << "\n";
    return 0;
}
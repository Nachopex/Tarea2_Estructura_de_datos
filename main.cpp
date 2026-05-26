#include <iostream>

#include <dirent.h>  // Librería para leer directorios
#include "tinyxml2.h"
#include <string>
#include <vector>

#include "tree.hpp"

using namespace tinyxml2;

Tree* crear_arbol_XML(){
    Tree* tree = new Tree();
    // Crear un nodo raiz
    tree->insert(0, "nodo_raiz", "0", 0, -1, "", "", 0.0, 0);

    
    //Datos a utilizar de los XML
    int id;
    int id_padre;
    std::string titulo;
    std::string ISBN;
    int year_publicacion;
    std::string idioma;
    std::string descripcion;
    double rating_promedio;
    int num_paginas;
    //ruta de los XMLs
    std::string path = "books_xml";
    DIR *dir;
    struct dirent *read;


    //Intentamos abrir la carpeta
    if ((dir = opendir(path.c_str())) != NULL) {
        
        //Leemos archivo por archivo
        while ((read = readdir(dir)) != NULL) {

            std::string archivo = read->d_name;
            
            //Ignoramos los directorios del sistema "." y ".."
            if (archivo != "." && archivo != "..") {
                
                
                std::string ruta_completa = path + "/" + archivo;
                XMLDocument doc;
                XMLError error = doc.LoadFile(ruta_completa.c_str());

                if (error == XML_SUCCESS) {
                    std::cout << "Exito al cargar " << ruta_completa << "\n";
                    //Tomamos todos los datos del libro
                    XMLElement* book = doc.FirstChildElement("book");
                    if(book == nullptr){
                        continue;
                    }
                    XMLElement* element;
                    element = book->FirstChildElement("title");
                    if(element != nullptr && element->GetText() != nullptr) {
                        titulo = element->GetText();
                    }
                    else{
                        titulo = "Sin titulo";
                    }
                    element = book->FirstChildElement("isbn");
                    if(element != nullptr && element->GetText() != nullptr){
                        ISBN = element->GetText();
                    }
                    else{
                        ISBN = "0";
                    }
                    element = book->FirstChildElement("publication_year");
                    if(element != nullptr){
                        element->QueryIntText(&year_publicacion);
                    }
                    else{
                        year_publicacion = 0;
                    }
                    element = book->FirstChildElement("id");
                    if(element != nullptr){
                        element->QueryIntText(&id_padre);
                    }
                    else{
                        id_padre = -2;
                    }
                    element = book->FirstChildElement("language_code");
                    if(element != nullptr && element->GetText() != nullptr){
                        idioma = element->GetText();
                    }
                    else{
                        idioma = "N/A";
                    }
                    element = book->FirstChildElement("description");
                    if(element != nullptr && element->GetText() != nullptr){
                        descripcion = element->GetText();
                    }
                    else{
                        descripcion = "N/A";
                    }
                    element = book->FirstChildElement("average_rating");
                    if(element != nullptr){
                        element->QueryDoubleText(&rating_promedio);
                    }
                    else{
                        rating_promedio = 0.0;
                    }
                    element = book->FirstChildElement("num_pages");
                    if(element != nullptr){
                        element->QueryIntText(&num_paginas);
                    }
                    else{
                        num_paginas = 0;
                    }
                    
                    tree->insert(-1,titulo, ISBN, year_publicacion, id_padre, idioma, descripcion, rating_promedio, num_paginas);

                    XMLElement* verificacion_similar_book = book->FirstChildElement("similar_books");

                    if(verificacion_similar_book != nullptr){
                        XMLElement* similar_book = verificacion_similar_book->FirstChildElement("book");
                        while(similar_book != nullptr){
                            //Tomamos todos los datos de los libros similares
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
                            

                            tree->insert(id_padre, titulo, ISBN, year_publicacion, id, "N/A", "N/A", 0.0, 0);
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


int main() {
    Tree* tree = crear_arbol_XML();

    return 0;
}
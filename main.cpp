#include <iostream>
#include <string>
#include <dirent.h>  // Librería para leer directorios
#include "tinyxml2.h"

using namespace tinyxml2;

int main() {
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
                } else {
                    std::cout << "Error al cargar " << ruta_completa << "\n";
                }
            }
        }
        closedir(dir);
    } else {
        std::cout << "Error: No se pudo abrir la carpeta " << path << "\n";
    }

    return 0;
}
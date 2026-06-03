# Tarea2_Estructura_de_datos
_Luis Martinez Neira_<br>
_2023427985_

_Valentina Serón Canales_<br>
_2020901589_

## Sobre el entregable
Este proyecto escrito en C++ permite crear un árbol general a partir de un dataset de 10.000 archivos XML de GoodReads. Todo esto usando la biblioteca `tinyxml2` para extraer los metadatos de los libros.

## Funcionalidades
A través de un menú interactivo en consola, el programa permite:
1. **Listar:** Listar los IDs de los libros principales siguiendo un recorrido preorder.
2. **Borrar ratings:** Eliminar del árbol todos los libros con un rating promedio menor o igual a un parámetro `r` dado.
3. **Precursores:** Listar los IDs de los libros que sólo tengan libros similares publicados en años estrictamente posteriores.

## Requisitos y Configuración
* **Sistema Operativo:** El código fuente utiliza la librería `<dirent.h>` para la lectura automatizada del directorio. Por lo tanto, debe ser compilado y ejecutado en un entorno **Linux, macOS, o utilizando WSL/MinGW en Windows**.
* **Dataset:** Para que el programa lea correctamente los datos, la carpeta que contiene los 10.000 archivos XML debe estar en el mismo directorio que los archivos fuente y llamarse exactamente **`books_xml`**.

## Sobre la compilación y ejecución
Para probar este proyecto, se debe abrir la terminal en el directorio del proyecto y compilar con el siguiente comando:
```bash
g++ main.cpp tree.cpp tinyxml2.cpp -o tarea2 -std=c++11
```
y para ejecutar 
```bash
./tarea2
```
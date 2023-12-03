# version OpenMP

Requisitos:

compilador g++

libreria opencv

CMake

## Compilacion:

Para compilar la version paralela usando OpenMP primeramente debe abrir la terminal y situarse en la direccion donde descargó el repositorio, entonces escribir el comando:

```cd trabajo-SO/openmp/build```

una vez situado en ese directorio, se compila con los comandos:

```cmake ..```

seguido de:

```make```

## Ejecucion:

 para ejecutarlo debe tener una imagen dentro del directorio build, entonces todavia estando en el directorio build debe escribir un comando con el siguiente formato, remplazando los <> con sus propios datos:

```./openmp <nombre_de_imagen_original>.<extension_de_la_imagen> <nombre_de_la_imagen_a_generar>.<extension_de_la_imagen_a_generar> <N°_de_hilos>```

el directorio build ya tiene dentro una imagen de prueba llamada "images.jpeg", por lo que puede usar el siguiente comando como ejemplo:

```./openmp images.jpeg gray.jpeg 3```

en ese caso se generará una imagen llamada "gray.jpeg" dentro del directorio build, siendo ejecutado con 3 hilos.

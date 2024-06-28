#include "map.h"
#include <stdlib.h>
#include <stdio.h>

// Función para crear un nuevo Map
Map *map_create() {
    Map *map = (Map *)malloc(sizeof(Map));
    map->hashArray = (MapPair **)malloc(sizeof(MapPair *) * 100);
    for (int i = 0; i < 100; i++){
        map->hashArray[i] = NULL;
    }
    map->size = 0;
    map->capac = 100;
    return map;
}

// Función para liberar la memoria ocupada por el Map
void map_destroy(Map *map) {
    if (map != NULL) {
        if (map->hashArray != NULL) {
            // Liberar cada MapPair en hashArray
            for (int i = 0; i < map->capac; i++) {
                if (map->hashArray[i] != NULL) {
                    free(map->hashArray[i]);
                }
            }
            // Liberar hashArray
            free(map->hashArray);
        }
        // Liberar Map
        free(map);
    }
}

// Función para insertar un elemento en el Map
void map_insert(Map *map, int key, void *data) {
    if ((double)(map->size / map->capac) > 0.75) {
        map_redimensionar(map, map->capac);
        printf("o");
    }
    int pos = key % map->capac;
    if(map->hashArray[pos] == NULL || map->hashArray[pos]->key == -1){
        MapPair *NewPair = (MapPair *)malloc(sizeof(MapPair));
        NewPair->key = key;
        NewPair->data = data;
        map->hashArray[pos] = NewPair;
        map->size++;
    } else {
        int new_pos = resolve_collision(map, pos);
        MapPair *NewPair = (MapPair *)malloc(sizeof(MapPair));
        NewPair->key = key;
        NewPair->data = data;
        map->hashArray[new_pos] = NewPair;
        map->size++;
    }
}

// Función para buscar un elemento en el Map
MapPair *map_search(Map *map, int key) {
    int pos = key % map->capac;
    if(map->hashArray[pos] == NULL){
        return NULL;
    }
    if(map->hashArray[pos]->key == key){
        return map->hashArray[pos];
    } else {
        int new_pos = resolve_collision(map, pos);
        if(map->hashArray[new_pos] == NULL){
            return NULL;
        }
        if(map->hashArray[new_pos]->key == key){
            return map->hashArray[new_pos];
        }
    }
    return NULL;
}

// Función para eliminar un elemento del Map
MapPair *map_remove(Map *map, int key) {
    int pos = key % map->capac;
    if(map->hashArray[pos] == NULL){
        return NULL;
    }
    if(map->hashArray[pos]->key == key){
        MapPair *aux = map->hashArray[pos];
        map->hashArray[pos] = NULL;
        map->size--;
        return aux;
    }
    return NULL;
}

// Función para redimensionar el Map
Map *map_redimensionar(Map *map, int capac) {
    Map *new_map = map_create();
    int new_capac = capac * 2;
    new_map->capac = new_capac;
    new_map->size = map->size;
    new_map->hashArray = (MapPair **)malloc(new_capac * sizeof(MapPair *));
    for (int i = 0; i < new_capac; i++) {
        new_map->hashArray[i] = NULL;
    }
    for (int i = 0; i < map->capac; i++){
        if(map->hashArray[i] != NULL){
            map_insert(new_map, map->hashArray[i]->key, map->hashArray[i]->data);
            free(map->hashArray[i]);
        }
    }
    free(map->hashArray);
    map->hashArray = new_map->hashArray;
    map->capac = new_capac;
    map->size = new_map->size;
    free(new_map);
    return map;
}

// Función para resolver colisiones en el Map
int resolve_collision(Map *map, int pos) {
    int new_pos = pos;
    int i = 1;
    while(map->hashArray[new_pos] != NULL){
        new_pos = (pos + i * i) % map->capac;
        i++;
    }
    return new_pos;
}

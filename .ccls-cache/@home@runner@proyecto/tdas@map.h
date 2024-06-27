#ifndef MAP_H
#define MAP_H

typedef struct MapPair {
    void* data;
    int key;
} MapPair;

typedef struct Map {
    MapPair** hashArray;
    int size;
    int capac;
    int current; // recorrido
} Map;

Map *map_create();
void map_destroy(Map *map);
void map_insert(Map *map, int key, void *data);
MapPair *map_search(Map *map, int key);
MapPair *map_remove(Map *map, int key);
Map *map_redimensionar(Map *map, int capac);
int resolve_collision(Map *map, int key);

#endif /* MAP_H */

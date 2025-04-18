#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    float porcentaje = (float)map->size / (float)map->capacity;
    if(porcentaje >= 0.7) enlarge(map);
    
    int posicion  = hash(key, map->capacity);
    while(map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL) {
        if(is_equal(map->buckets[posicion]->key, key)) return;
        posicion = (posicion + 1) % map->capacity;
    }

    Pair * nuevo = (Pair *)malloc(sizeof(Pair));
    if(nuevo == NULL) return;
    nuevo->key = key;
    nuevo->value = value;   
    map->buckets[posicion] = nuevo;
    map->size++;
    map->current = posicion;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)

    int capacidadAnterior = map->capacity;
    Pair **buckets = map->buckets;

    map->capacity = map->capacity * 2;
    map->buckets = (Pair **) calloc(map->capacity, sizeof(Pair *));
    map->size = 0;
    map->current = -1;

    for(int i = 0; i < capacidadAnterior ; i++) {
        if(buckets[i] != NULL && buckets[i]->key != NULL) {
            insertMap(map, buckets[i]->key, buckets[i]->value);
        }
    }
    free(buckets);
}


HashMap * createMap(long capacity) {
    if(capacity <= 0) return NULL;
    HashMap *hashmap = (HashMap *)malloc(sizeof(HashMap));
    if(hashmap == NULL) return NULL;

    hashmap->capacity = capacity;
    hashmap->size = 0;
    hashmap->current = -1;

    hashmap->buckets = (Pair **) calloc(capacity, sizeof(Pair *));
    if(hashmap->buckets == NULL) {
        free(hashmap);
        return NULL;
    }

    return hashmap;
}

void eraseMap(HashMap * map,  char * key) {    
int posicion = hash(key, map->capacity);

while(map->buckets[posicion] != NULL){
    if(map->buckets[posicion]->key != NULL && is_equal(map->buckets[posicion]->key, key)){
        map->buckets[posicion]->key = NULL;
        map->size--;
        if(map->current == posicion) map->current = -1;
        return;
    }
    posicion = (posicion + 1) % map->capacity; 
}
}

Pair * searchMap(HashMap * map,  char * key) {   
    int posicion = hash(key, map->capacity);
    while(map->buckets[posicion] != NULL) {
        if(map->buckets[posicion]->key != NULL && is_equal(map->buckets[posicion]->key, key)) {
            map->current = posicion;
            return map->buckets[posicion];
        }
        posicion = (posicion + 1) % map->capacity;
    }
    return NULL;
}

Pair * firstMap(HashMap * map) {
    for(int i = 0; i < map->capacity; i++) {
        if(map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    for(int i = map->current + 1; i < map->capacity; i++) {
        if(map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}

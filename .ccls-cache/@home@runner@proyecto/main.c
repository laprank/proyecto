#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/extra.h"
#include <ctype.h>


typedef struct {
    int id;
    char pregunta[200];
    char dificultad[10];
    char tipo[20];
    char **respuestas;
    int num_respuestas;
} Pregunta;
typedef struct {
    char username[50];
    char password[50];
    int top_score_easy;
    int top_score_medium;
    int top_score_hard;
    int top_score_lista;
    int top_score_random;
} UserProfile;
// Función para eliminar espacios en blanco al principio y al final de una cadena
char *strtrim(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    // All spaces?
    if (*str == 0)
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}
// Función de comparación para qsort
int compare_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

/**
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 */
int is_equal_str(void *key1, void *key2) {
    return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 */
int is_equal_int(void *key1, void *key2) {
    return *(int *)key1 == *(int *)key2;
}


void liberar_pregunta(Pregunta *pregunta) {
    for (int i = 0; i < pregunta->num_respuestas; ++i) {
        free(pregunta->respuestas[i]);
    }
    free(pregunta->respuestas);
    free(pregunta);
}

void cargar_preguntas(Map *mapa_preguntas, Map *mapa_lista, Map *mapa_hard, Map *mapa_easy, Map *mapa_medium) {
    FILE *archivo = fopen("data/protopreguntas.csv", "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    char **campos;
    campos = leer_linea_csv(archivo, ',');
    while (campos != NULL) {
        Pregunta *pregunta = (Pregunta *)malloc(sizeof(Pregunta));
        pregunta->id = atoi(campos[0]);
        strcpy(pregunta->pregunta, strtrim(campos[1]));
        strcpy(pregunta->dificultad, strtrim(campos[3]));
        strcpy(pregunta->tipo, strtrim(campos[4]));

        if (strcmp(pregunta->tipo, "tipo lista") == 0) {
            char *token;
            char *respuesta = strdup(campos[2]);
            token = strtok(respuesta, ";");
            pregunta->num_respuestas = 0;
            pregunta->respuestas = (char **)malloc(sizeof(char *) * 10);

            while (token != NULL) {
                pregunta->respuestas[pregunta->num_respuestas] = strdup(token);
                pregunta->num_respuestas++;
                token = strtok(NULL, ";");
            }

            free(respuesta);
        } else {
            pregunta->num_respuestas = 1;
            pregunta->respuestas = (char **)malloc(sizeof(char *));
            pregunta->respuestas[0] = strdup(campos[2]);
        }

        map_insert(mapa_preguntas, pregunta->id, pregunta);

        if (strcmp(pregunta->dificultad, "hard") == 0) {
            map_insert(mapa_hard, pregunta->id, pregunta);
        } else if (strcmp(pregunta->dificultad, "easy") == 0) {
            map_insert(mapa_easy, pregunta->id, pregunta);
        } else if (strcmp(pregunta->dificultad, "medium") == 0) {
            map_insert(mapa_medium, pregunta->id, pregunta);
        }

        if (strcmp(pregunta->tipo, "tipo lista") == 0) {
            map_insert(mapa_lista, pregunta->id, pregunta);
        }

        campos = leer_linea_csv(archivo, ',');
    }

    fclose(archivo);
}

List *seleccionar_preguntas_azar(Map *mapa_preguntas, int num_preguntas) {
    // Crear la lista de preguntas seleccionadas
    List *lista_preguntas = list_create();

    // Verificar si el mapa tiene suficientes preguntas
    if (mapa_preguntas->size < num_preguntas) {
        printf("El mapa no tiene suficientes preguntas para seleccionar.\n");
        return lista_preguntas;
    }

    // Inicializar el generador de números aleatorios
    srand(time(NULL));

    // Crear un array temporal para almacenar los índices de las preguntas
    int *indices = (int *)malloc(mapa_preguntas->size * sizeof(int));
    for (int i = 0; i < mapa_preguntas->size; i++) {
        indices[i] = i;
    }

    // Barajar el array de índices
    for (int i = mapa_preguntas->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    // Seleccionar las primeras num_preguntas del array barajado
    for (int i = 0; i < num_preguntas; i++) {
        int index = indices[i];
        // Buscar la pregunta en el mapa
        for (int j = 0; j < mapa_preguntas->capac; j++) {
            if (mapa_preguntas->hashArray[j] != NULL && mapa_preguntas->hashArray[j]->key != -1) {
                if (index == 0) {
                    // Insertar la pregunta seleccionada en la lista
                    list_pushBack(lista_preguntas, mapa_preguntas->hashArray[j]->data);
                    break;
                }
                index--;
            }
        }
    }

    // Liberar el array de índices
    free(indices);

    return lista_preguntas;
}
void mostrar_titulo() {
        printf("\n");
        printf("###############################################\n");
        printf("#                                             #\n");
        printf("#              SUPER FÚTBOL QUIZ              #\n");
        printf("#                                             #\n");
        printf("###############################################\n");
    printf("\n");
    sleep(3); // Pausa de 3 segundos
}

int mostrar_menu() {
    int opcion;

    printf("\n################################################\n");
    printf("#                                              #\n");
    printf("#                   MENÚ PRINCIPAL             #\n");
    printf("#                                              #\n");
    printf("################################################\n");
    printf("#                                              #\n");
    printf("#  1. Un jugador                               #\n");
    printf("#  2. Multijugador                             #\n");
    printf("#  3. Configuración                            #\n");
    printf("#  4. Top mejor jugador                        #\n");
    printf("#  5. Ayuda                                    #\n");
    printf("#  6. Salir                                    #\n");
    printf("#                                              #\n");
    printf("################################################\n");

    // Leer la opción ingresada por el usuario
    printf("Ingrese el número de la opción deseada: ");
    scanf("%d", &opcion);

    // Limpiar el buffer del teclado
    while ((getchar()) != '\n');

    return opcion;
}
int mostrar_menu_configuracion() {
    int opcion;

    printf("\n################################################\n");
    printf("#                                              #\n");
    printf("#              MENÚ DE CONFIGURACIÓN           #\n");
    printf("#                                              #\n");
    printf("################################################\n");
    printf("#                                              #\n");
    printf("#  1. Seleccionar cantidad de preguntas        #\n");
    printf("#  2. Volver al menú principal                 #\n");
    printf("#                                              #\n");
    printf("################################################\n");

    printf("Ingrese el número de la opción deseada: ");
    scanf("%d", &opcion);

    while ((getchar()) != '\n'); // Limpiar el buffer del teclado

    return opcion;
}

int cantidad_preguntas_configuracion = 5; // Variable global para almacenar la cantidad de preguntas

int configurar_cantidad_preguntas() {
    int opcion;

    printf("\n################################################\n");
    printf("#                                              #\n");
    printf("#         CONFIGURAR CANTIDAD DE PREGUNTAS     #\n");
    printf("#                                              #\n");
    printf("################################################\n");
    printf("#                                              #\n");
    printf("#  1. 5 preguntas                              #\n");
    printf("#  2. 10 preguntas                             #\n");
    printf("#  3. 20 preguntas                             #\n");
    printf("#  4. Personalizado                            #\n");
    printf("#                                              #\n");
    printf("################################################\n");

    printf("Ingrese el número de la opción deseada: ");
    scanf("%d", &opcion);

    switch (opcion) {
        case 1:
            cantidad_preguntas_configuracion = 5;
            break;
        case 2:
            cantidad_preguntas_configuracion = 10;
            break;
        case 3:
            cantidad_preguntas_configuracion = 20;
            break;
        case 4:
            printf("Ingrese la cantidad personalizada de preguntas: ");
            scanf("%d", &cantidad_preguntas_configuracion);
            break;
        default:
            printf("Opción no válida. Se mantiene la configuración actual (%d preguntas).\n", cantidad_preguntas_configuracion);
            break;
    }

    while ((getchar()) != '\n'); // Limpiar el buffer del teclado

    printf("Configuración guardada: %d preguntas.\n", cantidad_preguntas_configuracion);
    return cantidad_preguntas_configuracion;
}

int mostrar_menu_un_jugador() {
    int opcion;

    printf("\n################################################\n");
    printf("#                                              #\n");
    printf("#          SELECCIÓN DE JUEGO UN JUGADOR       #\n");
    printf("#                                              #\n");
    printf("################################################\n");
    printf("#                                              #\n");
    printf("#  1. Seleccionar dificultad                   #\n");
    printf("#  2. Volver al menú principal                 #\n");
    printf("#                                              #\n");
    printf("################################################\n");

    // Leer la opción ingresada por el usuario
    printf("Ingrese el número de la opción deseada: ");
    scanf("%d", &opcion);

    // Limpiar el buffer del teclado
    while ((getchar()) != '\n');

    return opcion;
}

List* seleccinar_dificultad(Map *mapa_hard, Map *mapa_medium, Map *mapa_easy, Map *mapa_lista, Map *mapa_preguntas,int num_preguntas){
    printf("\n################################################\n");
    printf("#                                              #\n");
    printf("#             SELECCIÓN DE DIFICULTAD          #\n");
    printf("#                                              #\n");
    printf("################################################\n");
    printf("#                                              #\n");
    printf("#  1. Fácil                                    #\n");
    printf("#  2. Medio                                    #\n");
    printf("#  3. Difícil                                  #\n");
    printf("#  4. Lista                                    #\n");
    printf("#  5. Aleatorea                                #\n");
    printf("#                                              #\n");
    printf("################################################\n");
    int opcion;
    printf("Ingrese el número de la opción deseada: ");
    scanf("%d", &opcion);
    while ((getchar()) != '\n');
    List *lista_preguntas = NULL;
    switch (opcion){
        case 1:
            lista_preguntas = seleccionar_preguntas_azar(mapa_easy, num_preguntas);
            break;
        case 2:
            lista_preguntas = seleccionar_preguntas_azar(mapa_medium, num_preguntas);
            break;
        case 3:
            lista_preguntas = seleccionar_preguntas_azar(mapa_hard, num_preguntas);
            break;
        case 4:
            lista_preguntas = seleccionar_preguntas_azar(mapa_lista, num_preguntas);
            break;
        case 5:
            lista_preguntas = seleccionar_preguntas_azar(mapa_preguntas, num_preguntas);
        default:
            printf("Opción inválida. Seleccionando preguntas aleatorias.\n");
            lista_preguntas = seleccionar_preguntas_azar(mapa_easy, num_preguntas);
            break;

    }
    return lista_preguntas;
}
void imprimir_preguntas(List *lista) {
    Pregunta *pregunta = list_first(lista);
    int contador = 1;
    while (pregunta != NULL) {
        printf("Pregunta %d:\n", contador);
        printf("ID: %d\n", pregunta->id);
        printf("Pregunta: %s\n", pregunta->pregunta);
        printf("Dificultad: %s\n", pregunta->dificultad);
        printf("Tipo: %s\n", pregunta->tipo);
        printf("Respuestas: ");
        for (int i = 0; i < pregunta->num_respuestas; i++) {
            printf("%s", pregunta->respuestas[i]);
            if (i < pregunta->num_respuestas - 1) {
                printf(", ");
            }
        }
        printf("\n\n");
        pregunta = list_next(lista);
        contador++;
    }
    printf("Total preguntas seleccionadas: %d\n", contador - 1);
}
void responder_preguntas(List *lista_preguntas){
    int puntaje = 0;
    int opcion;
    Pregunta *pregunta = list_first(lista_preguntas);
    while(pregunta != NULL){
        printf("Pregunta %s:\n", pregunta->pregunta);
        if(lista_preguntas->current->prev == NULL){
            printf("2. siguente\n");
            printf("3. responder\n");
        }
        else if(lista_preguntas->current->next == NULL){
            printf("1. anterior\n");
            printf("3. responder\n");
        }
        else{
            printf("1. anterior\n");
            printf("2. siguiente\n");
            printf("3. responder\n");
        }
        printf("Ingrese su opción: ");
        scanf("%d", &opcion);
        while ((getchar()) != '\n');
        switch(opcion){
            case 1:
                if(lista_preguntas->current->prev != NULL){
                    lista_preguntas->current = lista_preguntas->current->prev;
                    pregunta = lista_preguntas->current->data;
                }
                break;
            case 2:
                if(lista_preguntas->current->next != NULL){
                    lista_preguntas->current = lista_preguntas->current->next;
                    pregunta = lista_preguntas->current->data;
                }
                break;
            case 3:
                if(strcmp(((Pregunta*)lista_preguntas->current->data)->tipo, "tipo lista") == 0){
                    printf("Ingrese la respuesta separada por ';': ");
                    for (int i = 0; i < pregunta->num_respuestas; i++) {
                        printf("%s", pregunta->respuestas[i]);
                        if (i < pregunta->num_respuestas - 1) {
                            printf(";");
                        }
                    }
                    printf("\n");
                    char respuesta[100];
                    fgets(respuesta, sizeof(respuesta), stdin);
                    respuesta[strcspn(respuesta, "\n")] = '\0';
                    char*respuestas[pregunta->num_respuestas];
                    int num_respuestas = 0;
                    char*token = strtok(respuesta, ";");
                    while(token != NULL){
                        respuestas[num_respuestas] = strdup(token);
                        num_respuestas++;
                        token = strtok(NULL, ";");
                    }
                    int n_res_valid = pregunta->num_respuestas;
                    int n_res_correctas = 0;
                    for(int i = 0; i < n_res_valid; i++){
                        if (strcmp(pregunta->respuestas[i], respuestas[i]) == 0) {
                            n_res_correctas++;
                        }
                    }
                    puntaje += n_res_correctas;
                    printf("Respuestas correctas: %d\n", n_res_correctas);
                }
                else{
                    printf("Ingrese su respuesta: \n");
                    printf("%s,\n", ((Pregunta*)lista_preguntas->current->data)->tipo);
                    char respuesta[100];
                    fgets(respuesta, sizeof(respuesta), stdin);
                    respuesta[strcspn(respuesta, "\n")] = '\0';
                    if(strcmp(pregunta->respuestas[0], respuesta) == 0){
                        printf("Respuesta correcta\n");
                        puntaje++;
                    }
                    else{
                        printf("Respuesta incorrecta\n");
                    }
                }
                break;
            default:
                printf("Opción inválida. Seleccionando la siguiente pregunta.\n");
                break;
        }
    }
}
void responder_preguntas_versus(List *lista_preguntas, char *nombre_jugador1, char *nombre_jugador2) {
    int opcion;
    Pregunta *pregunta = list_first(lista_preguntas);
    int turno = 1; // 1 para jugador 1, 2 para jugador 2
    while (pregunta != NULL) {
        if (turno == 1) {
            printf("%s, es tu turno.\n", nombre_jugador1);
        } else {
            printf("%s, es tu turno.\n", nombre_jugador2);
        }

        printf("Pregunta %s:\n", pregunta->pregunta);
        if (lista_preguntas->current->prev == NULL) {
            printf("2. siguiente\n");
            printf("3. responder\n");
        } else if (lista_preguntas->current->next == NULL) {
            printf("1. anterior\n");
            printf("3. responder\n");
        } else {
            printf("1. anterior\n");
            printf("2. siguiente\n");
            printf("3. responder\n");
        }
        printf("Ingrese su opción: ");
        scanf("%d", &opcion);
        while ((getchar()) != '\n');
        switch (opcion) {
            case 1:
                lista_preguntas->current = lista_preguntas->current->prev;
                pregunta = lista_preguntas->current->data;
                break;
            case 2:
                lista_preguntas->current = lista_preguntas->current->next;
                pregunta = lista_preguntas->current->data;
                break;
            case 3:
                if (strcmp(pregunta->tipo, "tipo lista") == 0) {
                    printf("Ingrese la respuesta separada por ';': ");
                    for (int i = 0; i < pregunta->num_respuestas; i++) {
                        printf("%s", pregunta->respuestas[i]);
                        if (i < pregunta->num_respuestas - 1) {
                            printf(";");
                        }
                    }
                    printf("\n");
                    char respuesta[100];
                    fgets(respuesta, sizeof(respuesta), stdin);
                    respuesta[strcspn(respuesta, "\n")] = '\0';
                    char **respuestas = NULL;
                    int num_respuestas = 0;
                    char *token = strtok(respuesta, ";");
                    while (token != NULL) {
                        respuestas = (char **)realloc(respuestas, (num_respuestas + 1) * sizeof(char *));
                        respuestas[num_respuestas] = strdup(token);
                        num_respuestas++;
                        token = strtok(NULL, ";");
                    }
                    int n_res_valid = pregunta->num_respuestas;
                    int n_res_correctas = 0;
                    for (int i = 0; i < n_res_valid; i++) {
                        if (strcmp(pregunta->respuestas[i], respuestas[i]) == 0) {
                            n_res_correctas++;
                        }
                    }
                    printf("Respuestas correctas: %d\n", n_res_correctas);
                    for (int i = 0; i < num_respuestas; i++) {
                        free(respuestas[i]);
                    }
                    free(respuestas);
                } else {
                    printf("Ingrese su respuesta: \n");
                    printf("%s,\n", pregunta->tipo);
                    char respuesta[100];
                    fgets(respuesta, sizeof(respuesta), stdin);
                    respuesta[strcspn(respuesta, "\n")] = '\0';
                    if (strcmp(pregunta->respuestas[0], respuesta) == 0) {
                        printf("Respuesta correcta\n");
                    } else {
                        printf("Respuesta incorrecta\n");
                    }
                }
                break;
            default:
                printf("Opción no válida. Inténtelo de nuevo.\n");
                break;
        }
        // Cambiar turno
        turno = (turno == 1) ? 2 : 1;
    }
}

int main(void) {
    mostrar_titulo();
    Map *mapa_preguntas = map_create();
    Map *mapa_lista = map_create();
    Map *mapa_hard = map_create();
    Map *mapa_medium = map_create();
    Map *mapa_easy = map_create();
    int num_preguntas = 10;

    cargar_preguntas(mapa_preguntas, mapa_lista, mapa_hard, mapa_easy, mapa_medium);

    int opcion;
    do {
        opcion = mostrar_menu();

        switch (opcion) {
            case 1:
                printf("Ingrese su nombre\n");
                char nombre[100];
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = '\0';
                printf("Bienvenido %s\n", nombre);
                List *lista_preguntas = seleccinar_dificultad(mapa_hard, mapa_medium, mapa_easy, mapa_lista, mapa_preguntas, num_preguntas);
                    responder_preguntas(lista_preguntas);
                break;
            case 2:
                printf("Modo Versus seleccionado.\n");
                char nombre_jugador1[100];
                char nombre_jugador2[100];
                printf("Ingrese el nombre del jugador 1: ");
                fgets(nombre_jugador1, sizeof(nombre_jugador1), stdin);
                nombre_jugador1[strcspn(nombre_jugador1, "\n")] = '\0';
                printf("Ingrese el nombre del jugador 2: ");
                fgets(nombre_jugador2, sizeof(nombre_jugador2), stdin);
                nombre_jugador2[strcspn(nombre_jugador2, "\n")] = '\0';
                List *lista_preguntas_versus = seleccinar_dificultad(mapa_hard, mapa_medium, mapa_easy, mapa_lista, mapa_preguntas, num_preguntas);
                responder_preguntas_versus(lista_preguntas_versus, nombre_jugador1, nombre_jugador2);
                break;
            case 3:
                printf("Configuración seleccionada.\n");
                int opcion_config;
                do {
                    opcion_config = mostrar_menu_configuracion();
                    switch (opcion_config) {
                        case 1:
                            configurar_cantidad_preguntas();
                            break;
                        case 2:
                            printf("Volviendo al menú principal...\n");
                            break;
                        default:
                            printf("Opción no válida. Inténtalo de nuevo.\n");
                            break;
                    }
                } while (opcion_config != 2);
                break;
            case 4:
                printf("Top jugadores\n");
                printf("\n################################################\n");
                printf("#                                              #\n");
                printf("#                   TOP MEDIO                  #\n");
                printf("#                                              #\n");
                printf("################################################\n");
                printf("#                                              #\n");
                printf("#  1. Judador 1                                #\n");
                printf("#  2. Jugador 2                                #\n");
                printf("#  3. Jugador 3                                #\n");
                printf("#                                              #\n");
                printf("################################################\n");
            case 5:
                printf("has seleccionado ayuda\n");
                printf("\n################################################\n");
                printf("#                                              #\n");
                printf("#                   MENÚ AYUDA                 #\n");
                printf("#                                              #\n");
                printf("################################################\n");
                printf("#                                              #\n");
                printf("#  1. instrucciones                            #\n");
                printf("#  2. contacto servicio tecnico                #\n");
                printf("#                                              #\n");
                printf("################################################\n");
                break;
            case 6:
                printf("Saliendo...\n");
            default:
                printf("Opción no válida. Inténtalo de nuevo.\n");
                break;
        }
    } while (opcion != 4);

    return 0;
}
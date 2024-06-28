#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/extra.h"
#include <ctype.h>
#define MAX_NAME_LEN 50
#define TOP_SIZE 3


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
    int top_score_easy;
    int top_score_medium;
    int top_score_hard;
    int top_score_lista;
} Jugador;
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
void convertirCadenaAMayuscula(char* cadena) {
    int i = 0;
    while (cadena[i] != '\0') {
        // Convertir letras sin acento a mayúsculas
        cadena[i] = toupper(cadena[i]);

        // Manejar letras con acento
        /*switch (cadena[i]) {
            case 'á': cadena[i] = 'Á'; break;
            case 'é': cadena[i] = 'É'; break;
            case 'í': cadena[i] = 'Í'; break;
            case 'ó': cadena[i] = 'Ó'; break;
            case 'ú': cadena[i] = 'Ú'; break;
            case 'ñ': cadena[i] = 'Ñ'; break;
            case 'ü': cadena[i] = 'Ü'; break;
            // Si ya está en mayúscula, no hace falta cambiar
            case 'Á':
            case 'É':
            case 'Í':
            case 'Ó':
            case 'Ú':
            case 'Ñ':
            case 'Ü':
                break;
            default:
                cadena[i] = toupper(cadena[i]);
        }*/

        i++;
    }
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
void leerCSV(Jugador top3[TOP_SIZE], const char* filename, int dificultad) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TOP_SIZE; i++) {
        fscanf(file, "%[^,],", top3[i].username);
        switch (dificultad) {
            case 0: fscanf(file, "%d\n", &top3[i].top_score_easy); break;
            case 1: fscanf(file, "%d\n", &top3[i].top_score_medium);printf("%d",top3[i].top_score_medium);break;
            case 2: fscanf(file, "%d\n", &top3[i].top_score_hard); break;
            case 3: fscanf(file, "%d\n", &top3[i].top_score_lista); break;
        }
    }

    fclose(file);
}

void actualizarTop3(Jugador top3[], const char* username, int score, int dificultad) {
    for (int i = 0; i < TOP_SIZE; i++) {
        int current_score;
        switch (dificultad) {
            case 0: current_score = top3[i].top_score_easy; break;
            case 1: current_score = top3[i].top_score_medium; break;
            case 2: current_score = top3[i].top_score_hard; break;
            case 3: current_score = top3[i].top_score_lista; break;
        }
        if (score > current_score) {
            for (int j = TOP_SIZE - 1; j > i; j--) {
                top3[j] = top3[j - 1];
            }
            strncpy(top3[i].username, username, MAX_NAME_LEN - 1);
            top3[i].username[MAX_NAME_LEN - 1] = '\0';
            switch (dificultad) {
                case 0: top3[i].top_score_easy = score; break;
                case 1: top3[i].top_score_medium = score; break;
                case 2: top3[i].top_score_hard = score; break;
                case 3: top3[i].top_score_lista = score; break;
            }
            break;
        }
    }
}

void guardarCSV(Jugador top3[], const char* filename, int dificultad) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error al abrir el archivo para escribir");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TOP_SIZE; i++) {
        switch (dificultad) {
            case 0: fprintf(file, "%s,%d\n", top3[i].username, top3[i].top_score_easy); break;
            case 1: fprintf(file, "%s,%d\n", top3[i].username, top3[i].top_score_medium); break;
            case 2: fprintf(file, "%s,%d\n", top3[i].username, top3[i].top_score_hard); break;
            case 3: fprintf(file, "%s,%d\n", top3[i].username, top3[i].top_score_lista); break;
        }
    }

    fclose(file);
}

void imprimirTop3(Jugador top3[], int dificultad) {
    const char* dificultades[] = {"Fácil", "Media", "Difícil", "Lista"};
    printf("Top 3 Jugadores - Dificultad %s:\n", dificultades[dificultad]);
    for (int i = 0; i < TOP_SIZE; i++) {
        int score;
        switch (dificultad) {
            case 0: score = top3[i].top_score_easy; break;
            case 1: score = top3[i].top_score_medium; break;
            case 2: score = top3[i].top_score_hard; break;
            case 3: score = top3[i].top_score_lista; break;
        }
        printf("%d. %s - %d\n", i + 1, top3[i].username, score);
    }
    printf("\n");
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
                convertirCadenaAMayuscula(token);
                pregunta->respuestas[pregunta->num_respuestas] = strdup(token);
                pregunta->num_respuestas++;
                token = strtok(NULL, ";");
            }

            free(respuesta);
        } else {
            pregunta->num_respuestas = 1;
            pregunta->respuestas = (char **)malloc(sizeof(char *));
            convertirCadenaAMayuscula(campos[2]);
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

    int preguntas_seleccionadas = 0;
    while (preguntas_seleccionadas < num_preguntas) {
        // Generar un índice aleatorio dentro del tamaño de la tabla hash
        int indice = rand() % mapa_preguntas->capac;

        // Verificar si hay una pregunta en esa posición
        if (mapa_preguntas->hashArray[indice] != NULL && mapa_preguntas->hashArray[indice]->key != -1) {
            // Insertar la pregunta en la lista de preguntas seleccionadas
            list_pushBack(lista_preguntas, mapa_preguntas->hashArray[indice]->data);
            preguntas_seleccionadas++;
        }
    }

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

int seleccinar_dificultad(){
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
    return opcion;
}
List *generar_lista(Map *mapa_hard, Map *mapa_medium, Map *mapa_easy, Map *mapa_lista, Map *mapa_preguntas,int num_preguntas,int opcion){
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
int responder_preguntas(List *lista_preguntas){
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
                    list_popCurrent(lista_preguntas);
                    if(lista_preguntas->current!=NULL){
                        pregunta = lista_preguntas->current->data;
                    }
                    else{
                        pregunta = NULL;
                        printf("Puntaje total: %d\n", puntaje);
                        printf("precione enter para continuar");
                        while ((getchar()) != '\n');
                    }
                }
                else{
                    printf("Ingrese su respuesta: \n");
                    printf("%s,\n", ((Pregunta*)lista_preguntas->current->data)->tipo);
                    char respuesta[100];

                    fgets(respuesta, sizeof(respuesta), stdin);
                    convertirCadenaAMayuscula(respuesta);
                    respuesta[strcspn(respuesta, "\n")] = '\0';
                    if(strcmp(pregunta->respuestas[0], respuesta) == 0){
                        printf("Respuesta correcta\n");
                        puntaje++;
                        list_popCurrent(lista_preguntas);
                        if(lista_preguntas->current != NULL){
                            pregunta = lista_preguntas->current->data;
                        }
                        else{
                            pregunta = NULL;
                        }
                    }
                    else{
                        printf("Respuesta incorrecta\n");
                        printf("La respuesta correcta es: %s\n", pregunta->respuestas[0]);
                        list_popCurrent(lista_preguntas);
                        if(lista_preguntas->current != NULL){
                            pregunta = lista_preguntas->current->data;
                        }
                        else{
                            pregunta = NULL;
                        }


                    }
                }
                break;
            default:
                printf("Opción inválida. Seleccionando la siguiente pregunta.\n");
                break;
        }
    }
    return puntaje;
}
void responder_preguntas_versus(List *lista_preguntas, char *nombre_jugador1, char *nombre_jugador2) {
    int puntaje_jugador1 = 0;
    int puntaje_jugador2 = 0;
    int opcion;
    Pregunta *pregunta = list_first(lista_preguntas);
    int turno = 1; // 1 para jugador 1, 2 para jugador 2

    while (pregunta != NULL) {
        if (turno == 1) {
            printf("%s, es tu turno.\n", nombre_jugador1);
        } else {
            printf("%s, es tu turno.\n", nombre_jugador2);
        }

        printf("Pregunta: %s\n", pregunta->pregunta);

        if (lista_preguntas->current->prev == NULL) {
            printf("2. Siguiente\n");
            printf("3. Responder\n");
        } else if (lista_preguntas->current->next == NULL) {
            printf("1. Anterior\n");
            printf("3. Responder\n");
        } else {
            printf("1. Anterior\n");
            printf("2. Siguiente\n");
            printf("3. Responder\n");
        }
        printf("Ingrese su opción: ");
        scanf("%d", &opcion);
        while ((getchar()) != '\n'); // Limpiar el buffer de entrada

        switch (opcion) {
            case 1:
                if (lista_preguntas->current->prev != NULL) {
                    lista_preguntas->current = lista_preguntas->current->prev;
                    pregunta = lista_preguntas->current->data;
                }
                break;
            case 2:
                if (lista_preguntas->current->next != NULL) {
                    lista_preguntas->current = lista_preguntas->current->next;
                    pregunta = lista_preguntas->current->data;
                }
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

                    char *respuestas[pregunta->num_respuestas];
                    int num_respuestas = 0;
                    char *token = strtok(respuesta, ";");
                    while (token != NULL) {
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
                    if (turno == 1) {
                        puntaje_jugador1 += n_res_correctas;
                    } else {
                        puntaje_jugador2 += n_res_correctas;
                    }
                    printf("Respuestas correctas: %d\n", n_res_correctas);
                } else {
                    printf("Ingrese su respuesta: ");
                    char respuesta[100];
                    fgets(respuesta, sizeof(respuesta), stdin);
                    respuesta[strcspn(respuesta, "\n")] = '\0';

                    if (strcmp(pregunta->respuestas[0], respuesta) == 0) {
                        printf("Respuesta correcta\n");
                        if (turno == 1) {
                            puntaje_jugador1++;
                        } else {
                            puntaje_jugador2++;
                        }
                    } else {
                        printf("Respuesta incorrecta\n");
                    }
                }

                list_popCurrent(lista_preguntas);
                pregunta = lista_preguntas->current != NULL ? lista_preguntas->current->data : NULL;
                break;
            default:
                printf("Opción no válida. Inténtelo de nuevo.\n");
                break;
        }

        // Cambiar turno
        turno = (turno == 1) ? 2 : 1;
    }

    // Imprimir puntajes finales y felicitar al ganador
    printf("Puntaje final:\n");
    printf("%s: %d\n", nombre_jugador1, puntaje_jugador1);
    printf("%s: %d\n", nombre_jugador2, puntaje_jugador2);

    if (puntaje_jugador1 > puntaje_jugador2) {
        printf("¡Felicidades %s, has ganado!\n", nombre_jugador1);
    } else if (puntaje_jugador2 > puntaje_jugador1) {
        printf("¡Felicidades %s, has ganado!\n", nombre_jugador2);
    } else {
        printf("¡Es un empate!\n");
    }
    printf("precione enter para continuar");
    while ((getchar()) != '\n');
}
void revisar_puntos(int dificultad, int puntaje, char nombre[100]){
    Jugador jugador[TOP_SIZE]={0,0,0};
    switch(dificultad){
        case 1:
            leerCSV(jugador, "data/top3_easy.csv", dificultad-1);
            actualizarTop3(jugador,nombre, puntaje,(dificultad-1));
            guardarCSV(jugador,"data/top3_easy.csv",dificultad-1);
            break;
        case 2:
            leerCSV(jugador, "data/top3_medium.csv", dificultad-1);
            actualizarTop3(jugador,nombre, puntaje,(dificultad-1));
            guardarCSV(jugador,"data/top3_medium.csv",dificultad-1);
            break;
        case 3:
            leerCSV(jugador, "data/top3_hard.csv", dificultad-1);
            actualizarTop3(jugador,nombre, puntaje,(dificultad-1));
            guardarCSV(jugador,"data/top3_hard.csv",dificultad-1);
            break;
        case 4:
            leerCSV(jugador, "data/top3_lista.csv", dificultad-1);
            actualizarTop3(jugador,nombre, puntaje,(dificultad-1));
            guardarCSV(jugador,"data/top3_lista.csv",dificultad-1);
            break;
        default:
            printf("Dificultad no válida.\n");
            break;
    }
}
// Función que imprime el tutorial del programa
void imprimirTutorial() {
    printf("Bienvenido al Juego de Preguntas y Respuestas\n\n");
    printf("Este programa te permite jugar a un juego de preguntas y respuestas con diferentes modos y opciones.\n");
    printf("A continuación, te presentamos un tutorial sobre cómo utilizar el programa:\n\n");

    printf("1. Menú Principal:\n");
    printf("   Al ejecutar el programa, verás un menú con las siguientes opciones:\n");
    printf("   1. Modo Individual: Juega solo respondiendo preguntas y registra tu puntaje.\n");
    printf("   2. Modo Versus: Compite con otro jugador respondiendo preguntas alternadamente.\n");
    printf("   3. Configuración: Configura la cantidad de preguntas a responder en cada partida.\n");
    printf("   4. Tabla de Líderes: Muestra los puntajes más altos por cada dificultad.\n");
    printf("   5. Ayuda: Proporciona instrucciones y contacto para soporte técnico.\n");
    printf("   6. Salir: Cierra el programa.\n\n");

    printf("2. Modo Individual:\n");
    printf("   - Selecciona esta opción para jugar solo.\n");
    printf("   - El programa te pedirá tu nombre y la dificultad deseada.\n");
    printf("   - Responde las preguntas presentadas y acumula puntos.\n");
    printf("   - Al finalizar, tu puntaje se registrará si está entre los más altos.\n\n");

    printf("3. Modo Versus:\n");
    printf("   - Selecciona esta opción para jugar contra otro jugador.\n");
    printf("   - El programa pedirá los nombres de ambos jugadores y la dificultad deseada.\n");
    printf("   - Los jugadores responderán preguntas alternadamente.\n");
    printf("   - Al finalizar, se mostrará el puntaje de cada jugador y el ganador.\n\n");

    printf("4. Configuración:\n");
    printf("   - Selecciona esta opción para configurar la cantidad de preguntas a responder.\n");
    printf("   - Elige la cantidad de preguntas deseadas y guarda la configuración.\n\n");

    printf("5. Tabla de Líderes:\n");
    printf("   - Selecciona esta opción para ver los puntajes más altos por cada dificultad.\n\n");

    printf("6. Ayuda:\n");
    printf("   - Selecciona esta opción para ver las instrucciones y detalles de contacto para soporte técnico.\n\n");

    printf("7. Salir:\n");
    printf("   - Selecciona esta opción para cerrar el programa.\n\n");

    printf("Esperamos que disfrutes del juego. ¡Buena suerte!\n");
}

int main(void) {
    mostrar_titulo();
    Map *mapa_preguntas = map_create();
    Map *mapa_lista = map_create();
    Map *mapa_hard = map_create();
    Map *mapa_medium = map_create();
    Map *mapa_easy = map_create();
    int num_preguntas = 4;
    int opcion = 0;

    cargar_preguntas(mapa_preguntas, mapa_lista, mapa_hard, mapa_easy, mapa_medium);

    do {
        opcion = mostrar_menu();

        switch (opcion) {
            case 1:
                printf("Ingrese su nombre\n");
                char nombre[100];
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = '\0';
                printf("Bienvenido %s\n", nombre);
                opcion = seleccinar_dificultad();
                List *lista_preguntas = generar_lista(mapa_hard, mapa_medium, mapa_easy, mapa_lista, mapa_preguntas, num_preguntas, opcion);
                int puntaje = responder_preguntas(lista_preguntas);
                if(opcion<5){
                    revisar_puntos(opcion, puntaje, nombre);
                }

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
                opcion = seleccinar_dificultad();
                List *lista_preguntas_versus = generar_lista(mapa_hard, mapa_medium, mapa_easy, mapa_lista, mapa_preguntas, num_preguntas, opcion);
                responder_preguntas_versus(lista_preguntas_versus, nombre_jugador1, nombre_jugador2);
                break;
            case 3:
                printf("Configuración seleccionada.\n");
                int opcion_config;
                do {
                    opcion_config = mostrar_menu_configuracion();
                    switch (opcion_config) {
                        case 1:
                           num_preguntas = configurar_cantidad_preguntas();
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
                {

printf("\n################################################\n");
            printf("#                                              #\n");
            printf("#             SELECCIÓN DE DIFICULTAD          #\n");
            printf("#                                              #\n");
            printf("################################################\n");
            printf("#                                              #\n");
            printf("#  0. Fácil                                    #\n");
            printf("#  1. Medio                                    #\n");
            printf("#  2. Difícil                                  #\n");
            printf("#  3. Lista                                    #\n");
            printf("#                                              #\n");
            printf("################################################\n");
            printf("Ingrese su opción: ");
            int opcion_dificultad;
            scanf("%d", &opcion_dificultad);
            while ((getchar()) != '\n');
            switch (opcion_dificultad){
                case 0:
                    {
                        Jugador top3[TOP_SIZE];
                        leerCSV(top3, "data/top3_easy.csv", 0);
                        imprimirTop3(top3, 0);
                        printf("precione cualquier tecla para continuar\n");
                        getchar();
                    }
                    break;
                case 1:
                    {
                        Jugador top3[TOP_SIZE];
                        leerCSV(top3, "data/top3_medium.csv", 1);
                        imprimirTop3(top3, 1);
                        printf("precione cualquier tecla para continuar\n");
                        getchar();
                    }
                    break;
                case 2:
                    {
                        Jugador top3[TOP_SIZE];
                        leerCSV(top3, "data/top3_hard.csv", 2);
                        imprimirTop3(top3, 2);
                        printf("precione cualquier tecla para continuar\n");
                        getchar();
                    }
                    break;
                case 3:
                    {
                        Jugador top3[TOP_SIZE];
                        leerCSV(top3, "data/top3_lista.csv", 3);
                        imprimirTop3(top3, 3);
                        printf("precone cualquier tecla para continuar\n");
                        getchar();
                    }
                    break;
                default:
                    printf("Opción no válida. Inténtalo de nuevo.\n");
                    break;
                }
            }
            break;
            case 5:
                printf("has seleccionado ayuda\n");
                printf("\n################################################\n");
                printf("#                                              #\n");
                printf("#                   MENÚ AYUDA                 #\n");
                printf("#                                              #\n");
                printf("################################################\n");
                printf("#                                              #\n");
                printf("#  1. Instrucciones                            #\n");
                printf("#  2. Contacto Servicio Tecnico                #\n");
                printf("#  3. Volver al Menú Principal \n");
                printf("#                                              #\n");
                printf("################################################\n");
                printf("Ingrese el número de la opción deseada:");
                int opcion_ayuda;
                scanf("%d", &opcion_ayuda);
                while ((getchar()) != '\n');
                switch (opcion_ayuda){
                case 1:
                    printf("Instrucciones:\n");
                    printf("\n");
                    imprimirTutorial();
                    printf("\nprecione cualquier tecla para continuar\n");
                    int opcion_instrucciones;
                    scanf("%d", &opcion_instrucciones);
                    while ((getchar()) != '\n');
                    break;
                case 2:
                    printf("Contacto al servicio tecnico:\n");
                    printf("\n");
                    printf("Envianos un correo a : FutTest2024@gmail.com y te responderemos lo antes posible\n");
                    printf("\n");
                    printf("Para volver al Menú Principal Presiona Cualquier Número o Letra\n");
                    int opcion_contacto;
                    scanf("%d", &opcion_contacto);
                    while ((getchar()) != '\n');
                    break;
                case 3:
                    printf("Volviendo al menú principal...\n");    
                }

            case 6:
                printf("Saliendo...\n");
        }
    } while (opcion != 6);

    return 0;
}
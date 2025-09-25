#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dia {
    int dia, mes, anio;
    int* asistencia;
    struct Dia* sig;
} Dia;

typedef struct {
    char nombre[50];
} Alumno;


void eliminarAlumno(Alumno** alumnos, int* numAlumnos, int indice) {
    if (indice < 0 || indice >= *numAlumnos) {
        printf("\220ndice inv\240lido.\n"); 
        return;
    }
    for (int i = indice; i < *numAlumnos - 1; i++) {
        (*alumnos)[i] = (*alumnos)[i + 1];
    }
    *numAlumnos -= 1;
    *alumnos = (Alumno*)realloc(*alumnos, sizeof(Alumno) * (*numAlumnos));
    
    if (*numAlumnos > 0 && !*alumnos) {
        printf("Error: memoria insuficiente\n");
        exit(1);
    }
}


void ordenarAlumnos(Alumno* alumnos, int numAlumnos) {
    for (int i = 0; i < numAlumnos - 1; i++) {
        for (int j = 0; j < numAlumnos - i - 1; j++) {
            if (strcmp(alumnos[j].nombre, alumnos[j + 1].nombre) > 0) {
                Alumno temp = alumnos[j];
                alumnos[j] = alumnos[j + 1];
                alumnos[j + 1] = temp;
            }
        }
    }
}


void mostrarAlumnos(Alumno* alumnos, int numAlumnos) {
    printf("\n--- Lista de Alumnos ---\n");
    for (int i = 0; i < numAlumnos; i++) {
        printf("%d. %s", i + 1, alumnos[i].nombre);
    }
    printf("-----------------------\n");
}

void insertarDia(Dia** listaDias, int dia, int mes, int anio, int numAlumnos) {
    Dia* nuevo = (Dia*)malloc(sizeof(Dia));
    if (!nuevo) {
        printf("Error: memoria insuficiente\n");
        exit(1);
    }
    nuevo->dia = dia;
    nuevo->mes = mes;
    nuevo->anio = anio;
    nuevo->asistencia = (int*)calloc(numAlumnos, sizeof(int));
    nuevo->sig = NULL;
    
    if (*listaDias == NULL) {
        *listaDias = nuevo;
    } else {
        Dia* temp = *listaDias;
        while (temp->sig != NULL)
            temp = temp->sig;
        temp->sig = nuevo;
    }
}

void mostrarAsistencia(Dia* listaDias, Alumno* alumnos, int numAlumnos) {
    Dia* diaActual = listaDias;
    while (diaActual != NULL) {
        printf("Fecha: %02d/%02d/%04d\n", diaActual->dia, diaActual->mes, diaActual->anio);
        for (int i = 0; i < numAlumnos; i++) {
            printf("  %s: %s\n",
                    alumnos[i].nombre,
                    diaActual->asistencia[i] ? "Presente" : "Ausente");
        }
        printf("\n");
        diaActual = diaActual->sig;
    }
}

void liberarDias(Dia* listaDias) {
    while (listaDias != NULL) {
        Dia* temp = listaDias;
        listaDias = listaDias->sig;
        free(temp->asistencia);
        free(temp);
    }
}

int main() {
    int numAlumnos;
    printf("Ingrese n\243mero de alumnos: ");
    scanf("%d", &numAlumnos);
    getchar();
    
    Alumno* alumnos = (Alumno*)malloc(sizeof(Alumno) * numAlumnos);
    if (!alumnos) {
        printf("Error: memoria insuficiente\n");
        return 1;
    }
    
    for (int i = 0; i < numAlumnos; i++) {
        printf("Nombre del alumno %d: ", i + 1);
        fgets(alumnos[i].nombre, sizeof(alumnos[i].nombre), stdin);
    }
    
    Dia* listaDias = NULL;
    int opcion;
    
    do {
        printf("\n--- Men\243 ---\n"); 
        printf("1. Tomar lista\n");
        printf("2. Mostrar asistencia\n");
        printf("3. Mostrar todos los alumnos\n");
        printf("4. Ordenar lista alfab\202ticamente\n"); 
        printf("5. Eliminar alumno\n");
        printf("6. Salir\n");
        printf("Seleccione una opci\242n: "); 
        scanf("%d", &opcion);
        getchar();
        
        switch (opcion) {
            case 1: {
                int dia, mes, anio;
                printf("\nIngrese la fecha:\n");
                printf("D\241a: "); 
                scanf("%d", &dia);
                printf("Mes: ");
                scanf("%d", &mes);
                printf("A\244o: "); 
                scanf("%d", &anio);
                getchar();
                
                insertarDia(&listaDias, dia, mes, anio, numAlumnos);
                
                Dia* diaActual = listaDias;
                while (diaActual->sig != NULL)
                    diaActual = diaActual->sig;
                
                printf("\nRegistro de asistencia para el %02d/%02d/%04d:\n", dia, mes, anio);
                for (int i = 0; i < numAlumnos; i++) {
                    int respuesta;
                    do {
                        printf("\250%s asisti\242? (1=Presente, 0=Ausente): ",
                                alumnos[i].nombre);
                        scanf("%d", &respuesta);
                    } while (respuesta != 0 && respuesta != 1);
                    
                    diaActual->asistencia[i] = respuesta;
                }
                break;
            }
            case 2:
                if (listaDias == NULL) {
                    printf("\nNo hay registros de asistencia a\243n.\n"); 
                } else {
                    printf("\n--- Lista completa de asistencia ---\n");
                    mostrarAsistencia(listaDias, alumnos, numAlumnos);
                }
                break;
            case 3:
                mostrarAlumnos(alumnos, numAlumnos);
                break;
            case 4:
                ordenarAlumnos(alumnos, numAlumnos);
                printf("\nLista ordenada alfab\202ticamente.\n");
                break;
            case 5: {
                int indice;
                mostrarAlumnos(alumnos, numAlumnos);
                printf("Ingrese el n\243mero del alumno a eliminar: ");
                scanf("%d", &indice);
                getchar();
                eliminarAlumno(&alumnos, &numAlumnos, indice - 1);
                break;
            }
            case 6:
                break;
            default:
                printf("\nOpci\242n no v\240lida. Intente nuevamente.\n");
        }
    } while (opcion != 6);
    
    liberarDias(listaDias);
    free(alumnos);
    
    printf("\nSaliendo del programa...\n");
    return 0;
}
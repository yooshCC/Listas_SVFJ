#include <stdio.h>
#include <stdlib.h>

typedef struct Dia {
	int dia, mes, anio;
	int* asistencia;
	struct Dia* sig;
} Dia;

typedef struct {
	char nombre[50];
} Alumno;

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
			printf("  %s: %s\n",
				   (*(alumnos + i)).nombre,
				   *((*diaActual).asistencia + i) ? "Presente" : "Ausente");
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
	printf("Ingrese número de alumnos: ");
	scanf("%d", &numAlumnos);
	getchar();
	
	Alumno* alumnos = (Alumno*)malloc(sizeof(Alumno) * numAlumnos);
	if (!alumnos) {
		printf("Error: memoria insuficiente\n");
		return 1;
	}
	
	for (int i = 0; i < numAlumnos; i++) {
		printf("Nombre del alumno %d: ", i + 1);
		fgets((*(alumnos + i)).nombre, sizeof((*(alumnos + i)).nombre), stdin);
	}
	
	Dia* listaDias = NULL;
	int opcion;
	
	do {
		printf("\n--- Menú ---\n");
		printf("1. Tomar lista\n");
		printf("2. Mostrar asistencia\n");
		printf("3. Salir\n");
		printf("Seleccione una opción: ");
		scanf("%d", &opcion);
		getchar();
		
		if (opcion == 1) {
			int dia, mes, anio;
			printf("\nIngrese la fecha:\n");
			printf("Día: ");
			scanf("%d", &dia);
			printf("Mes: ");
			scanf("%d", &mes);
			printf("Año: ");
			scanf("%d", &anio);
			getchar();
			
			insertarDia(&listaDias, dia, mes, anio, numAlumnos);
			
			// Ir al último nodo insertado
			Dia* diaActual = listaDias;
			while (diaActual->sig != NULL)
				diaActual = diaActual->sig;
			
			printf("\nRegistro de asistencia para el %02d/%02d/%04d:\n", dia, mes, anio);
			for (int i = 0; i < numAlumnos; i++) {
				int respuesta;
				do {
					printf("¿%s asistió? (1=Presente, 0=Ausente): ", (*(alumnos + i)).nombre);
					scanf("%d", &respuesta);
				} while (respuesta != 0 && respuesta != 1);
				
				*((*diaActual).asistencia + i) = respuesta;
			}
		} else if (opcion == 2) {
			if (listaDias == NULL) {
				printf("\nNo hay registros de asistencia aún.\n");
			} else {
				printf("\n--- Lista completa de asistencia ---\n");
				mostrarAsistencia(listaDias, alumnos, numAlumnos);
			}
		} else if (opcion != 3) {
			printf("\nOpción no válida. Intente nuevamente.\n");
		}
	} while (opcion != 3);
	
	liberarDias(listaDias);
	free(alumnos);
	
	printf("\nSaliendo del programa...\n");
	return 0;
}

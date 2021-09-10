#ifndef UTIL_H
#define UTIL_H
#include <semaphore.h>
#include "./vida.h"
#include "./registro.h"
#include <unistd.h>
#include <pthread.h>
//int nthreads;

//int hilos;
typedef struct args{
	long id;
	long nthreads;
	char **grilla;
	char **nueva_gen;
	long fil;
	long col;
	
}datos;

typedef struct vale{
	char **matriz;
	long fil;
	long col;

}formas;



void *thread(void *arg);
void estadisticas( long gene, long cantidad);

char **crear_matriz( long fil,long  col);

void actualizar_matriz(char **fuente,char **destino, long fil,  long col);

void liberar_matriz(char **matriz, long fila, long  columna);

void avanzar_generacion(char **grilla,char **nueva_gen, long fil,  long col, long nthreads);


long contar_vecinos(char **grilla, long j, long k, long fil,  long col);

void dibujar_grilla(char **matriz,  long  fil,  long  col);


void llenar_matriz_azar(char **grilla,  long fil,  long col,  long cantidad);
//Para las formas de vida
void *forma1(void *arg);
void *forma2(void *arg);
void *forma3(void *arg);
void *forma4(void *arg);
void *forma5(void *arg);
void *forma6(void *arg);


int bloque(char **matriz,long i, long j, long fila, long col);
int colmena_horizontal(char **matriz,long i, long j, long fila, long col);
int colmena_vertical(char **matriz,long i, long j, long fila, long col);
int hogaza(char **matriz,long i, long j, long fila, long col);
int bote(char **matriz,long i, long j, long fila, long col);
int tina(char **matriz,long i, long j, long fila, long col);

void detectar_formas(char **matriz, long fila,long col);
void estadisticas_fv();

//escritura del archivo

void escribir_grilla(char **matriz,  long fil,  long  col, int fd);
void escribir(int fd, void *str, int tamanio);
int abrir(char *ruta);



#endif 

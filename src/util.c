#include <stdio.h>
#include <stdlib.h>
#include "../include/util.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "../include/registro.h"
#include "../include/vida.h"
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//int hilos=5;
//sem_t principal[hilos];	//empiezan en 1
//sem_t worker[hilos];	//empiezan en 0

sem_t na;
sem_t mu;


long nacieron_g0=0;
long nacieron=0;
long murieron=0;
long murieron_g0=0;
double promedio_cv=0.0;
double promedio_cm=0.0;
//Número de formas de vida
int nbloques=0;
int ncolmena_hori=0;
int ncolmena_vert=0;
int nhogaza=0;
int nbote=0;
int ntina=0;



void estadisticas( long gene, long cantidad){
	nacieron_g0=nacieron_g0+nacieron;
	murieron_g0=murieron_g0+murieron;
	if(gene==0){
		nacieron=cantidad;
		nacieron_g0=cantidad;
		
		
	}
	printf("Generacion %ld\n",gene);
	printf("Numero de celulas que nacieron en esta generación %ld\n", nacieron);

	printf("Numero de celulas que murieron en esta generación %ld\n",  murieron );
	printf("Numero de celulas vivas desde generacion 0 %ld\n",nacieron_g0);
	
	
	printf("Numero de celulas muertas desde generacion 0 %ld\n",murieron_g0);
	promedio_cv=(float)nacieron_g0/(gene+1);
	promedio_cm=(float)murieron_g0/(gene+1);
	printf("Promedio de celulas vivas por generacion %.1f\n",promedio_cv);
	printf("Promedio de celulas muertas por generacion %.1f\n",promedio_cm);
	nacieron=0;
	murieron=0;
	
	


}

void dibujar_grilla(char **matriz,  long fil,  long  col){		
	//system("clear");
	char *linea = malloc(col + 1);	//Char nulo al final
	for(  long i = 0; i < fil; i++){
		memset(linea, ' ', col+1);
		linea[col] = 0;
		for( long j = 0; j < col; j++){
			if(matriz[i][j] ==0){
				linea[j] = ' ';
			}
			else if(matriz[i][j] == 1){
				linea[j] = 'x';



			}
		}
		printf("%s", linea);
		printf("\n");
		fflush(stdout);
	}
	free(linea);
	

}


void llenar_matriz_azar(char **grilla,  long fil,  long col,  long cantidad){
	for(long i =0; i<fil; i++){
		memset(grilla[i], 0, col);	
	}
	srand(time(NULL));
	for(long i = 0; i < cantidad; i++){
		long rnd_fil = rand() % fil;	
		long rnd_col = rand() % col;
		
		// printf("%ld %ld\n", rnd_fil, rnd_col);
		if(grilla[rnd_fil][rnd_col] == 1){
			i--;
		}
		else{
			grilla[rnd_fil][rnd_col] = 1;
		}
		
	}

}
void actualizar_matriz(char **fuente,char **destino, long fil,  long col){

	for( long x=0; x < fil; x++){
				for( long y=0; y< col; y++){
					destino[x][y] = fuente[x][y];
				}
			}




}

void liberar_matriz(char **matriz, long fila,  long columna){


for( long i = 0; i < fila; i++){
                free(*(matriz+i));
		
		*(matriz+i)=NULL;
 	}


	free(matriz);
	matriz=NULL;


}



// Avance del proyecto (Inicio)


char **crear_matriz( long fil, long col){

char **nueva;
	nueva = (char **)calloc(fil, sizeof(char *));
	while(nueva == NULL){
		nueva = (char **)calloc(fil, sizeof(char *));
	}

	for (int i=0; i<fil; i++){
   		nueva[i] = (char *)calloc(col, sizeof(char));
   		while(nueva[i] == NULL){
			nueva[i] = (char *)calloc(col, sizeof(char));
		}
   	}
   	return nueva;
}
void *thread(void *arg){
datos *w =(datos *)arg;

for(long j=w->id;j<w->fil;j+=w->nthreads){
	for(long k=0;k<w->col;k++){
		long vecinos=contar_vecinos(w->grilla,j,k,w->fil,w->col);
		if(w->grilla[j][k]==0){
			
			if(vecinos==3){
				
				w->nueva_gen[j][k]=1;//nace

				sem_wait(&na);
				nacieron++;
				sem_post(&na);	

				
			}else{w->nueva_gen[j][k]=0;}	
			
			
		}else if(w->grilla[j][k]==1){

				if(vecinos>=2 && vecinos<=3){
				
					w->nueva_gen[j][k]=1;//sobrevive




				}else{
					w->nueva_gen[j][k]=0;//morisiona
					sem_wait(&mu);
					murieron++;
					sem_post(&mu);
				}


			
		   	}

	}


}

free(w);
return NULL;
	


}




void avanzar_generacion(char **grilla,char **nueva_gen, long fil,  long col, long nthreads){
sem_init(&na,0,1);
sem_init(&mu,0,1);


pthread_t threads[nthreads];
for(long n=0;n<nthreads;n++){
	datos *w =malloc(sizeof(datos));
	w->id=n;
	w->nthreads=nthreads;
	w->grilla=grilla;
	w->nueva_gen=nueva_gen;
	w->fil=fil;
	w->col=col;
	pthread_create(&(threads[n]),NULL,thread,w);

}


for(long i = 0; i<nthreads;i++){
		pthread_join(threads[i],NULL);
	}


}


long contar_vecinos(char **grilla,  long  j, long  k, long fil, long  col){

long vecinos=0;



if(j-1>=0 && k-1>=0){
if(grilla[j-1][k-1]==1){vecinos++;}}

if(j>=0 && k-1>=0){
if(grilla[j][k-1]==1){vecinos++;}}

if(j+1<=fil-1 && k-1>=0){
if(grilla[j+1][k-1]==1){vecinos++;}}

if(j+1<=fil-1 && k>=0){
if(grilla[j+1][k]==1){vecinos++;}}

if(j+1<=fil-1 && k+1<=col-1){
if(grilla[j+1][k+1]==1){vecinos++;}}

if(j>=0 && k+1<=col-1){
if(grilla[j][k+1]==1){vecinos++;}}

if(j-1>=0 && k+1<=col-1){
if(grilla[j-1][k+1]==1){vecinos++;}}

if(j-1>=0 && k>=0){
if(grilla[j-1][k]==1){vecinos++;}}





return vecinos;

}


//PARA LAS FORMAS DE VIDA


void detectar_formas(char **matriz, long fila,long col){
	
	pthread_t hilos[6];//Un hilo para cada forma de vida
	formas *w=(formas *)calloc(1,sizeof(formas));
	w->matriz=matriz;
	w->fil=fila;
	w->col=col;
	pthread_create(&hilos[0],NULL, forma1,w);
	pthread_create(&hilos[1],NULL, forma2,w);
	pthread_create(&hilos[2],NULL, forma3,w);
	pthread_create(&hilos[3],NULL, forma4,w);
	pthread_create(&hilos[4],NULL, forma5,w);
	pthread_create(&hilos[5],NULL, forma6,w);
	
	for(int i=0;i<6;i++){
		pthread_join(hilos[i],NULL);

	}
	free(w);

}
void estadisticas_fv(){
	printf("\n");
	printf("Formas de vida detectadas: \n");
	printf("Bloque: %d\n",nbloques);
	printf("Colmena vertical: %d\n",ncolmena_vert);
	printf("Colmena horizontal: %d\n",ncolmena_hori);
	printf("Hogaza: %d\n",nhogaza);
	printf("Bote: %d\n",nbote);
	printf("Tina: %d\n",ntina);	

}



int bloque(char **matriz,long i, long j, long fila, long col){
int existe=1;
long inicio_fila=i-1;
long fin_fila=i+2;
long inicio_col=j-1;
long fin_col=j+2;
if(inicio_fila>=0 && inicio_col>=0 && fin_fila<=fila-1 && fin_col<=col-1){
	for(long x=inicio_fila;x<=fin_fila;x++){
		//si entra a alguna de estas condiciones, la funcion retornara cero
		if(matriz[x][inicio_col]==1){return 0;}//parte izquiera del recuadro rojo
		if(matriz[x][fin_col]==1){return 0;}//parte derecha del recuadro rojo
	}
	for(long y=inicio_col;y<=fin_col;y++){
		//si entra a alguna de estas condiciones, la funcion retornara cero
		if(matriz[inicio_fila][y]==1){return 0;}//parte superior del recuadro rojo
		if(matriz[fin_fila][y]==1){return 0;}//parte inferior del recuadro rojo

	}
	//si entra a alguna de estas condiciones, la funcion retornara cero
	if(matriz[i][j]==0){return 0;}
	if(matriz[i][j+1]==0){return 0;}
	if(matriz[i+1][j]==0){return 0;}
	if(matriz[i+1][j+1]==0){return 0;}
}else{existe=0;}//Si las celulas tocan el tablero, existe es igual a 0

return existe;

}



int colmena_horizontal(char **matriz,long i, long j, long fila, long col){
int existe=1;

long inicio_fila=i-1;
long fin_fila=i+3;
long inicio_col=j-1;
long fin_col=j+4;

if(inicio_fila>=0 && inicio_col>=0 && fin_fila<=fila-1 && fin_col<=col-1){
	for(long x=inicio_fila;x<=fin_fila;x++){
		//si entra a alguna de estas condiciones, la funcion retornara cero
		if(matriz[x][inicio_col]==1){return 0;}//parte izquiera del recuadro rojo
		if(matriz[x][fin_col]==1){return 0;}//parte derecha del recuadro rojo
	}
	for(long y=inicio_col;y<=fin_col;y++){
		//si entra a alguna de estas condiciones, la funcion retornara cero
		if(matriz[inicio_fila][y]==1){return 0;}//parte superior del recuadro rojo
		if(matriz[fin_fila][y]==1){return 0;}//parte inferior del recuadro rojo

	}
	//si entra a alguna de estas condiciones, la funcion retornara cero
	//PRIMERA FILA DONDE ESTAN LAS CELULAS
	if(matriz[i][j]==1){return 0;}
	if(matriz[i][j+1]==0){return 0;}
	if(matriz[i][j+2]==0){return 0;}
	if(matriz[i][j+3]==1){return 0;}
	//segunda fila
	if(matriz[i+1][j]==0){return 0;}
	if(matriz[i+1][j+1]==1){return 0;}
	if(matriz[i+1][j+2]==1){return 0;}
	if(matriz[i+1][j+3]==0){return 0;}
	//tercera fila
	if(matriz[i+2][j]==1){return 0;}
	if(matriz[i+2][j+1]==0){return 0;}
	if(matriz[i+2][j+2]==0){return 0;}
	if(matriz[i+2][j+3]==1){return 0;}
}else{existe=0;}//Si las celulas tocan el tablero, existe es igual a 0



return existe;
}




int colmena_vertical(char **matriz,long i, long j, long fila, long col){

int existe=1;
long inicio_fila=i-1;
long fin_fila=i+4;
long inicio_col=j-1;
long fin_col=j+3;

if(inicio_fila>=0 && inicio_col>=0 && fin_fila<=fila-1 && fin_col<=col-1){
	for(long x=inicio_fila;x<=fin_fila;x++){
		//si entra a alguna de estas condiciones, la funcion retornara cero
		if(matriz[x][inicio_col]==1){return 0;}//parte izquiera del recuadro rojo
		if(matriz[x][fin_col]==1){ return 0;}//parte derecha del recuadro rojo
	}
	for(long y=inicio_col;y<=fin_col;y++){
		//si entra a alguna de estas condiciones, la funcion retornara cero
		if(matriz[inicio_fila][y]==1){  return 0;}//parte superior del recuadro rojo
		if(matriz[fin_fila][y]==1){return 0;}//parte inferior del recuadro rojo

	}
	//si entra a alguna de estas condiciones, la funcion retornara cero
	//primera fila donde estan las celulas
	if(matriz[i][j]==1){return 0;}
	if(matriz[i][j+1]==0){return 0;}
	if(matriz[i][j+2]==1){return 0;}
	//segunda fila
	if(matriz[i+1][j]==0){return 0;}
	if(matriz[i+1][j+1]==1){return 0;}
	if(matriz[i+1][j+2]==0){return 0;}
	//tercera fila
	if(matriz[i+2][j]==0){return 0;}
	if(matriz[i+2][j+1]==1){return 0;}
	if(matriz[i+2][j+2]==0){return 0;}
	//cuarta fila
	if(matriz[i+3][j]==1){return 0;}
	if(matriz[i+3][j+1]==0){return 0;}
	if(matriz[i+3][j+2]==1){return 0;}


}else{existe=0;}//Si las celulas tocan el tablero, existe es igual a 0

return existe;


}


int hogaza(char **matriz,long i, long j, long fila, long col){

int existe=1;
long inicio_fila=i-1;
long fin_fila=i+4;
long inicio_col=j-1;
long fin_col=j+4;
if(inicio_fila>=0 && inicio_col>=0 && fin_fila<=fila-1 && fin_col<=col-1){
	for(long x=inicio_fila;x<=fin_fila;x++){
		//si entra a alguna de estas condiciones, la funcion retornara cero
		if(matriz[x][inicio_col]==1){return 0;}//parte izquiera del recuadro rojo
		if(matriz[x][fin_col]==1){return 0;}//parte derecha del recuadro rojo
	}
	for(long y=inicio_col;y<=fin_col;y++){
		//si entra a alguna de estas condiciones, la funcion retornara cero
		if(matriz[inicio_fila][y]==1){return 0;}//parte superior del recuadro rojo
		if(matriz[fin_fila][y]==1){return 0;}//parte inferior del recuadro rojo

	}
	//si entra a alguna de estas condiciones, la funcion retornara cero
	//primera fila
	if(matriz[i][j]==1){return 0;}
	if(matriz[i][j+1]==0){return 0;}
	if(matriz[i][j+2]==0){return 0;}
	if(matriz[i][j+3]==1){return 0;}
	//segunda fila
	if(matriz[i+1][j]==0){return 0;}
	if(matriz[i+1][j+1]==1){return 0;}
	if(matriz[i+1][j+2]==1){return 0;}
	if(matriz[i+1][j+3]==0){return 0;}
	//tercera fila
	if(matriz[i+2][j]==1){return 0;}
	if(matriz[i+2][j+1]==0){return 0;}
	if(matriz[i+2][j+2]==1){return 0;}
	if(matriz[i+2][j+3]==0){return 0;}
	//cuarta fila
	if(matriz[i+3][j]==1){return 0;}
	if(matriz[i+3][j+1]==1){return 0;}
	if(matriz[i+3][j+2]==0){return 0;}
	if(matriz[i+3][j+3]==1){return 0;}
	
	
}else{existe=0;}//Si las celulas tocan el tablero, existe es igual a 0

return existe;

}



int bote(char **matriz,long i, long j, long fila, long col){

int existe=1;
long inicio_fila=i-1;
long fin_fila=i+3;
long inicio_col=j-1;
long fin_col=j+3;
if(inicio_fila>=0 && inicio_col>=0 && fin_fila<=fila-1 && fin_col<=col-1){
	for(long x=inicio_fila;x<=fin_fila;x++){
		//si entra a alguna de estas condiciones, la funcion retornara cero
		if(matriz[x][inicio_col]==1){return 0;}//parte izquiera del recuadro rojo
		if(matriz[x][fin_col]==1){return 0;}//parte derecha del recuadro rojo
	}
	for(long y=inicio_col;y<=fin_col;y++){
		//si entra a alguna de estas condiciones, la funcion retornara cero
		if(matriz[inicio_fila][y]==1){return 0;}//parte superior del recuadro rojo
		if(matriz[fin_fila][y]==1){return 0;}//parte inferior del recuadro rojo

	}
	//si entra a alguna de estas condiciones, la funcion retornara cero
	//primera fila
	if(matriz[i][j]==0){return 0;}
	if(matriz[i][j+1]==0){return 0;}
	if(matriz[i][j+2]==1){return 0;}
	//segunda fila
	if(matriz[i+1][j]==0){return 0;}
	if(matriz[i+1][j+1]==1){return 0;}
	if(matriz[i+1][j+2]==0){return 0;}
	//tercera fila
	if(matriz[i+2][j]==1){return 0;}
	if(matriz[i+2][j+1]==0){return 0;}
	if(matriz[i+2][j+2]==1){return 0;}
}else{existe=0;}//Si las celulas tocan el tablero, existe es igual a 0

return existe;
}


int tina(char **matriz,long i, long j, long fila, long col){

int existe=1;
long inicio_fila=i-1;
long fin_fila=i+3;
long inicio_col=j-1;
long fin_col=j+3;
if(inicio_fila>=0 && inicio_col>=0 && fin_fila<=fila-1 && fin_col<=col-1){
	for(long x=inicio_fila;x<=fin_fila;x++){
		//si entra a alguna de estas condiciones, la funcion retornara cero
		if(matriz[x][inicio_col]==1){return 0;}//parte izquiera del recuadro rojo
		if(matriz[x][fin_col]==1){return 0;}//parte derecha del recuadro rojo
	}
	for(long y=inicio_col;y<=fin_col;y++){
		//si entra a alguna de estas condiciones, la funcion retornara cero
		if(matriz[inicio_fila][y]==1){return 0;}//parte superior del recuadro rojo
		if(matriz[fin_fila][y]==1){return 0;}//parte inferior del recuadro rojo

	}
	//si entra a alguna de estas condiciones, la funcion retornara cero
	//primera fila
	if(matriz[i][j]==1){return 0;}
	if(matriz[i][j+1]==0){return 0;}
	if(matriz[i][j+2]==1){return 0;}
	//segunda fila
	if(matriz[i+1][j]==0){return 0;}
	if(matriz[i+1][j+1]==1){return 0;}
	if(matriz[i+1][j+2]==0){return 0;}
	//tercera fila
	if(matriz[i+2][j]==1){return 0;}
	if(matriz[i+2][j+1]==0){return 0;}
	if(matriz[i+2][j+2]==1){return 0;}
}else{existe=0;}//Si las celulas tocan el tablero, existe es igual a 0

return existe;


}


void *forma1(void *arg){
formas *w=(formas *)arg;
for(long i=0;i<w->fil;i++){
	for(long j=0;j<w->col;j++){
		if(bloque(w->matriz,i,j,w->fil,w->col)){
			
			nbloques++;
		}
	}	


}
return (void *)0;
}




void *forma2(void *arg){
formas *w=(formas *)arg;
for(long i=0;i<w->fil;i++){
	for(long j=0;j<w->col;j++){
		if(colmena_horizontal(w->matriz,i,j,w->fil,w->col)){
			
			ncolmena_hori++;
		}
	}	


}
return (void *)0;
}


void *forma3(void *arg){
formas *w=(formas *)arg;
for(long i=0;i<w->fil;i++){
	for(long j=0;j<w->col;j++){
		if(colmena_vertical(w->matriz,i,j,w->fil,w->col)){
			
			ncolmena_vert++;
		}
	}	


}
return (void *)0;
}


void *forma4(void *arg){
formas *w=(formas *)arg;
for(long i=0;i<w->fil;i++){
	for(long j=0;j<w->col;j++){
		if(hogaza(w->matriz,i,j,w->fil,w->col)){
			
			nhogaza++;
		}
	}	


}
return (void *)0;
}


void *forma5(void *arg){
formas *w=(formas *)arg;
for(long i=0;i<w->fil;i++){
	for(long j=0;j<w->col;j++){
		if(bote(w->matriz,i,j,w->fil,w->col)){
			
			nbote++;
		}
	}	


}
return (void *)0;
}


void *forma6(void *arg){
formas *w=(formas *)arg;
for(long i=0;i<w->fil;i++){
	for(long j=0;j<w->col;j++){
		if(tina(w->matriz,i,j,w->fil,w->col)){
			
			ntina++;
		}
	}	


}
return (void *)0;
}



//Escritura del archivo

int abrir(char *ruta){
umask(0);
int fd=open(ruta,O_WRONLY | O_CREAT | O_TRUNC, 0666);
if(fd<0){
	perror("error al abrir el archivo");
	exit(-1);
}
return fd;

}
void escribir(int fd, void *str, int tamanio){

char *msg=(char *)str;
int escritos = write(fd, msg, tamanio);
if(escritos < 0){
		perror("Error al escribir archivo.");
		exit(-1);
	}
else{


}




}
void escribir_grilla(char **matriz,  long fil,  long  col, int fd){
	char *linea = calloc(col,sizeof(char));	//Char nulo al final


	for(  long i = 0; i < fil; i++){
		for( long j = 0; j < col; j++){
			if(matriz[i][j] ==0){
				linea[j] = ' ';
			}
			else if(matriz[i][j] == 1){
				linea[j] = 'x';

			}
		}

		escribir(fd,linea,sizeof(char)*col);
		escribir(fd,"\n",strlen("\n"));
		
	}
	
	free(linea);


}











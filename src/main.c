#include <stdio.h>
#include <stdlib.h>
#include "../include/util.h"
#include "../include/vida.h"


int main(int argc, char **argv){



long fil=0;
long col =0;
long cantidad =0;
long generaciones=0;
long tiempo=0;
long nthreads=0;
int infinito=0;
int mostrar=0;
char opcion;
	while( (opcion = getopt(argc, argv, "f:c:g:s:i:n:m")) != -1){
		switch(opcion){
			case 'f':
				fil=atol(optarg);
				if(fil<=0){
					printf("Numero de filas ingresados incorrectamente, tiene que ser mayor a cero\n");
					exit(-1);
				}
				break;
			case 'c':
				col=atol(optarg);

				if(col<=0){
					printf("Numero de columnas ingresados incorrectamente,  tiene que ser  mayor a cero\n");
					exit(-1);

				}
				break;
			case 'g':
				generaciones=atol(optarg);
				if(generaciones<0){
					infinito=1;

				}
				break;
			case 's':

				tiempo=atol(optarg);
				if(tiempo<=0){
					printf("tiempo ingresado incorrectamente,  tiene que ser  mayor a cero\n");
					exit(-1);
				}
				break;
			case 'i':

				cantidad=atol(optarg);
				if(cantidad<=0 || cantidad>fil*col){
					printf("Cantidad inicial de celulas ingresados incorrectamente, ingreso menor a cero o sino supera el numero de celulas en el tablero\n");
					exit(-1);
				}
				break;
			case 'n':
				nthreads=atol(optarg);
				if(nthreads<=0){
					printf("Numero de hilos ingresado no válido, debe ser mayor que cero\n");
					exit(-1);

				}
				break;
			case 'm':
				mostrar=1;
				break;


			
		}
}



char **presente;
presente=crear_matriz(fil,col);
char **grilla ;
grilla= crear_matriz(fil,col);
for(long iteracion=0;iteracion<generaciones || infinito;iteracion++){
	if(iteracion==0){
		
		llenar_matriz_azar(grilla,fil,col,cantidad);

		if(mostrar){
			dibujar_grilla(grilla, fil,col);}		
		estadisticas(0,cantidad);
	

	}else {
		usleep(tiempo);
		avanzar_generacion(grilla,presente,fil,col,nthreads);

		if(mostrar){
			dibujar_grilla(presente, fil,col);}		
		estadisticas(iteracion,cantidad);
		actualizar_matriz(presente,grilla,fil,col);
		if(iteracion==generaciones-1){
			detectar_formas(presente,fil,col);
			estadisticas_fv();
			int fd=abrir("resultados.txt");
			escribir_grilla(presente,fil,col,fd);
			close(fd);


		}
		

	}
		


}


liberar_matriz(grilla,fil,col);
liberar_matriz(presente,fil,col);


return 0;

}

								





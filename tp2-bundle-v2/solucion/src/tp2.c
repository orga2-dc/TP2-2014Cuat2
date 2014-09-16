
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "tp2.h"
#include "tiempo.h"
#include "opencv_wrapper.h"
#include "utils.h"

DECLARAR_FILTRO(cropflip)
DECLARAR_FILTRO(sierpinski)
DECLARAR_FILTRO(mblur)
DECLARAR_FILTRO(bandas)

filtro_t filtros[] = {
	DEFINIR_FILTRO(cropflip) ,
	DEFINIR_FILTRO(sierpinski) ,
	DEFINIR_FILTRO(mblur) ,
	DEFINIR_FILTRO(bandas) ,
	{0,0,0,0}
};

void correr_filtro_video(configuracion_t *config, aplicador_fn_t aplicador);
void correr_filtro_imagen(configuracion_t *config, aplicador_fn_t aplicador);

int main( int argc, char** argv ) {

	configuracion_t config;

	procesar_opciones(argc, argv, &config);
	// Imprimo info
	if (!config.nombre)
	{
		printf ( "Procesando...\n");
		printf ( "  Filtro             : %s\n", config.nombre_filtro);
		printf ( "  Implementación     : %s\n", C_ASM( (&config) ) );
		printf ( "  Archivo de entrada : %s\n", config.archivo_entrada);
	}

	filtro_t *filtro = detectar_filtro(&config);

	filtro->leer_params(&config, argc, argv);

	if (config.es_video)
		correr_filtro_video(&config, filtro->aplicador);
	else
		correr_filtro_imagen(&config, filtro->aplicador);

	return 0;
}

filtro_t* detectar_filtro(configuracion_t *config)
{
	for (int i = 0; filtros[i].nombre != 0; i++)
	{
		if (strcmp(config->nombre_filtro, filtros[i].nombre) == 0)
			return &filtros[i];
	}

	perror("Filtro desconocido");
	return NULL; // avoid C warning
}

void correr_filtro_imagen(configuracion_t *config, aplicador_fn_t aplicador)
{
	snprintf(config->archivo_salida, sizeof(config->archivo_salida), "%s/%s.%s.%s%s.bmp",
                 config->carpeta_salida, basename(config->archivo_entrada),
                 config->nombre_filtro, C_ASM(config), config->extra_archivo_salida );

	if (config->nombre)
	{
		printf("%s\n", basename(config->archivo_salida));
	}
	else
	{
		opencv_abrir_imagenes(config);
		aplicador(config);
		opencv_liberar_imagenes(config);
	}
}

void correr_filtro_video(configuracion_t *config, aplicador_fn_t aplicador)
{
    if (!config->frames) {
        snprintf(config->archivo_salida, sizeof(config->archivo_salida), "%s/%s.%s.%s%s.avi",
                 config->carpeta_salida, basename(config->archivo_entrada),
                 config->nombre_filtro, C_ASM(config), config->extra_archivo_salida );
	}

	if (config->nombre)
	{
		printf("%s\n", basename(config->archivo_salida));
	}
	else
	{
		opencv_abrir_video(config);
		opencv_frames_do(config, aplicador);
		opencv_liberar_video(config);
	}
}



void imprimir_tiempos_ejecucion(unsigned long long int start, unsigned long long int end, int cant_iteraciones) {
	unsigned long long int cant_ciclos = end-start;

	printf("Tiempo de ejecución:\n");
	printf("  Comienzo                          : %llu\n", start);
	printf("  Fin                               : %llu\n", end);
	printf("  # iteraciones                     : %d\n", cant_iteraciones);
	printf("  # de ciclos insumidos totales     : %llu\n", cant_ciclos);
	printf("  # de ciclos insumidos por llamada : %.3f\n", (float)cant_ciclos/(float)cant_iteraciones);
}




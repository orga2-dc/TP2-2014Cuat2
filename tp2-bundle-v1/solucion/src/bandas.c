
#include <stdio.h>
#include <string.h>

#include "tp2.h"

void bandas_asm    (unsigned char *src, unsigned char *dst, int cols, int filas,
                      int src_row_size, int dst_row_size);

void bandas_c    (unsigned char *src, unsigned char *dst, int cols, int filas,
                      int src_row_size, int dst_row_size);

typedef void (bandas_fn_t) (unsigned char*, unsigned char*, int, int, int, int);


void leer_params_bandas(configuracion_t *config, int argc, char *argv[]) {

}

void aplicar_bandas(configuracion_t *config)
{
	bandas_fn_t *bandas = SWITCH_C_ASM ( config, bandas_c, bandas_asm ) ;
	buffer_info_t info = config->src;
	bandas(info.bytes, config->dst.bytes, info.width, info.height, info.width_with_padding,
	         config->dst.width_with_padding);

}

void ayuda_bandas()
{
	printf ( "       * bandas\n" );
	printf ( "           Parámetros     : \n"
	         "                         no tiene\n");
	printf ( "           Ejemplo de uso : \n"
	         "                         bandas -i c facil.bmp\n" );
}



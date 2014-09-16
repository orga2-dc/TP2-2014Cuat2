
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tp2.h"

void mblur_asm    (unsigned char *src, unsigned char *dst, int cols, int filas,
                      int src_row_size, int dst_row_size);

void mblur_c    (unsigned char *src, unsigned char *dst, int cols, int filas,
                      int src_row_size, int dst_row_size);

typedef void (mblur_fn_t) (unsigned char*, unsigned char*, int, int, int, int);


void leer_params_mblur(configuracion_t *config, int argc, char *argv[]) {
}

void aplicar_mblur(configuracion_t *config)
{
	mblur_fn_t *mblur = SWITCH_C_ASM ( config, mblur_c, mblur_asm ) ;
	buffer_info_t info = config->src;
	mblur(info.bytes, config->dst.bytes, info.width, info.height, info.width_with_padding,
	         config->dst.width_with_padding);

}

void ayuda_mblur()
{
	printf ( "       * mblur\n" );
	printf ( "           Parámetros     : \n"
	         "                         ninguno\n");
	printf ( "           Ejemplo de uso : \n"
	         "                         mblur -i c facil.bmp\n"
             "                         mblur -i c facil.bmp\n");
}



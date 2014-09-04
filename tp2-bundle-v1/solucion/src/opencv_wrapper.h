#ifndef __OPENCL_WRAPPER__H__
#define __OPENCL_WRAPPER__H__

void opencv_abrir_imagenes(configuracion_t *config);
void opencv_liberar_imagenes(configuracion_t *config);

void opencv_abrir_video(configuracion_t *config);
void opencv_liberar_video(configuracion_t *config);

void opencv_frames_do(configuracion_t *config, aplicador_fn_t aplicador);
void opencv_abrir_destino(int width, int height, configuracion_t *config);


#endif /* !__OPENCL_WRAPPER__H__ */


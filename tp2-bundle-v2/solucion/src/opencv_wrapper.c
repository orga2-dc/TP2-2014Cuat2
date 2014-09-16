
#include <highgui.h>
#include <cv.h>
#include <stdio.h>

#include "tp2.h"
#include "opencv_wrapper.h"
#include "utils.h"

typedef struct CvVideoWriter CvVideoWriter;



// atenti, se puede tener solo 1 video in, 1 video out, 1 imagen in y 1 imagen out
CvCapture     *srcVid = NULL;
CvVideoWriter *dstVid = NULL;
IplImage      *srcImg = NULL;
IplImage      *dstImg = NULL;

struct CvVideoWriter *abrir_writer(const char *archivo_salida, int fps, CvSize size);
CvCapture *abrir_video(const char *archivo_entrada);

// Esta funcion crea una IplImage para el destino con el tamanio dado y 4 canales.
// Es usada por filtros que trabajan sobre una imagen destino con tamanio distinto a la imagen de entrada.
void opencv_abrir_destino(int width, int height, configuracion_t *config) {
	cvReleaseImage(&dstImg);
	
	// Creo una imagen RGBA temporal para el DST.
	if( (dstImg = cvCreateImage ( (CvSize) {width, height}, IPL_DEPTH_8U, 4) ) == NULL ) {
        	exit(EXIT_FAILURE);
    	}

	config->dst.width              = dstImg->width;
	config->dst.height             = dstImg->height;
	config->dst.width_with_padding = dstImg->widthStep;
	config->dst.bytes = (unsigned char*)dstImg->imageData;
}

void opencv_abrir_imagenes(configuracion_t *config)
 {
    	IplImage *tmp = NULL;
	// Cargo la imagen
	if( (tmp = cvLoadImage (config->archivo_entrada, CV_LOAD_IMAGE_COLOR)) == 0 ) {
    	fprintf(stderr, "Error abriendo la imagen fuente\n");
		exit(EXIT_FAILURE);
	}


	// Creo una imagen RGBA temporal para el SRC.
	if( (srcImg = cvCreateImage (cvGetSize (tmp), IPL_DEPTH_8U, 4) ) == NULL ) {
        	exit(EXIT_FAILURE);
    	}

	// pongo en srcImg el contenido de la imagen en 4 canales.
	cvCvtColor(tmp, srcImg, CV_BGR2BGRA);
	cvReleaseImage(&tmp);

	config->src.bytes = (unsigned char*)srcImg->imageData;
	config->src.width              = srcImg->width;
	config->src.height             = srcImg->height;
	config->src.width_with_padding = srcImg->widthStep;

	opencv_abrir_destino(config->src.width, config->src.height, config);
}

void opencv_liberar_imagenes(configuracion_t *config)
{
	// Convierto la imagen destino de 4 canales a 3 canales.
    	IplImage *tmp = NULL;
	if( (tmp = cvCreateImage (cvGetSize (dstImg), IPL_DEPTH_8U, 3) ) == NULL ) {
        	exit(EXIT_FAILURE);
    	}
	cvCvtColor(dstImg, tmp, CV_BGRA2BGR);
	// Guardo imagen resultado y libero los buffers
	if (cvSaveImage(config->archivo_salida, tmp, NULL) == 0)
	    fprintf(stderr, "Error guardando la imagen destino (%s)\n", config->archivo_salida);


	cvReleaseImage(&tmp);
	cvReleaseImage(&srcImg);
	cvReleaseImage(&dstImg);
}



// ATENCION!!: Para trabajar con imagenes usamos 4 canales (RGBA), pero no hice el mismo 
// cambio para trabajar con video. Los archivos de video trabajan con 3 canales, así que
// las funciones no van a andar. Si se quiere probar un filtro sobre video, se debería
// convertir cada frame del video a RGBA y luego al guardarlo volver a convertirlo a RGB
void opencv_abrir_video(configuracion_t *config)
{
	srcVid = abrir_video(config->archivo_entrada);

	CvSize dst_size;
	dst_size.width = cvGetCaptureProperty(srcVid,CV_CAP_PROP_FRAME_WIDTH);
	dst_size.height = cvGetCaptureProperty(srcVid,CV_CAP_PROP_FRAME_HEIGHT);

	config->src.width  = dst_size.width;
	config->src.height = dst_size.height;
	config->src.width_with_padding = dst_size.width*3;
	config->dst.width  = dst_size.width;
	config->dst.height = dst_size.height;
	config->dst.width_with_padding = dst_size.width*3;

	double fps = cvGetCaptureProperty(srcVid,CV_CAP_PROP_FPS);

	/* Armo el buffer destino. */
	int nchannels = 3;
	dstImg = cvCreateImage (dst_size, IPL_DEPTH_8U, nchannels);
	if (dstImg == NULL) {
		fprintf(stderr, "Error armando la imagen destino\n");
		exit(EXIT_FAILURE);
	}

	config->dst.bytes = (unsigned char*)dstImg->imageData;

	if (!config->frames) {
		dstVid = abrir_writer(config->archivo_salida, fps, dst_size);
	}

	if (config->verbose) {
		cvNamedWindow("procesanding", CV_WINDOW_AUTOSIZE);
	}
}


void opencv_frames_do(configuracion_t *config, aplicador_fn_t aplicador)
{
	unsigned int framenum = 0;
	while(1) {
		/* Capturo un frame */
		IplImage *frame = cvQueryFrame(srcVid);
		framenum ++;

		if (frame == NULL) {
			break;
		}
		config->src.bytes = (unsigned char*)frame->imageData;

		aplicador(config);

		if (config->frames) { 
			const char *filename = basename(config->archivo_entrada);

			/* Escribo el frame en bmp */
			snprintf(config->archivo_salida, sizeof(config->archivo_salida), "%s/%s.%s.%d.bmp",
					config->carpeta_salida, filename, config->nombre_filtro, framenum);
			cvSaveImage(config->archivo_salida, dstImg, NULL);
		} else {
			/* Escribo el frame en el video */
			cvWriteFrame(dstVid, dstImg);
		}

		if (config->verbose) {
			cvShowImage("procesanding", dstImg);
			cvWaitKey(1);
		}
	}
}

void opencv_liberar_video(configuracion_t *config)
{
    cvReleaseImage(&dstImg);
    if (config->verbose) {
        cvDestroyWindow("procesanding");
    }

    if (!config->frames) {
        cvReleaseVideoWriter(&dstVid);
    }
    cvReleaseCapture(&srcVid);

}

struct CvVideoWriter *abrir_writer(const char *archivo_salida, int fps, CvSize size) {
    struct CvVideoWriter *dstVid = NULL;
    dstVid = cvCreateVideoWriter(archivo_salida, CV_FOURCC('M','J','P','G'), fps, size, 1);
    if(dstVid == NULL) {
        fprintf(stderr, "Invalid dstVid\n");
        exit(EXIT_FAILURE);
    }

    return dstVid;
}


CvCapture *abrir_video(const char *archivo_entrada) {
    CvCapture *srcVid = NULL;
    srcVid = cvCaptureFromFile(archivo_entrada);
    if( srcVid == NULL) {
        /* Esto no está documentado. No debería pasar nunca. */
        fprintf(stderr, "Invalid srcVid\n%s \n", archivo_entrada);
        exit(EXIT_FAILURE);
    }

    return srcVid;
}



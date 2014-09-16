#!/bin/bash

DATADIR=./data
INPUTDIR=$DATADIR/imagenes_a_testear

ROJO="\e[31m"
VERDE="\e[32m"

OKVALGRIND=1

echo 'Iniciando test de memoria (valgrind sobre C y ASM)...'

while read params_filtro; do
	for imagen in $(ls $INPUTDIR); do
	    ./implementacion_vs_valgrind.sh $imagen c $params_filtro
	    ./implementacion_vs_valgrind.sh $imagen asm $params_filtro
		if [ $? != 0 ]; then
			OKVALGRIND=0
#			exit # para que pare en el primer error descomentar esta linea
		fi
	done
done < corridas.txt


if [ $OKVALGRIND != "0" ]; then
	echo -e "\n${VERDE}Tests de memoria finalizados correctamente.\n"
else
	echo -e "\n${ROJO}Se encontraron problemas de memoria en alguno de los filtros.\n"
fi

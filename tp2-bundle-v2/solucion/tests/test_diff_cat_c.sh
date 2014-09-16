#!/bin/bash

DATADIR=./data
INPUTDIR=$DATADIR/imagenes_a_testear

ROJO="\e[31m"
VERDE="\e[32m"

OKDIFF=1

echo 'Iniciando test de diferencias C vs. la catedra...'

while read params_filtro; do
	for imagen in $(ls $INPUTDIR); do
	    ./buscardiff.sh $imagen c $params_filtro
		if [ $? != 0 ]; then
			OKDIFF=0
#			exit # para que pare en el primer error descomentar esta linea
		fi
	done
done < corridas.txt

if [ $OKDIFF != "0" ]; then
	echo -e "${VERDE}Test de filtros finalizados correctamente"
else
	echo -e "${ROJO}se encontraron diferencias en alguna de las imagenes"
fi


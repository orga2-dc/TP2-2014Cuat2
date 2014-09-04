#!/bin/bash

DATADIR=./data
INPUTDIR=$DATADIR/imagenes_a_testear
CATEDRADIR=$DATADIR/resultados_catedra
ALUMNOSDIR=$DATADIR/resultados_nuestros
TP2CAT=../bin/tpcatedra
TP2ALU=../bin/tp2

ROJO="\e[31m"
VERDE="\e[32m"
AZUL="\e[94m"
DEFAULT="\e[39m"

VALGRINDFLAGS="--leak-check=no --error-exitcode=1 -q"

# argumentos que me vienen
archivo_in=$1
implementacion=$2
tolerancia=$3
filtro=$4
mensaje="${AZUL}filtro $filtro version $implementacion${DEFAULT}"

shift 4   #descarto los argumentos que ya use

PARAMS=$@   # El resto de los argumentos 

echo -e $mensaje


archivo_out1=$($TP2ALU $filtro -n -i $implementacion     -o $ALUMNOSDIR/ $INPUTDIR/$archivo_in $PARAMS)
valgrind $VALGRINDFLAGS $TP2ALU $filtro -i $implementacion -o $ALUMNOSDIR/ $INPUTDIR/$archivo_in $PARAMS

exit $?


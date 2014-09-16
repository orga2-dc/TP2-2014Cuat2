#!/bin/bash

DIFF=../bin/tp2diff
DIFFFLAGS=""
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

# argumentos que me vienen
archivo_in=$1
implementacion=$2
tolerancia=$3
filtro=$4
mensaje="${AZUL}filtro $filtro version catedra contra tu $implementacion${DEFAULT}"

shift 4   # descarto los argumentos que ya use

PARAMS=$@   # El resto de los argumentos 

echo -e $mensaje


archivo_out_cat=$($TP2CAT $filtro -n -i c -o $CATEDRADIR/    $INPUTDIR/$archivo_in $PARAMS)
$TP2CAT $filtro -i c -o $CATEDRADIR/    $INPUTDIR/$archivo_in $PARAMS

archivo_out_alu=$($TP2ALU $filtro -n -i $implementacion     -o $ALUMNOSDIR/ $INPUTDIR/$archivo_in $PARAMS)
$TP2ALU $filtro -i $implementacion -o $ALUMNOSDIR/ $INPUTDIR/$archivo_in $PARAMS



ALUFILE=$ALUMNOSDIR/$archivo_out_alu
CATFILE=$CATEDRADIR/$archivo_out_cat

$DIFF $DIFFFLAGS $CATFILE $ALUFILE $tolerancia

if [ $? != "0" ]; then
    echo -e "${ROJO}error en $FILENAME"
    exit -1
else
	echo -e "${VERDE}iguales!"
fi





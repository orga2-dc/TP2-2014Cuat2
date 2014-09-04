#!/bin/bash

# Este script crea las multiples imagenes de prueba a partir de unas
# pocas imagenes base.

DATADIR=./data/
TESTINDIR=$DATADIR/imagenes_a_testear
CATEDRADIR=$DATADIR/resultados_catedra
ALUMNOSDIR=$DATADIR/resultados_nuestros

IMAGENES="lena.bmp"

mkdir $TESTINDIR $CATEDRADIR $ALUMNOSDIR

#sizes=(200x200 201x201 202x202 203x203 204x204 205x205 206x206 207x207 208x208 256x256 512x512 513x513 1023x767)
sizes=(200x200 204x204 208x208 256x256 512x512 1024x767)


for f in $IMAGENES;
do
	echo $f

	for s in ${sizes[*]}
	do
		echo "  *" $s

		`echo  "convert -resize $s!" $DATADIR/$f ` $TESTINDIR/`echo "$f" | cut -d'.' -f1`.$s.bmp

	done
done

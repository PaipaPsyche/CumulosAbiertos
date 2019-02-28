#!/bin/bash
ls *.dat > lista.txt

count=1
fileout="SIGCOL_RUN.txt"

for filename in $(ls *.dat); do
echo "Leyendo $filename"
thestr="$(./sigcol $filename 3 2)"
thestr=${thestr:104}
echo $thestr >> $fileout
 count=$(($count +1))
done



echo "Se leyeron $count archivos en total."

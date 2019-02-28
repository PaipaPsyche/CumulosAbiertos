#!/bin/bash

rm -rf FNVAR



while IFS='' read -r line || [[ -n "$line" ]]; do
    echo "Leyendo $line"
    NN=`wc -l <"$line"`
    if [ $NN -ge $5 ] ; then
    ./fnpeaks $line $2 $3 $4 > "FN.$line"
    sed -i -e '1,4d' "FN.$line"
    else
    echo "$line solo tiene $NN lineas."
    fi
    
done < "$1"

mkdir -m 777 FNVAR
cp readVariables.py FNVAR
mv FN.* FNVAR
mv *.max FNVAR



cd FNVAR
touch SUSPVAR.txt
ls FN.* > lista
python3 readVariables.py lista "$6"

mkdir -m 777 OUTVAR
mv FN* OUTVAR
mv *.max OUTVAR



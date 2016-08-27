#!/usr/bin/env bash
F=`mktemp`
tr --delete "\n\t" < index.html  > $F
STR=`sed 's/"/\\\\"/g' $F`
T=`cat template.h`
printf "$T" "$STR" > ../html.h
rm $F

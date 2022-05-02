#!/bin/bash
for i in `ls macros/*.C`; do
	basename=${i##*/}
	g++ -o run/${basename%.*} $i -I src `root-config --cflags --glibs`
done

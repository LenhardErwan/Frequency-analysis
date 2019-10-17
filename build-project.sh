#!/bin/bash

oldPath=`pwd`

cd Algo1
g++ FreqAnalysis-sequentiel.cpp Analyse.cpp -o FreqAnalysis-sequentiel.exe
g++ FreqAnalysis-pthread.cpp Analyse.cpp -o FreqAnalysis-pthread.exe -lpthread

cd $oldPath/Algo2
g++ FreqAnalysis-sequentiel.cpp Analyse.cpp -o FreqAnalysis-sequentiel.exe
g++ FreqAnalysis-pthread.cpp Analyse.cpp -o FreqAnalysis-pthread.exe -lpthread

exit 0
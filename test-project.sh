#!/bin/bash

echo "Algo1 : Séquentiel" > test.log
echo "fichier d'environ 200Ko" >> test.log
./Algo1/FreqAnalysis-sequentiel.exe ./Fichiers-Test/fic200ko.txt 2>&1 >> test.log
echo -e "\nfichier d'environ 400Ko" >> test.log
./Algo1/FreqAnalysis-sequentiel.exe ./Fichiers-Test/fic400ko.txt 2>&1 >> test.log
echo -e "\nfichier d'environ 600Ko" >> test.log
./Algo1/FreqAnalysis-sequentiel.exe ./Fichiers-Test/fic600ko.txt 2>&1 >> test.log
echo -e "\nfichier d'environ 1Mo" >> test.log
./Algo1/FreqAnalysis-sequentiel.exe ./Fichiers-Test/fic1mo.txt 2>&1 >> test.log
echo -e "\nfichier d'environ 10Mo" >> test.log
./Algo1/FreqAnalysis-sequentiel.exe ./Fichiers-Test/fic10mo.txt 2>&1 >> test.log

echo -e "\n\nAlgo1 : pthread" >> test.log
echo "fichier d'environ 200Ko" >> test.log
./Algo1/FreqAnalysis-pthread.exe ./Fichiers-Test/fic200ko.txt 2>&1 >> test.log
echo -e "\nfichier d'environ 400Ko" >> test.log
./Algo1/FreqAnalysis-pthread.exe ./Fichiers-Test/fic400ko.txt 2>&1 >> test.log
echo -e "\nfichier d'environ 600Ko" >> test.log
./Algo1/FreqAnalysis-pthread.exe ./Fichiers-Test/fic600ko.txt 2>&1 >> test.log
echo -e "\nfichier d'environ 1Mo" >> test.log
./Algo1/FreqAnalysis-pthread.exe ./Fichiers-Test/fic1mo.txt 2>&1 >> test.log
echo -e "\nfichier d'environ 10Mo" >> test.log
./Algo1/FreqAnalysis-pthread.exe ./Fichiers-Test/fic10mo.txt 2>&1 >> test.log

echo -e "\n\nAlgo2 : Séquentiel" >> test.log
echo "fichier d'environ 200Ko" >> test.log
./Algo2/FreqAnalysis-sequentiel.exe ./Fichiers-Test/fic200ko.txt 2>&1 >> test.log
echo -e "\nfichier d'environ 400Ko" >> test.log
./Algo2/FreqAnalysis-sequentiel.exe ./Fichiers-Test/fic400ko.txt 2>&1 >> test.log
echo -e "\nfichier d'environ 600Ko" >> test.log
./Algo2/FreqAnalysis-sequentiel.exe ./Fichiers-Test/fic600ko.txt 2>&1 >> test.log
echo -e "\nfichier d'environ 1Mo" >> test.log
./Algo2/FreqAnalysis-sequentiel.exe ./Fichiers-Test/fic1mo.txt 2>&1 >> test.log
echo -e "\nfichier d'environ 10Mo" >> test.log
./Algo2/FreqAnalysis-sequentiel.exe ./Fichiers-Test/fic10mo.txt 2>&1 >> test.log

echo -e "\n\nAlgo2 : pthread" >> test.log
echo "fichier d'environ 200Ko" >> test.log
./Algo2/FreqAnalysis-pthread.exe ./Fichiers-Test/fic200ko.txt 2>&1 >> test.log
echo -e "\nfichier d'environ 400Ko" >> test.log
./Algo2/FreqAnalysis-pthread.exe ./Fichiers-Test/fic400ko.txt 2>&1 >> test.log
echo -e "\nfichier d'environ 600Ko" >> test.log
./Algo2/FreqAnalysis-pthread.exe ./Fichiers-Test/fic600ko.txt 2>&1 >> test.log
echo -e "\nfichier d'environ 1Mo" >> test.log
./Algo2/FreqAnalysis-pthread.exe ./Fichiers-Test/fic1mo.txt 2>&1 >> test.log
echo -e "\nfichier d'environ 10Mo" >> test.log
./Algo2/FreqAnalysis-pthread.exe ./Fichiers-Test/fic10mo.txt 2>&1 >> test.log

exit 0

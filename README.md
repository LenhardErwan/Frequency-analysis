# Frequency-analysis

## Compilation et Utilisation

Version séquentielle
```g++ FreqAnalysis-sequentiel.cpp Analyse.cpp -o FreqAnalysis-sequentiel.exe```
```./FreqAnalysis-sequentiel.exe /path/to/file.txt```

Version avec pthread
```g++ FreqAnalysis-pthread.cpp Analyse.cpp -o FreqAnalysis-pthread.exe -lpthread```
```./FreqAnalysis-pthread.exe /path/to/file.txt```


## Fonctionnement

Le programme prend en paramètre le chemin vers le fichier à analyser. Puis, il prépare le nom du fichier de sorti (date à l'éxécution). Puis, il génère les objets **Analyse**, avec le chemin d'entrée et un tableau associatif avec pour clé le graphène à rechercher et pour valeur 0. Lors de l'analyse, ce tableau associatif prendra en valeur le nombre d'itérations de chaque graphènes et à la fin de la lecture du fichier la valeur sera la fréquence de chaque graphènes.
Ensuite, le programme fait l'analyse des lettres, puis des digrammes et enfin des trigramme, chaque **Analyse** lit le fichier entièrement (Après des tests nous avons remarqué que lire un seul fois le fichier ou plusieur fois n'influer pas sur le temps d'éxécution). A la fin de la lecture, la fréquence de chaque graphènes est calculée.
Enfin, le programme ecrit dans un fichier le résultat de l'analyse des lettres, puis des digrammes et des trigrammes. Puis libère la mémoire.
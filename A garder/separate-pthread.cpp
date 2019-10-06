#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <pthread.h>
#include <chrono>
#include <ctime> 

#include "Analyse.hpp"

Analyse* generateLetterAnalyse(std::string pathIn) {
    std::map<std::string, float> * map = new std::map<std::string, float>; //Créer une map avec pour clé les caractère et pour valeur leur itération

    for (char letter = 'a'; letter <= 'z'; ++letter) {  //Permet d'avoir tout l'alphabet en minuscule
        map->emplace(std::string(1, letter), 0.0f);    //rempli la map avec l'alaphabet initialisé à 0
    }

    return new Analyse(map, pathIn);
}

Analyse* generateDigrammeAnalyse(std::string pathIn) {
    std::map<std::string, float> * map = new std::map<std::string, float>; //Créer une map avec pour clé les caractère et pour valeur leur itération

    for (char letter1 = 'a'; letter1 <= 'z'; ++letter1) { 
        for(char letter2 = 'a';  letter2 <= 'z'; ++letter2) {
            map->emplace(std::string() + letter1 + letter2, 0.0f);    //rempli la map avec les digrammes initialisé à 0
        }
    }

    return new Analyse(map, pathIn);
}

Analyse* generateTrigrammeAnalyse(std::string pathIn) {
    std::map<std::string, float> * map = new std::map<std::string, float>; //Créer une map avec pour clé les caractère et pour valeur leur itération

    for (char letter1 = 'a'; letter1 <= 'z'; ++letter1) { 
        for(char letter2 = 'a';  letter2 <= 'z'; ++letter2) {
            for(char letter3 = 'a';  letter3 <= 'z'; ++letter3) {
                map->emplace(std::string() + letter1 + letter2 + letter3, 0.0f);    //rempli la map avec les digrammes initialisé à 0
            }
        }
    }

    return new Analyse(map, pathIn);
}


void printAnalyse(Analyse * a, std::string path) {
	std::ofstream fic;
    fic.open(path, std::ios::out | std::ios::app);	//Creer un ostream avec ce buffer

    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur
    
    fic << (*a);   //Ecrit dans le fichier les données de l'Analyse

    fic.close();
}

void* freqLetter(void* analyse) {
    Analyse * a = (Analyse*) analyse;

    std::string path = a->getPathIn();
    std::ifstream fic;
    fic.open(path, std::ios_base::in);  //Ouvre le fichier en lecture
    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur

    char c; //Caractère lut
    std::string letter;

    while (fic.get(c)) {    //Tant que il y a des caractères a lire
        a->incGraphene(&(letter = c));
    }
    
    a->calcFreq();//Créer les fréquence et la map finale

    fic.close();    //ferme le fichier (libère la mémoire je suppose)
}

void* freqDigramme(void* analyse) {
    Analyse * a = (Analyse*) analyse;

    std::string path = a->getPathIn();
    std::ifstream fic;
    fic.open(path, std::ios_base::in);  //Ouvre le fichier en lecture
    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur

    char c; //Caractère lut
    char old = fic.get();
    std::string digramme = "";  //Variable temporaire qui prend le caractère extrait + le suivant (non  extrait)

    while (fic.get(c)) {    //Tant que il y a des caractères a lire
        c = tolower(c);  //met en minuscule le caractère testé
        digramme = std::string() + old + c;
        if(a->incGraphene(&digramme)) {  //Si le caractère existe dans la map
            fic.get(c); //On passe au caractère suivant (évite de prendre un lettre deux fois pour un digramme)
        }
        old = c;
    }

    a->calcFreq();   //Créer les fréquence et la map finale

    fic.close();
}

void* freqTrigramme(void* analyse) {
    Analyse * a = (Analyse*) analyse;

    std::string path = a->getPathIn();
    std::ifstream fic;
    fic.open(path, std::ios_base::in);  //Ouvre le fichier en lecture
    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur

    char c; //Caractère lut (dernier caractère)
    char old2 = tolower(fic.get());    // sur trois caractère c'est le premier
    char old = tolower(fic.get());     // sur trois caractère c'est le second
    std::string trigramme = "";  //Variable temporaire qui prend le caractère extrait + le suivant (non  extrait)

    while (fic.get(c)) {    //Tant que il y a des caractères a lire
        c = tolower(c);  //met en minuscule le caractère testé
        trigramme = std::string() + old2 + old + c;
        if(a->incGraphene(&trigramme)) {  //Si le caractère existe dans la map
            fic.get(c);
            old = c;
            fic.get(c);  //Saute 2 caractère (evite de re utiliser les 3 caractère que l'ont viens d'utiliser)
        }
        old2 = old;
        old = c;
    }

    a->calcFreq();

    fic.close();
}

int main(int argc, char *argv[]) {
    if (argc == 1) { // verification de la présence des arguments
        std::cerr << "Il manque un argument : veuillez ajouter le nom du fichier a analyser" << std::endl;
    } else if (argc == 2) {
        std::cerr << "Il manque un argument : veuillez ajouter le nom du fichier de resultats" << std::endl;
    }

    clock_t t1=clock();
    
    auto end = std::chrono::system_clock::now(); 
    std::time_t date_actuelle = std::chrono::system_clock::to_time_t(end);
    std::string date_heure =  std::ctime(&date_actuelle);

   
    for (int i=0; i < date_heure.length(); i=i+1) {
        if (date_heure[i] == ':') {
            date_heure[i] = '.';
        }
        if (date_heure[i] == ' ') {
            date_heure.replace (i,1,"_");
        }
    }
    date_heure.append(".txt");
    std::string nomanalyse = argv[1];
    nomanalyse.append(".txt");

    std::ofstream fout(date_heure);


    std::string pathIn = nomanalyse;    // argument 1, chemin du fichier à analyser
    std::string pathOut = date_heure;    // le fichier resultat prend pour nom l'heure et la date actuelle

    Analyse * aLetter = generateLetterAnalyse(pathIn);
    Analyse * aDigramme = generateDigrammeAnalyse(pathIn);
    Analyse * aTrigramme = generateTrigrammeAnalyse(pathIn);

    pthread_t th1;
    pthread_t th2;
    pthread_t th3;

    try {
        pthread_create(&th1, NULL, freqLetter, (void*) aLetter);
        pthread_create(&th2, NULL, freqDigramme, (void*) aDigramme);
        pthread_create(&th3, NULL, freqTrigramme, (void*) aTrigramme);


        pthread_join(th1, NULL);
        printAnalyse(aLetter, pathOut);
        pthread_join(th2, NULL);
        printAnalyse(aDigramme, pathOut);
        pthread_join(th3, NULL);
        printAnalyse(aTrigramme, pathOut);
        
    }
    catch (std::out_of_range & e) { //Si le fichier ne peux pas s'ouvrire
        std::cerr << e.what() << std::endl;
    }
    
    delete aLetter;         //Libère la mémoire
    delete aDigramme;
    delete aTrigramme;

    clock_t t2= clock();
    float temps = (float)(t2-t1)/CLOCKS_PER_SEC;
    std::cout << "Le temps d'exécution total est de : " << temps << "s" <<std::endl;

    return EXIT_SUCCESS;    //FIN
}
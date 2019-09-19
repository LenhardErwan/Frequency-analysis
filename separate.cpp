#include <string>
#include <iostream>
#include <fstream>
#include <time.h>

#include "Analyse.hpp"

Analyse* generateLetterAnalyse() {
    std::map<std::string, float> * map = new std::map<std::string, float>; //Créer une map avec pour clé les caractère et pour valeur leur itération

    for (char letter = 'a'; letter <= 'z'; ++letter) {  //Permet d'avoir tout l'alphabet en minuscule
        map->emplace(std::string(1, letter), 0.0f);    //rempli la map avec l'alaphabet initialisé à 0
    }

    return new Analyse(map);
}

Analyse* generateDigrammeAnalyse() {
    std::map<std::string, float> * map = new std::map<std::string, float>; //Créer une map avec pour clé les caractère et pour valeur leur itération

    for (char letter1 = 'a'; letter1 <= 'z'; ++letter1) { 
        for(char letter2 = 'a';  letter2 <= 'z'; ++letter2) {
            map->emplace(std::string() + letter1 + letter2, 0.0f);    //rempli la map avec les digrammes initialisé à 0
        }
    }

    return new Analyse(map);
}

Analyse* generateTrigrammeAnalyse() {
    std::map<std::string, float> * map = new std::map<std::string, float>; //Créer une map avec pour clé les caractère et pour valeur leur itération

    for (char letter1 = 'a'; letter1 <= 'z'; ++letter1) { 
        for(char letter2 = 'a';  letter2 <= 'z'; ++letter2) {
            for(char letter3 = 'a';  letter3 <= 'z'; ++letter3) {
                map->emplace(std::string() + letter1 + letter2 + letter3, 0.0f);    //rempli la map avec les digrammes initialisé à 0
            }
        }
    }

    return new Analyse(map);
}


void printAnalyse(Analyse * a, std::string path) {
	std::ofstream fic;
    fic.open(path, std::ios::out | std::ios::app);	//Creer un ostream avec ce buffer

    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur
    
    fic << (*a);   //Ecrit dans le fichier les données de l'Analyse

    fic.close();
}

void freqLetter(Analyse* a, std::string path) {
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

void freqDigramme(Analyse * a, std::string path) {
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

void freqTrigramme(Analyse* a, std::string path) {
    std::ifstream fic;
    fic.open(path, std::ios_base::in);  //Ouvre le fichier en lecture
    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur

    std::map<std::string, float> * map = new std::map<std::string, float>;  //Créer une map avec pour clé les caractère et pour valeur leur itération

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

int main() {
    clock_t t1=clock();
    std::remove("./result2.txt");    //Supprime le fichier de resultat s'il existe déja

    std::string pathIn = "./grosfichier.txt";    //Il faudrai le passé en argument du programme
    std::string pathOut = "./result2.txt";    //Il faudrai le passé en argument du programme
    Analyse * aLetter = generateLetterAnalyse();
    Analyse * aDigramme = generateDigrammeAnalyse();
    Analyse * aTrigramme = generateTrigrammeAnalyse();

    try {
        freqLetter(aLetter, pathIn);
        freqDigramme(aDigramme, pathIn);
        freqTrigramme(aTrigramme, pathIn);

        printAnalyse(aLetter, pathOut);
        printAnalyse(aDigramme, pathOut);
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
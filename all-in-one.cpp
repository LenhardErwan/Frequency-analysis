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

void startAnalyse(Analyse* aLetter, Analyse* aDigramme, Analyse* aTrigramme, std::string path) {
    std::ifstream fic;
    fic.open(path, std::ios_base::in);  //Ouvre le fichier en lecture
    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur


    char c; //Caractère lut
    std::string letter ="", dig = "", trig = "";

    while (fic.get(c)) {    //Tant que il y a des caractères a lire
        c = (char) tolower(c);  //met en minuscule le caractère test

        //Pour les caractères uniquement
        aLetter->incGraphene(&(letter = c));

        //Pour les digrammes
        dig = dig + c;
        if(dig.length() >= 2) {
            if(aDigramme->incGraphene(&dig))
                dig = "";   //remet le graphène à zero (evite d'utiliser un caractère dans deux digrammes)
            else
                dig.erase(0,1);   //Efface le premier caractère du digramme
        }

        //Pour les trigrammes
        trig = trig + c;
        if(trig.length() >= 3) {
            if(aTrigramme->incGraphene(&trig))
                trig = "";   //remet le graphène à zero (evite d'utiliser un caractère dans deux trigrammes)
            else
                trig.erase(0,1);   //Efface le premier caractère du trigramme
        }
        
    }

    aLetter->calcFreq();
    aDigramme->calcFreq();
    aTrigramme->calcFreq();

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
        startAnalyse(aLetter, aDigramme, aTrigramme, pathIn);

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
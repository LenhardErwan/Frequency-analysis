#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

#include "Analyse.hpp"

/**
 * @brief Créer un objet Analyse pour connaitre la fréquence des lettres a-z
 * 
 * @param pathIn : Chemin vers le fichier à analyser
 * @return Analyse* : Pointeur sur un l'objet Analyse
 */
Analyse* generateLetterAnalyse(std::string pathIn) {
    std::map<std::string, float> * map = new std::map<std::string, float>; //Créer une map avec pour clé les caractères et pour valeur leur itération

    for (char letter = 'a'; letter <= 'z'; ++letter) {  //Permet d'avoir tout l'alphabet en minuscule
        map->emplace(std::string(1, letter), 0.0f);    //rempli la map avec l'alphabet initialisé à 0
    }

    return new Analyse(map, pathIn);
}

/**
 * @brief Créer un objet Analyse pour connaitre la fréquence des digrammes aa-zz
 * 
 * @param pathIn : Chemin vers le fichier à analyser
 * @return Analyse* : Pointeur sur un l'objet Analyse
 */
Analyse* generateDigrammeAnalyse(std::string pathIn) {
    std::map<std::string, float> * map = new std::map<std::string, float>; //Créer une map avec pour clé les caractères et pour valeur leur itération

    for (char letter1 = 'a'; letter1 <= 'z'; ++letter1) { 
        for(char letter2 = 'a';  letter2 <= 'z'; ++letter2) {
            map->emplace(std::string() + letter1 + letter2, 0.0f);    //rempli la map avec les digrammes initialisés à 0
        }
    }

    return new Analyse(map, pathIn);
}

/**
 * @brief Créer un objet Analyse pour connaitre la fréquence des trigrammes aaa-zzz
 * 
 * @param pathIn : Chemin vers le fichier à analyser
 * @return Analyse* : Pointeur sur un l'objet Analyse
 */
Analyse* generateTrigrammeAnalyse(std::string pathIn) {
    std::map<std::string, float> * map = new std::map<std::string, float>; //Créer une map avec pour clé les caractères et pour valeur leur itération

    for (char letter1 = 'a'; letter1 <= 'z'; ++letter1) { 
        for(char letter2 = 'a';  letter2 <= 'z'; ++letter2) {
            for(char letter3 = 'a';  letter3 <= 'z'; ++letter3) {
                map->emplace(std::string() + letter1 + letter2 + letter3, 0.0f);    //rempli la map avec les digrammes initialisé à 0
            }
        }
    }

    return new Analyse(map, pathIn);
}

/**
 * @brief Ecris à la suite d'un fichier les résulats de l'analyse
 * 
 * @param a : pointeur sur l'objet Analyse
 * @param path : chemin vers le fichier de sortie
 */
void printAnalyse(Analyse * a, std::string path) {
	std::ofstream fic;
    fic.open(path, std::ios::out | std::ios::app);	//Creer un ostream avec ce buffer

    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur
    
    fic << (*a);   //Ecrit dans le fichier les données de l'Analyse

    fic.close();
}

/**
 * @brief Effectue la fréquence des lettres a-z
 * 
 * @param a : pointeur sur l'objet Analyse
 */
void freqLetter(Analyse * a) {
    std::string path = a->getPathIn();
    std::ifstream fic;
    fic.open(path, std::ios_base::in);  //Ouvre le fichier en lecture
    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur

    char c; //Caractère lu
    std::string letter;

    while (fic.get(c)) {    //Tant qu'il y a des caractères à lire
        a->incGraphene(&(letter = c));
    }
    
    a->calcFreq();//Créer les fréquences et la map finale

    fic.close();    //ferme le fichier (libère la mémoire je suppose)
}

/**
 * @brief Effectue la fréquence des digrammes aa-zz
 * 
 * @param a : pointeur sur l'objet Analyse
 */
void freqDigramme(Analyse * a) {
    std::string path = a->getPathIn();
    std::ifstream fic;
    fic.open(path, std::ios_base::in);  //Ouvre le fichier en lecture
    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur

    char c; //Caractère lu
    char old = fic.get();
    std::string digramme = "";  //Variable temporaire qui prend le caractère extrait + le suivant (non  extrait)

    while (fic.get(c)) {    //Tant que il y a des caractères a lire
        c = tolower(c);  //met en minuscule le caractère testé
        digramme = std::string() + old + c;
        if(a->incGraphene(&digramme)) {  //Si le caractère existe dans la map
            fic.get(c); //On passe au caractère suivant (évite de prendre une lettre deux fois pour un digramme)
        }
        old = c;
    }

    a->calcFreq();   //Créer les fréquence et la map finale

    fic.close();
}

/**
 * @brief Effectue la fréquence des trigrammes aaa-zzz
 * 
 * @param a : pointeur sur l'objet Analyse
 */
void freqTrigramme(Analyse * a) {
    std::string path = a->getPathIn();
    std::ifstream fic;
    fic.open(path, std::ios_base::in);  //Ouvre le fichier en lecture
    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur

    char c; //Caractère lu (dernier caractère)
    char old2 = tolower(fic.get());    // sur trois caractère c'est le premier
    char old = tolower(fic.get());     // sur trois caractère c'est le second
    std::string * trigramme = new std::string("");  //Variable temporaire qui prend le caractère extrait + le suivant (non  extrait)

    while (fic.get(c)) {    //Tant que il y a des caractères a lire
        c = tolower(c);  //met en minuscule le caractère testé
        (*trigramme) = std::string() + old2 + old + c;
        if(a->incGraphene(trigramme)) {  //Si le caractère existe dans la map
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
    if (argc != 2) { // verification de la présence des arguments
        std::cerr << "Commandes : " << argv[0] << " <path_to_file>" << std::endl;
    } else {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> temps;
        
        auto end = std::chrono::system_clock::now(); 
        std::time_t date_actuelle = std::chrono::system_clock::to_time_t(end);
        std::string date_heure =  std::ctime(&date_actuelle);
    
        for (size_t i=0; i < date_heure.length(); ++i) {
            if (date_heure[i] == ':') {
                date_heure[i] = '-';
            }
            if (date_heure[i] == ' ') {
                date_heure.replace (i,1,"_");
            }
        }
        date_heure.erase(date_heure.length()-1, 1);
        date_heure.append(".log");

        std::string pathIn = argv[1];    // argument 1, chemin du fichier à analyser
        std::string pathOut = date_heure;    // le fichier resultat prend pour nom l'heure et la date actuelle

        Analyse * aLetter = generateLetterAnalyse(pathIn);
        Analyse * aDigramme = generateDigrammeAnalyse(pathIn);
        Analyse * aTrigramme = generateTrigrammeAnalyse(pathIn);

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        temps = t2 - t1;
        std::cout << "Le temps d'initialisation est de : " << temps.count() << "ms" <<std::endl;
        
        try {
            freqLetter(aLetter);
            freqDigramme(aDigramme);
            freqTrigramme(aTrigramme);

            std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();
            temps = t3 - t2;
            std::cout << "Le temps d'analyse est de : " << temps.count() << "ms" <<std::endl;

            printAnalyse(aLetter, pathOut);
            printAnalyse(aDigramme, pathOut);
            printAnalyse(aTrigramme, pathOut);

            std::chrono::high_resolution_clock::time_point t4 = std::chrono::high_resolution_clock::now();
            temps = t4 - t3;
            std::cout << "Le temps d'écriture est de : " << temps.count() << "ms" <<std::endl;
            
        }
        catch (std::out_of_range & e) { //Si le fichier ne peut pas s'ouvrir
            std::cerr << e.what() << std::endl;
        }
        
        delete aLetter;         //Libère la mémoire
        delete aDigramme;
        delete aTrigramme;

        std::chrono::high_resolution_clock::time_point t5 = std::chrono::high_resolution_clock::now();
        temps = t5 - t1;
        std::cout << "Le temps d'exécution total est de : " << temps.count() << "ms" <<std::endl;

    }
    return EXIT_SUCCESS;    //FIN
}
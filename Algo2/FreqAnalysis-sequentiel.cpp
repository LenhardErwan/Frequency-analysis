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
Analyse* generateLetterAnalyse() {
    std::map<std::string, float> * map = new std::map<std::string, float>; //Créer une map avec pour clé les caractère et pour valeur leur itération

    for (char letter = 'a'; letter <= 'z'; ++letter) {  //Permet d'avoir tout l'alphabet en minuscule
        map->emplace(std::string(1, letter), 0.0f);    //rempli la map avec l'alaphabet initialisé à 0
    }

    return new Analyse(map);
}

/**
 * @brief Créer un objet Analyse pour connaitre la fréquence des digrammes aa-zz
 * 
 * @param pathIn : Chemin vers le fichier à analyser
 * @return Analyse* : Pointeur sur un l'objet Analyse
 */
Analyse* generateDigrammeAnalyse() {
    std::map<std::string, float> * map = new std::map<std::string, float>; //Créer une map avec pour clé les caractère et pour valeur leur itération

    for (char letter1 = 'a'; letter1 <= 'z'; ++letter1) { 
        for(char letter2 = 'a';  letter2 <= 'z'; ++letter2) {
            map->emplace(std::string() + letter1 + letter2, 0.0f);    //rempli la map avec les digrammes initialisé à 0
        }
    }

    return new Analyse(map);
}

/**
 * @brief Créer un objet Analyse pour connaitre la fréquence des trigrammes aaa-zzz
 * 
 * @param pathIn : Chemin vers le fichier à analyser
 * @return Analyse* : Pointeur sur un l'objet Analyse
 */
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
 * @brief Permet de faire l'analyse d'un buffer
 * 
 * @param aLetter Analyse pour les lettres
 * @param aDigramme Analyse pour les digramme
 * @param aTrigramme Analyse pour les trigrammes
 * @param buffer buffer à analyser
 */
void startAnalyse(Analyse* aLetter, Analyse* aDigramme, Analyse* aTrigramme, std::streambuf * buffer) {

    char c; //Caractère lut
    std::string letter ="", dig = "", trig = "";

    while (buffer->sgetc() != EOF ) {    //Caractère actuel

        //sbumpc() récupère le caractère actuel et incrémente la position dans le buffer
        c = (char) tolower(buffer->sbumpc());  //met en minuscule le caractère tester

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

        Analyse * aLetter = generateLetterAnalyse();
        Analyse * aDigramme = generateDigrammeAnalyse();
        Analyse * aTrigramme = generateTrigrammeAnalyse();

        try {
            std::ifstream fic;
            fic.open(pathIn, std::ios_base::in);  //Ouvre le fichier en lecture
            if( !fic.is_open() )    //Si le fichier n'est pas ouvert
                throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + pathIn + "\" !"); //Alors on émet une erreur

            std::streambuf * buffer = fic.rdbuf();

            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            temps = t2 - t1;
            std::cout << "Le temps d'initialisation est de : " << temps.count() << "ms" <<std::endl;
            
            startAnalyse(aLetter, aDigramme, aTrigramme, buffer);

            aLetter->calcFreq();
            aDigramme->calcFreq();
            aTrigramme->calcFreq();

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
        catch (std::out_of_range & e) { //Si le fichier ne peux pas s'ouvrire
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
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <time.h>

/**
 * @brief Modofie la map passé en paramètre, calcul les fréquence de chaque clé
 * @param std::map<std::string, float> * map : pointeur sur la map a modifier
 * @param unsigned long nbChar : Nombre de caratère reconnu dans le fichier, diviseur pour faire la fréquence.  
 */
void calcFrequence(std::map<std::string, float> * map, unsigned long nbChar) {
    for (std::map<std::string, float>::iterator it = map->begin(); it != map->end(); ++it) {    //Parcours toute la map
        it->second = it->second / nbChar;  //Pour chaque élément on calcul sa fréquence
    }
}

/**
 * @brief Permet d'avoir les fréquence des lettre de l'alphabet sur un fichier
 * @param unsigned long& nbIte : Nombre d'itérations effectuées (nombre de caratères analysés)
 * @param std::string path : chemin vers le fichier depuis l'éxecutable
 * @return std::map<std::string, float>* : map qui contient pour clé le caractère et pour valeur sa fréquence 
 */
std::map<std::string, float>* freqLetter(/*out*/ unsigned long& nbIte, std::string path) {
    std::ifstream fic;
    fic.open(path, std::ios_base::in);  //Ouvre le fichier en lecture
    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur

    std::map<std::string, float> * map = new std::map<std::string, float>; //Créer une map avec pour clé les caractère et pour valeur leur itération

    for (char letter = 'a'; letter <= 'z'; ++letter) {  //Permet d'avoir tout l'alphabet en minuscule
        map->emplace(std::string(1, letter), 0.0f);    //rempli la map avec l'alaphabet initialisé à 0
    }

    char c; //Caractère lut
    unsigned long count = 0;    //Compte le nombre de caractère pris en compte dans l'analyse
    std::map<std::string, float>::iterator it;    //créé un itérateur sur la map

    while (fic.get(c)) {    //Tant que il y a des caractères a lire
        c = (char) tolower(c);  //met en minuscule le caractère testé
        it = map->find(std::string(1, c));  //L'itérateur prend la valeur de la case de la map qui contient le caractère s'il existe, sinon il prend la fin de la liste
        if(it != map->end()) {  //Si le caractère existe dans la map
            ++it->second;   //Incrémente la valeur du caractère
            ++count;    //Incrémente le nombre de caractère pris en compte dans l'analyse
        }
    }
    
    nbIte = count;  //Le nombre passé en paramètre prend la valeur du nombre de caractères analysés pris en compte
    calcFrequence(map, count);    //Créer les fréquence et la map finale

    fic.close();    //ferme le fichier (libère la mémoire je suppose)
    return map;     //delete la map temporaire
}


std::map<std::string, float>* freqDigramme(/*out*/ unsigned long& nbIte, std::string path) {
    std::ifstream fic;
    fic.open(path, std::ios_base::in);  //Ouvre le fichier en lecture
    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur

    std::map<std::string, float> * map = new std::map<std::string, float>;  //Créer une map avec pour clé les caractère et pour valeur leur itération

    //Génère les digrammes
    for (char letter1 = 'a'; letter1 <= 'z'; ++letter1) { 
        for(char letter2 = 'a';  letter2 <= 'z'; ++letter2) {
            map->emplace(std::string() + letter1 + letter2, 0.0f);    //rempli la map avec les digrammes initialisé à 0
        }
    }

    char c; //Caractère lut
    char old = fic.get();
    std::string digramme = "";  //Variable temporaire qui prend le caractère extrait + le suivant (non  extrait)
    unsigned long count = 0;    //Compte le nombre de caractère pris en compte dans l'analyse
    std::map<std::string, float>::iterator it;    //créé un itérateur sur la map

    while (fic.get(c)) {    //Tant que il y a des caractères a lire
        c = tolower(c);  //met en minuscule le caractère testé
        digramme = std::string() + old + c;
        it = map->find(digramme);  //L'itérateur prend la valeur de la case de la map qui contient le caractère s'il existe, sinon il prend la fin de la liste
        if(it != map->end()) {  //Si le caractère existe dans la map
            ++it->second;   //Incrémente la valeur du caractère
            ++count;    //Incrémente le nombre de caractère pris en compte dans l'analyse
            fic.get(c); //On passe au caractère suivant (évite de prendre un lettre deux fois pour un digramme)
        }
        old = c;
    }

    nbIte = count;  //Le nombre passé en paramètre prend la valeur du nombre de caractères analysés pris en compte
    calcFrequence(map, count);    //Créer les fréquence et la map finale

    fic.close();
    return map;
}

std::map<std::string, float>* freqTrigramme(/*out*/ unsigned long& nbIte, std::string path) {
    std::ifstream fic;
    fic.open(path, std::ios_base::in);  //Ouvre le fichier en lecture
    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur

    std::map<std::string, float> * map = new std::map<std::string, float>;  //Créer une map avec pour clé les caractère et pour valeur leur itération

    //Génère les Trigrammes
    for (char letter1 = 'a'; letter1 <= 'z'; ++letter1) { 
        for(char letter2 = 'a';  letter2 <= 'z'; ++letter2) {
            for(char letter3 = 'a';  letter3 <= 'z'; ++letter3) {
                map->emplace(std::string() + letter1 + letter2 + letter3, 0.0f);    //rempli la map avec les digrammes initialisé à 0
            }
        }
    }

    char c; //Caractère lut (dernier caractère)
    char old2 = tolower(fic.get());    // sur trois caractère c'est le premier
    char old = tolower(fic.get());     // sur trois caractère c'est le second
    std::string trigramme = "";  //Variable temporaire qui prend le caractère extrait + le suivant (non  extrait)
    unsigned long count = 0;    //Compte le nombre de caractère pris en compte dans l'analyse
    std::map<std::string, float>::iterator it;    //créé un itérateur sur la map

    while (fic.get(c)) {    //Tant que il y a des caractères a lire
        c = tolower(c);  //met en minuscule le caractère testé
        trigramme = std::string() + old2 + old + c;
        it = map->find(trigramme);  //L'itérateur prend la valeur de la case de la map qui contient le caractère s'il existe, sinon il prend la fin de la liste
        if(it != map->end()) {  //Si le caractère existe dans la map
            ++it->second;   //Incrémente la valeur du caractère
            ++count;    //Incrémente le nombre de caractère pris en compte dans l'analyse
            fic.get(c);
            old = c;
            fic.get(c);  //Saute 2 caractère (evite de re utiliser les 3 caractère que l'ont viens d'utiliser)
        }
        old2 = old;
        old = c;
    }

    nbIte = count;  //Le nombre passé en paramètre prend la valeur du nombre de caractères analysés pris en compte
    calcFrequence(map, count);    //Créer les fréquence et la map finale

    fic.close();
    return map;
}


void printMap(std::map<std::string, float> * map, int nbChar, std::string path) {
	std::ofstream fic;
    fic.open(path, std::ios::out | std::ios::app);	//Creer un ostream avec ce buffer

    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur
    //A ecrire dans un fichier
    fic << "Taille de la map " << map->size() << ", Nombre de graphènes trouvés " << nbChar << std::endl; //Affiche la taille de la map et le nombre de caractère analysé pris en compte

    for (std::map<std::string, float>::iterator it = map->begin(); it != map->end(); ++it) {    //Parcours de la liste
        fic << it->first << " à pour valeur " << it->second << std::endl; //Pour chaque élément on affiche le caractère et sa fréquence
    }
    fic << std::endl << std::endl;

    fic.close();
}


int main() {
    clock_t t1=clock();
    std::remove("./result.txt");    //Supprime le fichier de resultat s'il existe déja

    std::string pathIn = "./grosfichier.txt";    //Il faudrai le passé en argument du programme
    std::string pathOut = "./result.txt";    //Il faudrai le passé en argument du programme
    unsigned long nbLetter, nbDigramme, nbTrigramme;
    std::map<std::string, float> * freqLet = nullptr;
    std::map<std::string, float> * freqDig = nullptr;
    std::map<std::string, float> * freqTrig = nullptr;

    try {
        freqLet = freqLetter(nbLetter, pathIn);   //Récupère la map avec les fréquences du fichier
        printMap(freqLet, nbLetter, pathOut);
        
        freqDig = freqDigramme(nbDigramme, pathIn);
        printMap(freqDig, nbDigramme, pathOut);

        freqTrig = freqTrigramme(nbTrigramme, pathIn);
        printMap(freqTrig, nbTrigramme, pathOut);
        
    }
    catch (std::out_of_range & e) { //Si le fichier ne peux pas s'ouvrire
        std::cerr << e.what() << std::endl;
    }
    
    delete freqLet;         //Libère la mémoire
    delete freqDig;
    delete freqTrig;

    clock_t t2= clock();
    float temps = (float)(t2-t1)/CLOCKS_PER_SEC;
    std::cout << "Le temps d'exécution total est de : " << temps << "s" <<std::endl;

    return EXIT_SUCCESS;    //FIN
}



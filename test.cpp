#include <string>
#include <iostream>
#include <fstream>
#include <map>

/**
 * @brief calcul la fréquence de chaque élément d'une map
 * @param std::map<char, int>::iterator begin : pointeur sur le premier élément de la map
 * @param std::map<char, int>::iterator end : pointeur sur le dernier élément de la map
 * @param unsigned long nbChar : Nombre de caratère reconnu dans le fichier, diviseur pour faire la fréquence.  
 * @return std::map<char, float>* : map avec le caractère et sa fréquence
 */
std::map<char, float>* calcFrequence(std::map<char, int>::iterator begin, std::map<char, int>::iterator end, unsigned long nbChar) {
    std::map<char, float>* map = new std::map<char, float>; //Créer une map

    for (std::map<char, int>::iterator it = begin; it != end; ++it) {    //Parcours toute la map
        map->emplace(it->first, (float) it->second / nbChar );  //Pour chaque élément on calcul sa fréquence
    }

    return map; //retourne la map avec les fréquence
}

std::map<char, float>* freqLetter(unsigned long & nbIte, std::string path) {
    std::ifstream fic;
    fic.open(path, std::ios_base::in);  //Ouvre le fichier en lecture
    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur

    std::map<char, int> * map = new std::map<char, int>;  //Créer une map avec pour clé les caractères et pour valeur leur itération

    for (char letter = 'a'; letter <= 'z'; ++letter) {  //Permet d'avoir tout l'alphabet en minuscule
        map->emplace(letter, 0);    //rempli la map avec l'alaphabet initialisé à 0
    }

    /////////////////////////////////////
    //Ceci fonctionne ^^
    map->emplace('ç', 0);
    map->emplace('🙃', 0);
    /////////////////////////////////////

    char c; //Caractère lut
    unsigned long count = 0;    //Compte le nombre de caractère pris en compte dans l'analyse
    std::map<char, int>::iterator it;    //créé un itérateur sur la map
    while (fic.get(c)) {    //Tant que il y a des caractères a lire
        it = map->find(c);  //L'itérateur prend la valeur de la case de la map qui contient le caractère s'il existe, sinon il prend la fin de la liste
        if(it != map->end()) {  //Si le caractère existe dans la map
            ++it->second;   //Incrémente la valeur du caractère
            ++count;    //Incrémente le nombre de caractère pris en compte dans l'analyse
        }
    }
    
    nbIte = count;  //Le nombre passé en paramètre prend la valeur du nombre de caractères analysés pris en compte
    fic.close();    //ferme le fichier (libère la mémoire je suppose)

    std::map<char, float>* fmap = calcFrequence(map->begin(), map->end(), count);    //Créer les fréquence et la map finale

    delete map;     //delete la map temporaire
    return fmap;    //retourne la map avec les fréquence
}


int main() {
    std::string path = "./test.txt";    //Il faudrai le passé en argument du programme
    unsigned long nbIte = 0;  //Variable qui prend le nombre d'itération
    std::map<char, float> * freqLet;

    try {
        freqLet = freqLetter(nbIte, path);   //Récupère la map avec les fréquences du fichier
        std::cout << freqLet->size() << ", nbCar = " << nbIte << std::endl; //Affiche la taille de la map et le nombre de caractère analysé pris en compte

        for (std::map<char, float>::iterator it = freqLet->begin(); it != freqLet->end(); ++it) {    //Parcours de la liste
            std::cout << it->first << " has value " << it->second << std::endl; //Pour chaque élément on affiche le caractère et sa fréquence
        }
    }
    catch (std::out_of_range & e) { //Si le fichier ne peux pas s'ouvrire
        std::cerr << e.what() << std::endl;
    }
    
    delete freqLet;         //Libère la mémoire
    return EXIT_SUCCESS;    //FIN
}



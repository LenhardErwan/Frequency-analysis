#include <string>
#include <iostream>
#include <fstream>
#include <map>

/**
 * @brief calcul la fréquence de chaque élément d'une map
 * @param std::map<std::string, int>::iterator begin : pointeur sur le premier élément de la map
 * @param std::map<std::string, int>::iterator end : pointeur sur le dernier élément de la map
 * @param unsigned long nbChar : Nombre de caratère reconnu dans le fichier, diviseur pour faire la fréquence.  
 * @return std::map<std::string, float>* : map avec le caractère et sa fréquence
 */
std::map<std::string, float>* calcFrequence(std::map<std::string, int>::iterator begin, std::map<std::string, int>::iterator end, unsigned long nbChar) {
    std::map<std::string, float>* map = new std::map<std::string, float>;   //Créer une map

    for (std::map<std::string, int>::iterator it = begin; it != end; ++it) {    //Parcours toute la map
        map->emplace(it->first, (float) it->second / nbChar );  //Pour chaque élément on calcul sa fréquence
    }

    return map; //retourne la map avec les fréquence
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

    std::map<std::string, int> * map = new std::map<std::string, int>;  //Créer une map avec pour clé les caractère et pour valeur leur itération

    for (char letter = 'a'; letter <= 'z'; ++letter) {  //Permet d'avoir tout l'alphabet en minuscule
        map->emplace(std::string(1, letter), 0);    //rempli la map avec l'alaphabet initialisé à 0
    }

    /////////////////////////////////////
    //Ceci fonctionne ^^ MAIS bon le prof en a pas besoin
    //map->emplace(std::string(1, 'ç'), 0);
    //map->emplace(std::string(1, '🙃'), 0);
    /////////////////////////////////////

    char c; //Caractère lut
    unsigned long count = 0;    //Compte le nombre de caractère pris en compte dans l'analyse
    std::map<std::string, int>::iterator it;    //créé un itérateur sur la map

    while (fic.get(c)) {    //Tant que il y a des caractères a lire
        it = map->find(std::string(1, c));  //L'itérateur prend la valeur de la case de la map qui contient le caractère s'il existe, sinon il prend la fin de la liste
        if(it != map->end()) {  //Si le caractère existe dans la map
            ++it->second;   //Incrémente la valeur du caractère
            ++count;    //Incrémente le nombre de caractère pris en compte dans l'analyse
        }
    }
    
    nbIte = count;  //Le nombre passé en paramètre prend la valeur du nombre de caractères analysés pris en compte
    std::map<std::string, float>* fmap = calcFrequence(map->begin(), map->end(), count);    //Créer les fréquence et la map finale

    fic.close();    //ferme le fichier (libère la mémoire je suppose)
    delete map;     //delete la map temporaire
    return fmap;    //retourne la map avec les fréquence
}


std::map<std::string, float>* freqDigramme(/*out*/ unsigned long& nbIte, std::string path) {
    std::ifstream fic;
    fic.open(path, std::ios_base::in);  //Ouvre le fichier en lecture
    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur

    std::map<std::string, int> * map = new std::map<std::string, int>;  //Créer une map avec pour clé les caractère et pour valeur leur itération

    //Génère les digrammes
    for (char letter1 = 'a'; letter1 <= 'z'; ++letter1) { 
        for (char letter2 = 'a'; letter2 <= 'z'; ++letter2) {
            map->emplace(std::string() + letter1 + letter2, 0);    //rempli la map avec les digrammes initialisé à 0
        }
    }

    char c; //Caractère lut
    std::string digramme = "";  //Variable temporaire qui prend le caractère extrait + le suivant (non  extrait)
    unsigned long count = 0;    //Compte le nombre de caractère pris en compte dans l'analyse
    std::map<std::string, int>::iterator it;    //créé un itérateur sur la map

    while (fic.get(c)) {    //Tant que il y a des caractères a lire
        digramme = std::string() + c + (char) fic.peek();
        it = map->find(digramme);  //L'itérateur prend la valeur de la case de la map qui contient le caractère s'il existe, sinon il prend la fin de la liste
        if(it != map->end()) {  //Si le caractère existe dans la map
            ++it->second;   //Incrémente la valeur du caractère
            ++count;    //Incrémente le nombre de caractère pris en compte dans l'analyse
        }
    }

    nbIte = count;  //Le nombre passé en paramètre prend la valeur du nombre de caractères analysés pris en compte
    std::map<std::string, float>* fmap = calcFrequence(map->begin(), map->end(), count);    //Créer les fréquence et la map finale

    fic.close();
    delete map;
    return fmap;
}


int main() {
    std::string path = "./test.txt";    //Il faudrai le passé en argument du programme
    unsigned long nbIte = 0;  //Variable qui prend le nombre d'itération
    std::map<std::string, float> * freqLet;
    std::map<std::string, float> * freqDig;

    try {
        freqLet = freqLetter(nbIte, path);   //Récupère la map avec les fréquences du fichier
        
        //A ecrire dans un fichier
        /*std::cout << freqLet->size() << ", nbCar = " << nbIte << std::endl; //Affiche la taille de la map et le nombre de caractère analysé pris en compte

        for (std::map<std::string, float>::iterator it = freqLet->begin(); it != freqLet->end(); ++it) {    //Parcours de la liste
            std::cout << it->first << " has value " << it->second << std::endl; //Pour chaque élément on affiche le caractère et sa fréquence
        }
        */

        freqDig = freqDigramme(nbIte, path);
        //A ecrire dans un fichier
        /*std::cout << freqLet->size() << ", nbCar = " << nbIte << std::endl; //Affiche la taille de la map et le nombre de caractère analysé pris en compte

        for (std::map<std::string, float>::iterator it = freqLet->begin(); it != freqLet->end(); ++it) {    //Parcours de la liste
            std::cout << it->first << " has value " << it->second << std::endl; //Pour chaque élément on affiche le caractère et sa fréquence
        }
        */
    }
    catch (std::out_of_range & e) { //Si le fichier ne peux pas s'ouvrire
        std::cerr << e.what() << std::endl;
    }
    
    delete freqLet;         //Libère la mémoire
    delete freqDig;
    return EXIT_SUCCESS;    //FIN
}



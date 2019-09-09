#include <string>
#include <iostream>
#include <fstream>
#include <map>

void openFileIn(std::string path, std::ifstream& fic) {   //return un fichier en lecture
    fic.open(path, std::ios_base::in);  //Ouvre le fichier en lecture
    if( !fic.is_open() )    //Si le fichier n'est pas ouvert
        throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + path + "\" !"); //Alors on émet une erreur
}

std::map<std::string, float>* calcFrequence(std::map<std::string, int>::iterator begin, std::map<std::string, int>::iterator end, unsigned long nbChar) {
    std::map<std::string, float>* map = new std::map<std::string, float>;

    for (std::map<std::string, int>::iterator it = begin; it != end; ++it) {
        map->emplace(it->first, (float) it->second / nbChar );
    }

    return map;
}

std::map<std::string, float>* freqLetter(unsigned long & nbIte, std::string path) {
    std::ifstream fic;

    try {
        openFileIn(path, fic);  //Ouvre le fichier en lecture
    } catch (std::out_of_range & e) {
        std::cerr << e.what() << std::endl;
    }

    std::map<std::string, int> * map;  //créer un pointeur sur map
    map = new std::map<std::string, int>;  //Créer une map avec pour clé les caractère et pour valeur leur itération

    for (char letter = 'a'; letter <= 'z'; ++letter) {  //Permet d'avoir tout l'alphabet en minuscule
        map->emplace(std::string(1, letter), 0);
    }

    /////////////////////////////////////
    //Ceci fonctionne ^^
    map->emplace(std::string(1, 'ç'), 0);
    map->emplace(std::string(1, '🙃'), 0);
    /////////////////////////////////////

    char c; //Caractère lut
    unsigned long count = 0;
    std::map<std::string, int>::iterator it;
    while (fic.get(c)) {    //Tant que il y a des caractères a lire
        it = map->find(std::string(1, c));
        if(it != map->end()) {
            ++it->second;
            ++count;
        }
    }
    
    nbIte = count;
    fic.close();    

    std::map<std::string, float>* fmap = calcFrequence(map->begin(), map->end(), count);

    delete map;
    return fmap;
}


int main() {
    std::string path = "./test.txt";    //Il faudrai le passé en argument du programme
    unsigned long nbIte = 0;  //Variable qui prend le nombre d'itération

    std::map<std::string, float> * freqLet = freqLetter(nbIte, path);
    std::cout << freqLet->size() << ", nbCar = " << nbIte << std::endl;

    for (std::map<std::string, float>::iterator it = freqLet->begin(); it != freqLet->end(); ++it) {
        std::cout << it->first << " has value " << it->second << std::endl;
    }
    
    delete freqLet;
    return EXIT_SUCCESS;
}



#include "Analyse.hpp"

Analyse::Analyse() {
    nbGraphene = 0;
    map = new std::map<std::string, float>;
}

Analyse::Analyse(std::map<std::string, float>* _map) {
    nbGraphene = 0;
    map = _map;
}

Analyse::~Analyse() {
    delete map;
}

void Analyse::calcFreq() {
    for (std::map<std::string, float>::iterator it = map->begin(); it != map->end(); ++it) {    //Parcours toute la map
        it->second = it->second / nbGraphene;  //Pour chaque élément on calcul sa fréquence
    }
}

bool Analyse::incGraphene(std::string* graphene) {
    std::map<std::string, float>::iterator it = map->find( *graphene );    //créé un itérateur sur la map

    if(it != map->end()) {  //Si le graphène existe dans la map
        ++it->second;   //Incrémente la valeur du graphène
        ++nbGraphene;   //Incrément le nombre de graphènes analysés
        return true;
    }
    else
        return false;

}


std::ostream& operator<<(std::ostream& os, const Analyse & a) {
    os << "Taille de la map " << a.map->size() << ", Nombre de graphènes trouvés " << a.nbGraphene << std::endl; //Affiche la taille de la map et le nombre de caractère analysé pris en compte

    for (std::map<std::string, float>::iterator it = a.map->begin(); it != a.map->end(); ++it) {    //Parcours de la liste
        os << it->first << " à pour valeur " << it->second << std::endl; //Pour chaque élément on affiche le caractère et sa fréquence
    }
    os << std::endl;

    return os;
}
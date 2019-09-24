#include <map>
#include <iostream>

class Analyse {
    private:
        unsigned long nbGraphene;               //Nombre de graphène trouvé dans le fichier
        std::string pathIn;                     //Chemin vers le fichier à analyser
        std::map<std::string, float>* map;      //Map qui contient les graphène en clé et les fréquence en valeur

    public:
        Analyse();
        Analyse(std::map<std::string, float>* map, std::string pathIn);
        ~Analyse();

        void calcFreq();
        bool incGraphene(std::string* graphene);

        inline unsigned long getNbGraphene() {return nbGraphene;}
        inline const std::string getPathIn() {return pathIn;}
        inline std::map<std::string, float>* getMap() {return map;}

        inline void setPathIn(std::string _pathIn) {pathIn = _pathIn;}

        friend std::ostream& operator<<(std::ostream& os, const Analyse & a);
};

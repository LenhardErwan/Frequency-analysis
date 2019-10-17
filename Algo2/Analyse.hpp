#include <map>
#include <iostream>

/**
 * @brief Classe pour créer des Analyses
 * @class Analyse Analyse.hpp
 */
class Analyse {
    private:
        /**
         * @var unsigned long nbGrahphene
         * @brief nombre de graphènes trouvés dans un fichier
         * 
         */
        unsigned long nbGraphene;

        /**
         * @var std::map<std::string, float>* map
         * @brief pointeur sur une map avec pour clé le graphène et pour valeur sa fréquence
         * 
         */
        std::map<std::string, float>* map;

    public:
        /**
         * @brief Construct a new Analyse object
         * 
         */
        Analyse();

        /**
         * @brief Construct a new Analyse object
         * 
         * @param map : pointeur sur une map déja existante
         * @param pathIn : chemin vers le fichier à analyser
         */
        Analyse(std::map<std::string, float>* map);

        /**
         * @brief Destroy the Analyse object
         * 
         */
        ~Analyse();


        /**
         * @brief Calcul la fréquence des graphènes
         * 
         */
        void calcFreq();

        /**
         * @brief Incrémente le nombre d'itération du graphène dans la map
         * 
         * @param graphene : graphène à incrémenter
         * @return true : Si le graphène est présent dans la map
         * @return false : sinon
         */
        bool incGraphene(std::string* graphene);


        /**
         * @brief Avoir le nombre de graphène trouvé
         * 
         * @return unsigned long 
         */
        inline unsigned long getNbGraphene() {return nbGraphene;}

        /**
         * @brief Avoir un pointeur sur la map qui contient les graphènes et leur fréquence
         * 
         * @return std::map<std::string, float>* 
         */
        inline std::map<std::string, float>* getMap() {return map;}

        /**
         * @brief Permet d'écrire l'objet dans un un flux ostream
         * 
         * @param os 
         * @param a 
         * @return std::ostream& 
         */
        friend std::ostream& operator<<(std::ostream& os, const Analyse & a);
};

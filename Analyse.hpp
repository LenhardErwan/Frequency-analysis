#include <map>
#include <iostream>

class Analyse {
    private:
        unsigned long nbGraphene;
        std::map<std::string, float>* map;

    public:
        Analyse();
        Analyse(std::map<std::string, float>* map);
        ~Analyse();

        void calcFreq();
        bool incGraphene(std::string graphene);

        inline unsigned long getNbGraphene() {return nbGraphene;}
        inline const std::map<std::string, float>* getMap() {return map;}

        friend std::ostream& operator<<(std::ostream& os, const Analyse & a);
};

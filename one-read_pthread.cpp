#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <thread>

#include "Analyse.hpp"

sem_t semLetter, semDigramme, semTrigramme;

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
 * @brief Structure qui permet de définir les paramètre de la fonction startAnalyse()
 * @var aLetter : pointeur vers l'objet Analyse pour les lettres
 * @var aDigramme : pointeur vers l'objet Analyse pour les digrammes
 * @var aTrigramme : pointeur vers l'objet Analyse pour les trigrammes
 * @var buffer : pointeur sur un std::streambuf, qui contient une partie du fichier à analyser
 */
typedef struct {
    Analyse* aLetter;
    Analyse* aDigramme;
    Analyse* aTrigramme;
    std::streambuf* buffer;
} paramAnalyse;

bool incGraphene(Analyse * a, std::string * graphene, sem_t * semaphore) {
    //Bloque l'accès si dispo sinon attend
    sem_wait(semaphore);

    //incrémente le graphène
    bool ret = a->incGraphene(graphene);

    //débloque 
    sem_post(semaphore);

    return ret;
}

/**
 * @brief permet de lire un buffer et d'incrémenter les graphènes trouvés
 * 
 * @param param : Structure avec tous les paramètre à l'intérieur
 */
void* startAnalyse(void* _param) {
    paramAnalyse * param = (paramAnalyse*) _param;

    char c; //Caractère lut
    std::string letter ="", dig = "", trig = "";

    while (param->buffer->sgetc() != EOF ) {    //Caractère actuel

        //sbumpc() récupère le caractère actuel et incrémente la position dans le buffer
        c = (char) tolower(param->buffer->sbumpc());  //met en minuscule le caractère tester

        //Pour les caractères uniquement
        incGraphene(param->aLetter, &(letter = c), &semLetter);

        //Pour les digrammes
        dig = dig + c;
        if(dig.length() >= 2) {
            if(incGraphene(param->aDigramme, (&dig), &semDigramme))
                dig = "";   //remet le graphène à zero (evite d'utiliser un caractère dans deux digrammes)
            else
                dig.erase(0,1);   //Efface le premier caractère du digramme
        }

        //Pour les trigrammes
        trig = trig + c;
        if(trig.length() >= 3) {
            if(incGraphene(param->aTrigramme, (&trig), &semTrigramme))
                trig = "";   //remet le graphène à zero (evite d'utiliser un caractère dans deux trigrammes)
            else
                trig.erase(0,1);   //Efface le premier caractère du trigramme
        }
        
    }

    pthread_exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (argc != 2) { // verification de la présence des arguments
        std::cerr << "Commandes : " << argv[0] << " <path_to_file>" << std::endl;
    } else {
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
        date_heure.append(".txt");

        std::string pathIn = argv[1];    // argument 1, chemin du fichier à analyser
        std::string pathOut = date_heure;    // le fichier resultat prend pour nom l'heure et la date actuelle

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        Analyse * aLetter = generateLetterAnalyse();
        Analyse * aDigramme = generateDigrammeAnalyse();
        Analyse * aTrigramme = generateTrigrammeAnalyse();

        sem_init(&semLetter, 0, 1);
        sem_init(&semDigramme, 0, 1);
        sem_init(&semTrigramme, 0, 1);
        const unsigned short NB_THREADS = 4;
        pthread_t * tab_thread = new pthread_t[NB_THREADS];

        std::vector<std::stringstream*> tab_buffer;

        for (size_t i = 0; i < NB_THREADS; i++) {
            tab_buffer.push_back(new std::stringstream);
        }
             
        
        unsigned long NB_LIGNES = 0;

        try {
            std::ifstream fic;
            fic.open(pathIn, std::ios_base::in);  //Ouvre le fichier en lecture
            if( !fic.is_open() )    //Si le fichier n'est pas ouvert
                throw std::ios_base::failure("Impossible d'ouvrir le fichier: \"" + pathIn + "\" !"); //Alors on émet une erreur

            //Compte le nombre de ligne
            std::string line ="";
            while(std::getline(fic,line) ) {
                ++NB_LIGNES;    //Ajouter dans le bon buffer la ligne
            }

            fic.clear();
            fic.seekg (0, std::ios::beg);
            line ="";
            unsigned long numline = 0;
            while(std::getline(fic,line) ) {
                unsigned short val = numline++ / (NB_LIGNES / NB_THREADS);
                if(val >= NB_THREADS) val = NB_THREADS - 1; //Lorsque on travail avec de grand nombre la precision de la division n'est pas bon donc on obtient le nombre de thread
                std::stringstream * ss = tab_buffer[val];

                *ss << line << std::endl;   //Ajouter dans le bon buffer la ligne
            }
            

            for (size_t i = 0; i < NB_THREADS; i++) {
                paramAnalyse * param =  new paramAnalyse{aLetter, aDigramme, aTrigramme, tab_buffer[i]->rdbuf()};
                pthread_create(&tab_thread[i], NULL, startAnalyse, (void*) param);
            }

            for (size_t i = 0; i < NB_THREADS; i++) {
                pthread_join(tab_thread[i], NULL);
            }

            aLetter->calcFreq();
            aDigramme->calcFreq();
            aTrigramme->calcFreq();

            printAnalyse(aLetter, pathOut);
            printAnalyse(aDigramme, pathOut);
            printAnalyse(aTrigramme, pathOut);
            
        }
        catch (std::out_of_range & e) { //Si le fichier ne peux pas s'ouvrire
            std::cerr << e.what() << std::endl;
        }
        
        delete aLetter;         //Libère la mémoire
        delete aDigramme;
        delete aTrigramme;

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> temps = t2 - t1;
        std::cout << "Le temps d'exécution total est de : " << temps.count() << "ms" <<std::endl;

    }

    sem_destroy(&semLetter);
    sem_destroy(&semDigramme);
    sem_destroy(&semTrigramme);

    return EXIT_SUCCESS;    //FIN
}
#ifndef GREEDY_H
#define GREEDY_H

#include "orienteering/Point.hh"
#include "orienteering/Path.hh"

class Greedy
{
public:
    Greedy();
    ~Greedy();
/*
    typedef struct
    {
        Point pos;
        float dist; //somma della distanza tra start-punto-end, usata per ordinare i punti durante l'inizializzazione
        int index; //numero del punto, serve per quando vengono ordinati perchè non ci sarà più corrispondenza tra indice della struttra e del punto
        bool in_path = false; //punto è stato utilizzato in un i-esimo percorso, reset ogni nuovo path i-esimo
        Float64 score; //punteggio associato ad ogni nodo

        //DISTANZE TRA PUNTI, usate durante l'aggiunte di punti ai path
        float dist_to_start_point;
        float dist_to_end_point;

    }Punto;

    typedef struct {
        Punto p[ MAX_NUMBER_TARGET];      //al massimo comprende tutti i punti
        int size = 0;       //lo incremento ogni volta che inserisco un punto nel path, mi serve per non stampare punti non inizializzati
        Float64 total_score; //somma del punteggio di tutti i nodi del percorso
        float total_dist;   //totale della distanza del percorso
    }Percorso;

*/

    void calc(std::list<Orienteering::Point *> points, float budget1, Orienteering::Point *start, Orienteering::Point *end);
    //void calc(std::list<Orienteering::Point *> points, float budget1, float budget2);
    Orienteering::Path *getPath1();
    Orienteering::Path *getPath2();
    float getScore();



private:

    Orienteering::Path *_path1;
    Orienteering::Path *_path2;


};


#endif

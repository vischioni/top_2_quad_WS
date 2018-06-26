#ifndef TARGET_H
#define TARGET_H

#include "simulatore_drone/support_methods.h"
#include "simulatore_drone/support_types.h"
#include "simulatore_drone/my_message.h"

#include <cmath>        /* pow sqrt */
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */
#include <random>

class Target
{

public:

    Target(int id);
    ~Target();

    // callBack per V-REP
    void callBack(const Point::ConstPtr& t);

    bool isRead();  // serve per capire se v-rep ha già comunicato le posizioni di tutti i target sui topic
    void reset();
    void setDirection(direction_t dir);
    direction_t getDirection();
    void randomDirection();
    void move(); // muove il target di un passo
    void undo(); // muove un passo indietro
    void setPosition(Point pos);
    void setPosition(float x, float y);


    Point getPosition();
    void printPosition();

private:
    const float _SIZE;
    const float _SHIFT;
    Point _pos;  // posizione di un target
    direction_t _dir;  // Direzione in cui si sposta durante la simulazione
    bool _read; //a true se v-rep ha comunicato a ros la posizione
    int _id;
};

#endif

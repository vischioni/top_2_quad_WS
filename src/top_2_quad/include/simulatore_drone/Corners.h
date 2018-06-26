#ifndef CORNERS_H
#define CORNERS_H

#include "simulatore_drone/support_methods.h"
#include "simulatore_drone/support_types.h"

class Corners
{

public:
    Corners();
    ~Corners();

    // callBack pubbliche per v-rep
    void callBackBottomLeft(const Point::ConstPtr& corner);

    Point getPosition(corner_t c); // la posizione dell'angolo s
    void setPosition(corner_t c, Point pos);

    void reset();  //reset delle cordinate degli angoli
    bool isRead();

private:
    Point _corners[4];  //array di geometry_msgs::Point per le cordinate dei 4 angoli del campo
    bool _read;
};

#endif

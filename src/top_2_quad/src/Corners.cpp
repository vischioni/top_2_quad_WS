#include "simulatore_drone/Corners.h"
#include "simulatore_drone/support_types.h"

Corners::Corners()
    : _read(false)
{
}

Corners::~Corners()
{
    // ntd
}

void Corners::callBackBottomLeft(const Point::ConstPtr& corner)
{
    // BOTTOM_LEFT
    _corners[0].x = corner->x;
    _corners[0].y = corner->y;
    _corners[0].z = 1;

    // TOP_LEFT
    _corners[1].x = corner->x;
    _corners[1].y = corner->y + ALTEZZA_CAMPO;
    _corners[1].z = 1;

    // TOP_RIGHT
    _corners[2].x = corner->x + LARGHEZZA_CAMPO;
    _corners[2].y = corner->y + ALTEZZA_CAMPO;
    _corners[2].z = 1;

    // BOTTOM_RIGHT
    _corners[3].x = corner->x + LARGHEZZA_CAMPO;
    _corners[3].y = corner->y;
    _corners[3].z = 1;


    //ho letto dei dati su un topic
    _read = true;
}

void Corners::reset()
{
    /*
    for(int i = 0; i < 4; ++i)
    {
        _corners[i].x = 0;
        _corners[i].y = 0;
        _corners[i].z = 0;
    }
    _reset = true;
    */
    _read = false;
}

Point Corners::getPosition(corner_t c)
{
    int index;

    switch (c)
    {
    case corner_t::BOTTOM_LEFT: index = 0; break;
    case corner_t::TOP_LEFT: index = 1; break;
    case corner_t::TOP_RIGHT: index = 2; break;
    case corner_t::BOTTOM_RIGHT: index = 3; break;

        default: break;
    }
    return _corners[index];
}

void Corners::setPosition(corner_t c, Point pos)
{
    int index;
    switch (c)
    {
    case corner_t::BOTTOM_LEFT: index = 0; break;
    case corner_t::TOP_LEFT: index = 1; break;
    case corner_t::TOP_RIGHT: index = 2; break;
    case corner_t::BOTTOM_RIGHT: index = 3; break;

        default: break;
    }
    _corners[index].x = pos.x;
    _corners[index].y = pos.y;
    _corners[index].z = 1;
}

bool Corners::isRead()
{
    return _read;
}

#ifndef SUPPORT_METHODS_H
#define SUPPORT_METHODS_H

#include "simulatore_drone/support_types.h"

float my_abs(float n)
{
    if(n < 0)
        return -n;
    return n;
}

float pointDist(Point p, Point q)
{
    return my_abs(sqrt( pow( ( p.x - q.x), 2 ) + pow( ( p.y - q.y ), 2 ) ));
}

lato_t _verticalOpposite(corner_t c)
{
    switch (c)
    {
        case (corner_t::TOP_LEFT):
        case (corner_t::TOP_RIGHT):
            return lato_t::BOTTOM;
            break;
        case (corner_t::BOTTOM_LEFT):
        case (corner_t::BOTTOM_RIGHT):
            return lato_t::TOP;
            break;
        default:
            assert( 0 );
            break;
    }
}

drone_dir_t _horizontalOpposite(corner_t c)
{
    switch (c)
    {
        case (corner_t::TOP_RIGHT):
        case (corner_t::BOTTOM_RIGHT):
            return drone_dir_t::LEFT;

        case (corner_t::TOP_LEFT):
        case (corner_t::BOTTOM_LEFT):
            return drone_dir_t::RIGHT;

        default:
            assert( 0 );
    }
}

corner_t  _diagonalOpposite(corner_t c)
{
    switch(c)
    {
        case(corner_t::TOP_LEFT):
            return corner_t::BOTTOM_RIGHT;
        case(corner_t::TOP_RIGHT):
            return corner_t::BOTTOM_LEFT;
        case(corner_t::BOTTOM_LEFT):
            return corner_t::TOP_RIGHT;
        case(corner_t::BOTTOM_RIGHT):
            return corner_t::TOP_LEFT;
        default:
            assert( 0 );
    }
}

#endif

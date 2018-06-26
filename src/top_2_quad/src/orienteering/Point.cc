#include "orienteering/Point.hh"
#include <cmath>

using namespace Orienteering;

Point::Point()
    : _x(0.0),
      _y(0.0),
      _score(0.0),
      _flag(false),
      _handler(0)
{
    // ntd
}

Point::Point(float x, float y, float score, int handler)
    : _x(x),
      _y(y),
      _score(score),
      _flag(false),
      _handler(handler)
{
    // ntd
}

Point::~Point()
{
    // ntd
}
int Point::getHandler() const
{
    return _handler;
}

void Point::setHandler(int handler)
{
    _handler = handler;
}

float Point::dist(Point *p1, Point *p2)
{
    return sqrt( pow( ( p1->_x - p2->_x ), 2 )
                 + pow( ( p1->_y - p2->_y ), 2 ) );
}
float Point::getDistStartEnd() const
{
    return _distStartEnd;
}

void Point::setDistStartEnd(float distStartEnd)
{
    _distStartEnd = distStartEnd;
}


bool Point::isFlag() const
{
    return _flag;
}

void Point::setFlag(bool flag)
{
    _flag = flag;
}

int Point::getX() const
{
    return _x;
}

void Point::setX(int x)
{
    _x = x;
}

int Point::getY() const
{
    return _y;
}

void Point::setY(int y)
{
    _y = y;
}

float Point::getScore() const
{
    return _score;
}

void Point::setScore(float score)
{
    _score = score;
}


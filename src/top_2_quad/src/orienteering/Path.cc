#include "orienteering/Path.hh"
#include <list>
#include <cfloat>
#include <iostream>

using namespace Orienteering;

Path::Path()
    :  _start(0),
      _end(0),
      _score(0),
      _length(0),
      size(0)
{
    // ntd
}

Path::Path(Point *start, Point *end)
    : _start(start),
      _end(end),
      _score(0),
      _length(0),
      size(0)
{
    _update();
}
Path::Path(const Path &p)
    : _start(p._start),
      _end(p._end),
      _score(p._score),
      _length(p._length),
      _points(p._points),
      size(p.size)
{

}

Path::~Path()
{
    // ntd
}

Point *Path::getStart() const
{
    return _start;
}

void Path::setStart(Point *start)
{
    _start = start;
}

Point *Path::getEnd() const
{
    return _end;
}

void Path::setEnd(Point *end)
{
    _end = end;
}


float Path::getScore() const
{
    return _score;
}

float Path::getLength() const
{
    return _length;
}

int Path::getSize() const
{
    return _points.size();
}

Point *Path::at(int pos)
{
    auto it = _points.begin();
    std::advance(it, pos);
    return *it;
}

void Path::insert(Path::iterator pos, Point *point)
{
    _points.insert(pos, point);
    _update();
}

void Path::insert(int pos, Point *point)
{
    auto it = _points.begin();
    std::advance(it, pos);
    _points.insert(it, point);
    _update();
}

void Path::addPoint(Point *point)
{
    float min_dist = FLT_MAX;

    float pre = Point::dist(_start, point);
    float post;
    std::list<Point *>::iterator pos;
    std::list<Point *>::iterator it;
    // Try all possible positions, find the best
    it = _points.begin();
    pos =  _points.end();

    for(it = _points.begin(); it != _points.end(); ++it)
    {
        post = Point::dist(point, *it);
        if((pre + post) < min_dist)
        {
            min_dist = pre + post;
            pos = it;
        }
        pre = post;
    }

    // Try last position
    post = Point::dist(point, _end);
    if((pre + post) < min_dist)
    {
        pos = _points.end();
    }

    // Insert point in the right position
    _points.insert(pos, point);

    // Update length and score
    _update();

}

void Path::push_back(Point *point)
{
    _points.push_back(point);
    _update();
}

void Path::pop_back()
{
    _points.pop_back();
    _update();
}

void Path::erase(int pos)
{
    auto it = _points.begin();
    std::advance(it, pos);
    _points.erase(it);
    _update();
}

void Path::remove(Point *point)
{
    _points.remove(point);
    _update();
}

bool Path::contains(Point *point)
{
    for(auto it = _points.begin(); it != _points.end(); ++it)
        if(*it == point)
            return true;
    return false;
}

bool Path::_cleanUp_aux()
{
    float oldLength = _length;
    for(auto it1 = _points.begin(); it1 != std::prev(_points.end(), 1); ++it1)
    {
        for(auto it2 = std::next(it1, 1); it2 != _points.end(); ++it2)
        {
            std::iter_swap(it1, it2);
            _updateLength();
            if(_length < oldLength)
            {
                return true;
            }
            std::iter_swap(it1, it2);
            _length = oldLength;
        }
    }
    _updateLength();
    return false;
}

bool Path::cleanUp()
{
    bool move = false;
    while(_cleanUp_aux())
        move = true;
    return move;
}

void Path::_updateScore()
{
    _score = 0.0;
    for(auto it = _points.begin(); it != _points.end(); ++it)
    {
        Point *p = *it;
        _score += p->getScore();
    }
}

void Path::_updateLength()
{
    _length = 0.0;
    Point *p = _start;
    for(auto it = _points.begin(); it != _points.end(); ++it)
    {
        _length += Point::dist(p, *it);
        p = *it;
    }
    _length += Point::dist(p, _end);
}

void Path::_update()
{
    _updateScore();
    _updateLength();
}

#include "simulatore_drone/Reader.hh"
#include <fstream>
#include <iostream>


Reader::Reader()
{
    // ntd
}

Reader::~Reader()
{
    for(Orienteering::Point *p : _points)
        delete(p);
    if(_start)
        delete (_start);
    if(_end)
        delete(_end);
}

void Reader::read(char *filename)
{
    std::ifstream infile(filename);
    std::string s;
    int n, m;
    float x, y;
    float score;


    infile >> s >> n;

    infile >> s >> m;

    infile >> s >> score;
    _budget = score;

    infile >> x >> y >>  score;

    _start = new Orienteering::Point(x, y, score, 0);
    for(int i = 1; i < n-1; ++i)
    {
        infile >> x >> y >>  score;
        _points.push_back( new Orienteering::Point(x, y, score, i) );

    }
    infile >> x >> y >>  score;
    _end = new Orienteering::Point(x, y, score, n-1);
}

std::list<Orienteering::Point *> Reader::getPoints()
{
    return _points;
}
void Reader::getPoints(std::list<Orienteering::Point *> &points)
{
    for(Orienteering::Point *p : _points)
    {
        points.push_back(p);
    }
}
Orienteering::Point *Reader::getStart()
{
    return _start;
}

Orienteering::Point *Reader::getEnd()
{
    return _end;
}

float Reader::getBudget()
{
    return _budget;
}


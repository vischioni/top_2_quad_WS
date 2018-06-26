#ifndef READER_H
#define READER_H

#include "orienteering/Point.hh"
#include "orienteering/Path.hh"

//using namespace Orienteering;
class Reader
{
public:
    Reader();
    ~Reader();
    void read(char *filename);
    std::list<Orienteering::Point *> getPoints();
    void getPoints(std::list<Orienteering::Point *> &points);
    Orienteering::Point* getStart();
    Orienteering::Point* getEnd();
    float getBudget();

private:
    std::list<Orienteering::Point *> _points;
    Orienteering::Point *_start;
    Orienteering::Point *_end;
    float _budget;
};

#endif

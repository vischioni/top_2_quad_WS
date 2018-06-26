#include "simulatore_drone/Greedy.h"

using namespace Orienteering;
//costruttore
Greedy::Greedy()
{
    _path1 = new Orienteering::Path();
    _path2 = new Orienteering::Path();

}
//distruttore
Greedy :: ~Greedy(){
    //ntd
}

Orienteering::Path* Greedy::getPath1()
{
    return _path1;
}

Path *Greedy::getPath2()
{
    return _path2;
}

float Greedy::getScore()
{
    return _path1->getScore() + _path2->getScore();
}


// ricerca greedy
//void Greedy :: calc(std::list<Point *> points, float budget1, float budget2)
void Greedy :: calc(std::list<Point *> points, float budget1, Point *start, Point *end)
{
    //Point *start = new Point(5, 5, 0, 0);
    //Point *end = new Point(5, 5, 0, 0);
    float budget2 = budget1;
    _path1->setStart(start);
    _path1->setEnd(end);
    _path2->setStart(end);
    _path2->setEnd(start);

    for(auto it = points.begin(); it != points.end(); ++it)
    {
        if((*it)->isFlag())
            continue;
        _path1->addPoint(*it);
        if(_path1->getLength() > budget1)
        {
            _path1->remove(*it);
            _path2->addPoint(*it);
            if(_path2->getLength() > budget2)
                _path2->remove(*it);
             else
                (*it)->setFlag(true);
        }
        else
            (*it)->setFlag(true);
    }
    // XXX
    //_path1->push_back(end);
    //_path2->push_back(start);
}


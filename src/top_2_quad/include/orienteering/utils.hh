#ifndef ORIENTEERTING_UTILS_HH
#define ORIENTEERTING_UTILS_HH

#include "orienteering/Point.hh"
#include "orienteering/Path.hh"

using namespace Orienteering;
bool cmp_points_dist(Point *p1, Point *p2)
{
   return p1->getDistStartEnd() > p2->getDistStartEnd();
}

bool cmp_paths_score(Path *p1, Path *p2)
{
//    if(p1->getScore() == p2->getScore())
//        return p1->getLength() < p2->getLength();
    return p1->getScore() > p2->getScore();
}

Point *_maxDist(std::list<Point *> &points)
{
    Point *p_ret;
    float p_dist = -1;

    for(auto it = points.begin(); it != points.end(); ++it)
    {
        Point *p = *it;
        if(p->isFlag())
            continue;
        if(p->getDistStartEnd() > p_dist)
        {
            p_ret = p;
            p_dist = p->getDistStartEnd();
        }
    }
    return p_ret;
}

#endif

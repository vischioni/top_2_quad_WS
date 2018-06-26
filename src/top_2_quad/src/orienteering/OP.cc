#include "orienteering/OP.hh"
#include "orienteering/utils.hh"

#include <algorithm>
#include <iostream>

#include <cassert>
#include <cfloat>

using namespace Orienteering;
using namespace std;

void OP::print()
{
    for(Path *pa : _pathsTOP)
    {
        cout << "Path: ";
        for(Point *p : pa->_points)
        {
            cout << p->getHandler() << " ";
        }
        cout << "Length: " << pa->getLength() << " " << "Score: " << pa->getScore() << endl;
    }
    cout << endl;
}

float OP::getScore()
{
    float acc = 0.0;
    for (Path *p : _pathsTOP)
        acc += p->getScore();
    return acc;
}

float OP::getScore(int path)
{
    list<Path *>::iterator it = _pathsTOP.begin();
    advance(it, path);
    return (*it)->getScore();
}

float OP::getLength(int path)
{
    list<Path *>::iterator it = _pathsTOP.begin();
    advance(it, path);
    return (*it)->getLength();
}


OP::OP()
{
    // ntd
}

OP::~OP()
{
    for(auto it = _pathsTOP.begin(); it != _pathsTOP.end(); ++it)
        delete *it;

    for(auto it = _pathsNTOP.begin(); it != _pathsNTOP.end(); ++it)
        delete *it;
}

void OP::addPoint(Point *point)
{
    _points.push_back(point);
}

void OP::addPoint(float x, float y, float score, int handler)
{
    Point *p = new Point(x, y, score, handler);
    _points.push_back(p);
}

void OP::setStart(Point *point)
{
    _start = point;
}

void OP::setStart(float x, float y, float score, int handler)
{
    _start = new Point(x, y, score, handler);
}

void OP::setEnd(Point *point)
{
    _end = point;
}

void OP::setEnd(float x, float y, float score, int handler)
{
    _end = new Point(x, y, score, handler);

}

void OP::setBudget(float budget)
{
    _budget = budget;
}

std::list<Path *> OP::getPathsTOP() const
{
    return _pathsTOP;
}

void OP::doThings()
{
/*
Step 1. Initialization
    Perform initialization
    Set record = team score of the initial solution
    Set p = 5
    Set deviation = p % x record
Step 2. Improvement
    For k = 1,2 . . . . . K	( K loop)
        For i = 1,2 . . . . . I	( I loop)
            Perform two-point exchange
            Perform one-point movement
            Perform clean up
            If no movement has been made above, end I loop
            If a new better solution has been obtained, then
                set record = score of new best solution
                set deviation = p % × record
        End I loop
        If no new record is obtained in 5 iterations, then
        go to Step 3
        Perform Reinitialization I (free k points)
    End K loop
Step 3. Reset p = 2.5, perform Reinitialization II (free k points, k is the stopping value in the K loop) and redo Step 2 once more
*/
    int K = _KK;
    int I = _II;
    int _p;
    int k;
    int recK = 1;

    // Step 1
    _initialization();

    _record = _teamScore();
    _p = 5;
    _deviation = _p * 0.01 * _record;

    // Step 2
    for (k = 1; k <= K; ++k)
    {
        for (int i = 0; i < I; ++i)
        {
            _move= false;
            _twoPointExchange();
            _move=false;
            _onePointMovement();
            _cleanUp();
            if(!_move)
                break;
            if (_teamScore() > _record)
            {
                _record =_teamScore();
                _deviation = _p * 0.01 * _record;
                recK = k;
            }
        }
        // If no new record in 5 iterations
        if(k >= recK + 5 )
            break;

        _reinitialization1(k);
    }

    // Step 3
    _p = 2.5;
    if(k > K) k = K;
    _reinitialization2(k);
    recK = 1;


    for (k = 1; k <= K; ++k)
    {
        for (int i = 0; i < I; ++i)
        {
            _move = false;
            _twoPointExchange();
            _move = false;
            _onePointMovement();
            _cleanUp();
            if(!_move)
                break;
            if (_teamScore() > _record)
            {
                _record =_teamScore();
                _deviation = _p * 0.01 * _record;
                recK = k;
            }
        }
        // If no new record in 5 iterations
        if(k >= recK + 5 )
            break;
        if(k == K) // total hack
            continue;
        _reinitialization1(k);
    }
}

void OP::_initialization()
{
    // Find points in ellipse
    for(Point *p : _points)
    {
        p->setDistStartEnd(Point::dist(_start, p) + Point::dist(p, _end));

        if(p->getDistStartEnd() <= _budget)
            _ellipse.push_back(p);
    }

    // sort points in ellipse
    _ellipse.sort(cmp_points_dist);

    // Construct the sorted list of size L
    int L = std::min(5, (int)_ellipse.size());

    _M = 2;


    // Construct M paths
    auto it = _ellipse.begin();
    for(int m = 0; m < _M; ++m, ++it)
    {
        // Constuct path [ _start -> p -> _end ]
        Point *p = *it;
        Path *pathM = new Path(_start, _end);
        pathM->push_back(p);
        p->setFlag(true);
        _pathsNTOP.push_back(pathM);
    }

    // Extend paths
    for(Path *path : _pathsNTOP)
    {
        _extendPath(path);
    }

    // Create path with remainig nodes
    for(Point *p : _ellipse)
    {
        if(p->isFlag())
            continue;

        // Create new path
        Path *newPath = new Path(_start, _end);
        // Add the point
        newPath->push_back(p);
        p->setFlag(true);
        // Extend path
        _extendPath(newPath);
        // Add path to list
        _pathsNTOP.push_back(newPath);
    }

    // Get patshTOP
    _pathsNTOP.sort(cmp_paths_score);
    for(int i = 0; i < _M; ++i)
    {
        _pathsTOP.push_back(_pathsNTOP.front());
        _pathsNTOP.pop_front();
    }
}

void OP::_exchange(Path *path1, Path *path2, Point *point1, Point *point2)
{
    path1->remove(point1);
    path2->remove(point2);

    path1->addPoint(point2);
    path2->addPoint(point1);
}

bool OP::_isPointInPath(Path *path, Point *point)
{
    for(auto it = path->begin(); it != path->end(); ++it)
    {
        Point *p = *it;
        if (p->getHandler() == point->getHandler())
            return true;
    }

    return false;
}

float OP::_teamScore()
{
    float acc = 0.0;

    for(Path *p : _pathsTOP)
        acc +=  p->getScore();
    return acc;
}

float OP::_teamScore_one(std::list<Path *> paths)
{
    float acc = 0.0;

    std::list<Path *> pathsTMP (paths);
    pathsTMP.sort(cmp_paths_score);

    std::list<Path *>::iterator it = pathsTMP.begin();
    for(int i = 0; i < _M; ++i, ++it)
    {
        Path *p = *it;
        acc += p->getScore();
    }

    return acc;
}

void OP::_twoPointExchangeAux(Path *top)
{

    bool exit = false;

    double bestExScore;
    Path *bestExNTOP;
    Point *bestEx_i;
    Point *bestEx_j;
    Path newTop(*top);

    for (Point *j : newTop)
    {
        bestExScore = -1;
        // For each path in the set of pathNop...
        for (Path* pathNTOP :  _pathsNTOP)
        {
            Path newNTOP(*pathNTOP);
            double oldScore = top->getScore();

            // For each node in the current path...
            for (Point *i : newNTOP)
            {

                if (!top->contains(j) || top->contains(i) || !pathNTOP->contains(i) || pathNTOP->contains(j))
                  continue;

                // Do the two-point exchange.
                _exchange(top, pathNTOP, j, i);

                // If the exchange is not feasible, revert the exchange.
                if (top->getLength() > _budget || pathNTOP->getLength() > _budget)
                {
                    _exchange(top, pathNTOP, i, j);
                    continue;
                }

                // Check if an improvement is occurred...
                if (top->getScore() > oldScore)
                {
                    _move = true;
                    exit = true;
                    break;
                }
                // ... otherwise check if this exchange is the best among
                // the worsts.
                else
                {
                    if(top->getScore() > bestExScore)
                    {
                        bestExScore = top->getScore();
                        bestExNTOP = pathNTOP;
                        bestEx_i = i;
                        bestEx_j = j;
                    }
                    _exchange(top, pathNTOP, i, j);
                }
            }
            if (exit)
            {
                exit = false;
                break;
            }

        }

        if(bestExScore > -1 && (bestExScore >= _record - _deviation))
        {
            if (top->contains(bestEx_j) && !top->contains(bestEx_i) && bestExNTOP->contains(bestEx_i) && bestExNTOP->contains(bestEx_j))
            {
                _exchange(top, bestExNTOP, bestEx_j, bestEx_i);
                _move = true;
            }
        }
    }
}



void OP::_twoPointExchange()
{
    for(Path *top : _pathsTOP)
    {
        _twoPointExchangeAux(top);
    }
}


void OP::_onePointMovement()
{
    bool exit = false;
    float oldTeamScore = _teamScore();
    float bestScore = std::numeric_limits<float>::lowest();
    Path *bestPath;


    // Since paths are modified at every iteration
    // we don't need TOPs
    for(int i = 0; i < _M; ++i)
    {
        _pathsNTOP.push_front(_pathsTOP.back());
        _pathsTOP.pop_back();
    }

    // for each point in ellipse

    for(Point *point_i : _ellipse)
    {
        bestScore = std::numeric_limits<float>::lowest();
        std::list<Path *> paths2;
        Path *thePath;
        // for each path
        paths2.clear();
        thePath = nullptr;
        for(Path *path : _pathsNTOP)
        {
            if(_isPointInPath(path, point_i))
            {
                thePath = path;
                thePath->remove(point_i);
                break;
            }
        }
        for(Path *path : _pathsNTOP)
        {
            if(path == thePath) continue;

            // add point
            path->addPoint(point_i);

            // if not feasible, revert
            if(path->getLength() > _budget)
            {
                path->remove(point_i);
                continue;
            }
            float score = _teamScore_one(_pathsNTOP);

            //
            if(score > oldTeamScore)
            {
                oldTeamScore = score;
                exit = true;
                _move = true;
                break;
            }

            if(score > bestScore)
            {
                bestScore = score;
                bestPath = path;
            }

            // revert
            path->remove(point_i);

        }


        if(exit)
        {
            exit = false;
            continue;
        }
        else
        // Do insertion
        if(bestScore > _record - _deviation)
        {
            bestPath->addPoint(point_i);
            oldTeamScore = bestScore;
            _move = true;
        }
        else
        {
            thePath->addPoint(point_i);
        }
    }


    // Delete empty paths
    std::list<Path *> zeroLenPaths;
    for(Path *p : _pathsNTOP)
    {
        if(p->getSize() == 0)
        {
            zeroLenPaths.push_back(p);
        }
    }

    for(Path *p : zeroLenPaths)
    {
        _pathsNTOP.remove(p);
        delete(p);
    }


    // Get patshTOP
    _pathsNTOP.sort(cmp_paths_score);
    for(int i = 0; i < _M; ++i)
    {
        _pathsTOP.push_back(_pathsNTOP.front());
        _pathsNTOP.pop_front();
    }
}

// 2-opt
void OP::_cleanUp()
{
    for(auto it = _pathsNTOP.begin(); it != _pathsNTOP.end(); ++it)
    {
        Path *path = *it;
        if(path->cleanUp())
            _move = true;
    }
}

// Remove k points with smallest score from pathsTOP
// and insert them in patsNTOP
void OP::_reinitialization1(int k)
{
    std::list<std::pair<float, Point *>> table;
    Point *point;
    for(auto it_path = _pathsTOP.begin(); it_path != _pathsTOP.end(); ++it_path)
    {
        Path *path = *it_path;

        // Find the point with smallest score
        for(auto it_point = path->begin(); it_point != path->end(); ++it_point)
        {
            point = *it_point;
            table.push_back(std::pair<float, Point*>(point->getScore(), point));
        }
    }
    table.sort();
    for(int i = 0; i < k && !table.empty(); ++i)
    {
        // Remove point from path
        std::pair<float, Point*> entry = table.front();
        table.pop_front();
        point = entry.second;

        for(Path *path : _pathsTOP)
        {
            if(!_isPointInPath(path, point))
            {
                continue;
            }

            path->remove(point);
            // Insert point in pathsNTOP
            bool no_insert = true;
            for(auto it_ntop = _pathsNTOP.begin(); it_ntop != _pathsNTOP.end(); ++it_ntop)
            {
                Path *ntop = *it_ntop;
                ntop->addPoint(point);
                if(ntop->getLength() > _budget)
                {
                    ntop->remove(point);
                }
                else
                {
                    no_insert = false;
                    //++counter;
                    break;
                }
            }
            if(no_insert)
            {
                path->addPoint(point);
            }
        }
    }

    for(int i = 0; i < _M; ++i)
    {
        _pathsNTOP.push_front(_pathsTOP.back());
        _pathsTOP.pop_back();
    }


    std::list<Path *> zeroLenPaths;
    for(Path *p : _pathsNTOP)
    {
        if(p->getSize() == 0)
        {
            zeroLenPaths.push_back(p);
        }
    }

    for(Path *p : zeroLenPaths)
    {
        _pathsNTOP.remove(p);
        delete(p);
        //++counter2;
    }

    // Get patshTOP
    _pathsNTOP.sort(cmp_paths_score);
    for(int i = 0; i < _M; ++i)
    {
        _pathsTOP.push_back(_pathsNTOP.front());
        _pathsNTOP.pop_front();
    }

}


float insertionCost(Path *path, Path::iterator point_it)
{
    Point *point = *point_it;
    Point *prev;
    Point *next;

    if(point_it == path->begin())
    {
        prev = path->getStart();
    }
    else
    {
        --point_it;
        prev = *point_it;
        ++point_it;
    }
    if(point_it == path->end())
    {
        next = path->getEnd();
    }
    else
    {
        ++point_it;
        next = *point_it;
        --point_it;
    }
    return Point::dist(prev, point) + Point::dist(point, next) - Point::dist(prev, next);
}

// Remove k points with smallest ratio score/cost from pathsTOP
// and insert them in patsNTOP

void OP::_reinitialization2(int k)
{
    Point *point;
    std::list<std::pair<float, Point *>> table;
    for(auto it_path = _pathsTOP.begin(); it_path != _pathsTOP.end(); ++it_path)
    {
        Path *path = *it_path;

        // Find the point with smallest score
        for(auto it_point = path->begin(); it_point != path->end(); ++it_point)
        {
            point = *it_point;
            float cost = insertionCost(path, it_point);
            float ratio = point->getScore() / cost;
            table.push_back(std::pair<float, Point*>(ratio, point));
        }
    }
    table.sort();

    for(int i = 0; i < k && !table.empty(); ++i)
    {
        // Remove point from path
        std::pair<float, Point*> entry = table.front();
        table.pop_front();
        point = entry.second;
        for(Path *path : _pathsTOP)
        {
            if(!_isPointInPath(path, point))
            {
                continue;
            }
            path->remove(point);
            // Insert point in pathsNTOP
            bool no_insert = true;
            for(auto it_ntop = _pathsNTOP.begin(); it_ntop != _pathsNTOP.end(); ++it_ntop)
            {
                Path *ntop = *it_ntop;

                ntop->addPoint(point);
                if(ntop->getLength() > _budget)
                {
                    ntop->remove(point);
                }
                else
                {
                    no_insert = false;
                    break;
                }
            }
            if(no_insert)
            {
                path->addPoint(point);
            }
        }
    }

    for(int i = 0; i < _M; ++i)
    {
        _pathsNTOP.push_front(_pathsTOP.back());
        _pathsTOP.pop_back();
    }
    std::list<Path *> zeroLenPaths;
    for(Path *p : _pathsNTOP)
    {
        if(p->getSize() == 0)
        {
            zeroLenPaths.push_back(p);
        }
    }

    for(Path *p : zeroLenPaths)
    {
        _pathsNTOP.remove(p);
        delete(p);
    }
    // Get patshTOP
    _pathsNTOP.sort(cmp_paths_score);
    for(int i = 0; i < _M; ++i)
    {
        _pathsTOP.push_back(_pathsNTOP.front());
        _pathsNTOP.pop_front();
    }
}


// Greedy extend path
void OP::_extendPath(Path *path)
{
    // For each point in ellipse
    for(auto it = _ellipse.begin(); it != _ellipse.end(); ++it)
    {
        Point *p = *it;
        // if visited, skip
        if(p->isFlag())
            continue;
        // add point
        path->push_back(p);

        // if out of budget
        if(path->getLength() > _budget)
        {
            // remove
            path->pop_back();
        }
        else
        {
            // set point visited
            p->setFlag(true);
        }
    }
}

bool OP::_allVisited()
{
 for(auto it = _ellipse.begin(); it != _ellipse.end(); ++it)
     if(!(*it)->isFlag())
         return true;
 return false;
}

bool OP::_clearEllipse()
{
    for(auto it = _ellipse.begin(); it != _ellipse.end(); ++it)
        (*it)->setFlag(false);
}



#ifndef ORIENTEERING_OP_HH
#define ORIENTEERING_OP_HH

#include <list>
#include <set>
#include "orienteering/Point.hh"
#include "orienteering/Path.hh"

namespace Orienteering {
    class OP
    {
      public:
        OP();
        ~OP();

        void addPoint(Point *point);
        void addPoint(float x, float y, float score, int handler);

        void setStart(Point *point);
        void setStart(float x, float y, float score, int handler);

        void setEnd(Point *point);
        void setEnd(float x, float y, float score, int handler);

        void setBudget(float budget);

        std::list<Path *> getPathsTOP() const;
        void doThings();
        void print();

        float getScore();
        float getScore(int path);
        float getLength(int path);


    //private:
        std::list<Point *> _points;
        std::list<Point *> _ellipse;
        std::list<Path *> _pathsTOP;
        std::list<Path *> _pathsNTOP;

        int _M;
        int _II; int _KK;
        Path *_pathOP;
        Point *_start;
        Point *_end;
        float _budget;
        float _record;
        float _deviation;

        bool _move;

        void _initialization();
        void _twoPointExchange();
        void _twoPointExchangeAux(Path *top);


        void _onePointMovement();
        void _cleanUp();
        void _reinitialization1(int k);
        void _reinitialization2(int k);

        void _reinitialization();

        void _extendPath(Path *path);
        bool _allVisited();
        bool _clearEllipse();
        void _exchange(Path *path1, Path *path2, Point *point1, Point *point2);

        bool _isPointInPath(Path *path, Point *point);
        float _teamScore();
        float _teamScore_one(std::list<Path *> paths);



    };
}
#endif

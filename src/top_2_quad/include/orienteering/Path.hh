#ifndef ORIENTEERING_PATH_HH
#define ORIENTEERING_PATH_HH

#include "orienteering/Point.hh"
#include <list>
namespace Orienteering
{

    class Path
    {
      public:
        Path();
        Path(const Path &p);
        Path(Point *start, Point *end);
        ~Path();

        Point *getStart() const;
        void setStart(Point *start);

        Point *getEnd() const;
        void setEnd(Point *end);

        float getScore() const;
        float getLength() const;
        int getSize() const;


        // -- ITERATOR --
        using iterator = std::list<Point *>::iterator;
        using const_iterator = std::list<Point *>::const_iterator;

        iterator begin() { return _points.begin(); }
        iterator end() { return _points.end(); }
        const_iterator begin() const { return _points.begin(); }
        const_iterator end() const { return _points.end(); }
        const_iterator cbegin() const { return _points.cbegin(); }
        const_iterator cend() const { return _points.cend(); }
        // ---

        Point *at(int pos);
        void insert(iterator pos, Point *point);
        void insert(int pos, Point *point);
        void addPoint(Point *point);
        void push_back(Point *point);
        void pop_back();
        void erase(int pos);
        void remove(Point *point);
        bool contains(Point *point);
        void opt();
        bool cleanUp();
        //Point *closest(Point point);

        std::list<Point *> _points;

    private:
        Point *_start;
        Point *_end;
        float _score;
        float _length;
        int size;

        void _updateScore();
        void _updateLength();
        void _update();
        bool _cleanUp_aux();


    };
}
#endif

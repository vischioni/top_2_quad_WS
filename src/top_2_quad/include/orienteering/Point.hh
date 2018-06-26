#ifndef ORIENTEERING_POINT_HH
#define ORIENTEERING_POINT_HH

namespace Orienteering
{
    class Point
    {
    public:
        Point();
        Point(float x, float y, float score, int handler);
        ~Point();


        int getX() const;
        void setX(int x);

        int getY() const;
        void setY(int y);

        float getScore() const;
        void setScore(float score);

        bool isFlag() const;
        void setFlag(bool flag);

        int getHandler() const;
        void setHandler(int handler);

        float getDistStartEnd() const;
        void setDistStartEnd(float distStartEnd);

        static float dist(Point *p1, Point *p2);

    private:
        int _x;
        int _y;
        float _score;
        bool _flag;
        int _handler;
        float _distStartEnd;
    };
}
#endif

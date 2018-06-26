#include <simulatore_drone/Target.h>
#include <iostream>

using namespace std;

Target::Target(int id)
    : _SHIFT(0.002),
      _SIZE(0.5),
      _dir(direction_t::STOP),
      _read(false),
      _id(id)

{
    _pos.x = 0;
    _pos.y = 0;
    _pos.z = 0;
}

Target::~Target()
{
    // ntd
}

void Target::callBack(const Point::ConstPtr& target)
{
    /*
    _pos.x = target->x;
    _pos.y = target->y;
    _pos.z = target->z;
    */    _read = true;

}

void Target::reset()
{
    _pos.x = 0;
    _pos.y = 0;
    _pos.z = 0;
    _read = false;
    _dir = direction_t::STOP;
}


Point Target::getPosition()
{
    return _pos;
}

bool Target::isRead()
{
    return _read;
}

void Target::move()
{
    switch(_dir)
    {
        case(direction_t::UP):
            _pos.x += _SHIFT;
            break;
        case(direction_t::DOWN):
            _pos.x -= _SHIFT;
            break;
        case(direction_t::RIGHT):
            _pos.y += _SHIFT;
            break;
        case(direction_t::LEFT):
            _pos.y -= _SHIFT;
            break;
        case(direction_t::STOP):
                // ndt
            break;
        default:
            assert(0);
    }
    return;
}

void Target::setPosition(Point pos)
{
    _pos.x = pos.x;
    _pos.y = pos.y;
}

void Target::setPosition(float x, float y)
{
    _pos.x = x;
    _pos.y = y;
}
void Target::undo()
{
    switch(_dir)
    {
        case(direction_t::UP):
            _pos.x -= _SHIFT;
            break;
        case(direction_t::DOWN):
            _pos.x += _SHIFT;
            break;
        case(direction_t::RIGHT):
            _pos.y -= _SHIFT;
            break;
        case(direction_t::LEFT):
            _pos.y += _SHIFT;
            break;
        case(direction_t::STOP):
                // ndt
            break;
        default:
            assert(0);
    }
    return;
}


void Target::setDirection(direction_t dir)
{
    _dir = dir;
}

direction_t Target::getDirection()
{
    return _dir;
}

void Target::randomDirection()
{
    /*
    std::mt19937 dir;
    dir.seed(std::random_device());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,4); // distribution in range [0, 4]


    int dir_scelta = ( int )dist6( dir );
    */
    // FIXME
    int dir_scelta = 4;
    switch( dir_scelta ){
        case(0):
             setDirection(direction_t::UP);
             break;
        case(1):
            setDirection(direction_t::DOWN);
            break;
        case(2):
            setDirection(direction_t::RIGHT);
            break;
        case(3):
            setDirection(direction_t::LEFT);
            break;
        case(4):
            setDirection(direction_t::STOP);
            break;
        default:
            assert(0);
    }

    return;
}


void Target :: printPosition()
{
    ROS_INFO("target[%d] posizione: X[%f], Y[%f]", _id, _pos.x, _pos.y);
    return;
}


/*
bool Target::is_collision_with_TARGET( int t ){ //t = indice del target preso in esame
    float dist_x, dist_y;
    //cerca se la distanza tra un target e tutti gli altri è minore di 0.5, se si allora c'è una collisione
    for ( int i = 0; i < MAX_NUMBER_TARGET; ++i){
        if( i != t ){ //non ha senso controllare la distanza con se stesso
            dist_x = my_abs( target[t].pos.x - target[i].pos.x);
            dist_y = my_abs( target[t].pos.y - target[i].pos.y);
            if( dist_x <= SIZE && dist_y <= SIZE ){
                return true; //rilevata collisione
            }
        }
    }
    return false; //nessuna collisione tra oggetti
}

bool Target :: is_collision_with_WALL( int i, Spigoli &s ){
    //cerca se il target sta collidendo con un muro di confine
    float dist_est, dist_ovest, dist_nord, dist_sud; //le distanze dai 4 muri di confine
    dist_nord = my_abs( s.return_pos(UP_SX).x - target[i].pos.x );
    dist_est = my_abs( s.return_pos(DWN_DX).y - target[i].pos.y );
    dist_sud = my_abs( s.return_pos(DWN_DX).x - target[i].pos.x );
    dist_ovest = my_abs( s.return_pos(UP_SX).y - target[i].pos.y );

    if( dist_nord <= SIZE/2 || dist_est <= SIZE/2 || dist_sud <= SIZE/2 || dist_ovest <= SIZE/2 )
        return true; //rilevata collisione con un confine

    return false; //nessuna collisione con il confine
}
*/


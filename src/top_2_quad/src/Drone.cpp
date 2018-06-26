#include "simulatore_drone/Drone.h"
#include "simulatore_drone/support_types.h"
#include "simulatore_drone/support_methods.h"

//using namespace Drone;


Drone::Drone(float height)
    : _STEP (0.02),
      _readData(false),
      _height(height)
{
/*
    _STEP = 0.02;
    _ALPHA = 0.05;
    _BETA = 0.05;
*/
    _currentPosition.x = 0;
    _currentPosition.y = 0;
    _currentPosition.z = _height;

    _destPosition.x = 0;
    _destPosition.y = 0;
    _destPosition.z = _height;

    _pathOPRead = false;
    _distPercorsa = 0;
    _newDistToEnd = 0;
    _oldDistToEnd = 0;
    _budget = true;
    _totalScore = 0;
    _calcolo_fatto = false;

    //handle dei dummy associati ai target
    targets[ 0 ].id.data = 24; // objHandle univici usati da v-rep
    targets[ 1 ].id.data = 25;
    targets[ 2 ].id.data = 23;
    targets[ 3 ].id.data = 26;
    targets[ 4 ].id.data = 46;
    targets[ 5 ].id.data = 28;
    targets[ 6 ].id.data = 30;
    targets[ 7 ].id.data = 48;
    targets[ 8 ].id.data = 32;
    targets[ 9 ].id.data = 34;
    targets[ 10 ].id.data = 36;
    targets[ 11 ].id.data = 38;
    targets[ 12 ].id.data = 40;
    targets[ 13 ].id.data = 42;
    targets[ 14 ].id.data = 44;

    //Drone :: _crea_nodi_per_orienteering(); //bypasso il fatto che deve essere il drone in v-rep a scoprire le posizioni dei targetget
    Drone ::_printTargetsPos();

    _oldDistToEnd = sqrt( pow( LARGHEZZA_CAMPO , 2 ) + pow( ALTEZZA_CAMPO , 2 ) ); //all'inizia la distanza che ti separa dall punto finale è la diagonale del campo

    _stepPosition.z = _height;

}

Drone::~Drone()
{
    // ndt
}

void Drone::callBackDronePos(const Point::ConstPtr& drone)
{
    _currentPosition.x = drone->x;
    _currentPosition.y = drone->y;
    _currentPosition.z = drone->z;

    _readData = true;

}

void Drone::callBackSensorPos( const simulatore_drone::my_message::ConstPtr& prox_sens)
{
    Int32 handle;
    handle.data = prox_sens->handle.data;
   // ROS_INFO("callback target %d", handle.data);
    for(int i = 0; i < MAX_NUMBER_TARGET; ++i)
    {
        if(targets[i].id.data == handle.data)
        {
                    targets[i].pos.x = prox_sens->posizione.x;
                    targets[i].pos.y = prox_sens->posizione.y;
                    targets[i].visitato = true;
                    targets[i].precisione.data = prox_sens->precisione.data;
                    ROS_INFO( "target[%d]  X[%f],  Y[%f],  Handle[%d],  precisione[%f]",
                              i, targets[i].pos.x, targets[i].pos.y, targets[i].id.data, targets[i].precisione.data);
                   break;
        }
    } // for
}

void Drone::reset()
{
    _currentPosition.x = 0;
    _currentPosition.y = 0;
    _currentPosition.z = _height;

    _destPosition.x = 0;
    _destPosition.y = 0;
    _destPosition.z = _height;

    _readData = false;

}

Point Drone::getPosition()
{
    return _stepPosition;
}

void Drone::move()
{
    //ROS_INFO("move");
    if(!_positionReached2(_destPosition))
    {
        _calcStep();
        _move();
    }
    //else
        //ROS_INFO("arrivato");

}


void Drone::set_raggiunto_punto_path_OP( int target_index )
{
    return;
    /*
    for(int i = 1; i < _pathOPSize; ++i)
    {
        if( punti_path_OP[i].index == target_index )
            punti_path_OP[i].raggiunto = true;
    }
    */
}


void Drone::_printTargetsPos()
{
    /*
    if(greedy)
        ROS_INFO( "\t\tGREEDY" );
    else
        ROS_INFO( "\t\tEURISTICA" );
    for(int i = 0; i < MAX_NUMBER_TARGET; ++i)
        ROS_INFO( "target[ %d ]\tX[ %f ]\tY[ %f ]\tscore:[ %f ]", i, targets[ i ].pos.x, targets[ i ].pos.y, targets[ i ].precisione.data);

    ROS_INFO("\n \n");
    */
}

void Drone::performRecognition(Corners c)
{
}

bool Drone::_positionReached2(Point pos)
{
    return _currentPosition.x == pos.x && _currentPosition.y == pos.y;
}

bool Drone::_positionReached(Point pos)
{
    return my_abs( _currentPosition.x - pos.x) < EPS  &&
             my_abs( _currentPosition.y - pos.y ) < EPS ;
}

void Drone::_setDestination(Point dest )
{
    _destPosition = dest;
}

void Drone::_calcStep()
{
    _distX = _currentPosition.x - _destPosition.x;
    _distY = _currentPosition.y - _destPosition.y;

    float alfa; //angolo in radianti

    //evita divisione per zero
    if( my_abs( _distX ) <= 0.00001 )
        alfa = PI/2; // 90 gradi
    else
        alfa = atan( my_abs(_distY) / my_abs( _distX ) );

    _stepY = _STEP * sin( alfa );
    _stepX = _STEP * cos( alfa );
}

void Drone::_move()
{
    const double X = 1;
    const double Y = 0;
    const double Z = 0;
    const double W = 90;

    if( _positionReached(_destPosition) )
    {
        //sei praticamente arrivato, non fare alcun step e posizionati sopra alla destinazione
        _stepPosition.x = _destPosition.x;
        _stepPosition.y = _destPosition.y;
        _stepPosition.y = _height;
        return;
    }
    //... continua ad avvicanarti
    if( _distX <= 0 && _distY <= 0 )
    {
        _stepPosition.x = _currentPosition.x + _stepX;
        _stepPosition.y = _currentPosition.y + _stepY;
    }
    else if( _distX <= 0 && _distY > 0 )
    {
        _stepPosition.x = _currentPosition.x + _stepX;
        _stepPosition.y = _currentPosition.y - _stepY;
    }
    else if( _distX > 0 && _distY > 0 )
    {
        _stepPosition.x = _currentPosition.x - _stepX;
        _stepPosition.y = _currentPosition.y - _stepY;
    }
    else if( _distX > 0 && _distY <= 0 )
    {
        _stepPosition.x = _currentPosition.x - _stepX;
        _stepPosition.y = _currentPosition.y + _stepY;
    }

    _stepPosition.z = _height;

}

bool Drone::allTargetsVisited()
{
    for ( int i = 0; i < MAX_NUMBER_TARGET; ++i )
        if( !targets[i].visitato )
            return false;

    return true;
}


bool Drone::isRead()
{
 return _readData;
}

void Drone::_resetAllTargets()
{
    for( int i = 0; i < MAX_NUMBER_TARGET; ++i )
            targets[i].visitato = false;
}

void Drone::_resetTarget( int target_i )
{
    _resetAllTargets();
    targets[target_i].visitato = true;
}

corner_t  Drone::_closestCorner(Corners c)
{
    float dist_UP_SX, dist_UP_DX, dist_DWN_SX, dist_DWN_DX;  //distanze tra drone e spigoli

    dist_UP_SX = sqrt( pow( ( _currentPosition.x - c.getPosition(corner_t::TOP_LEFT).x), 2 ) +
                       pow( ( _currentPosition.y - c.getPosition(corner_t::TOP_LEFT).y ), 2 ) );

    dist_UP_DX = sqrt( pow( ( _currentPosition.x - c.getPosition(corner_t::TOP_RIGHT).x ), 2 ) +
                       pow( ( _currentPosition.y - c.getPosition(corner_t::TOP_RIGHT).y ), 2 ) );

    dist_DWN_DX = sqrt( pow( ( _currentPosition.x - c.getPosition(corner_t::BOTTOM_RIGHT).x ), 2 ) +
                        pow( ( _currentPosition.y - c.getPosition(corner_t::BOTTOM_RIGHT).y ), 2 ) );

    dist_DWN_SX = sqrt( pow( ( _currentPosition.x - c.getPosition(corner_t::BOTTOM_LEFT).x ), 2 ) +
                        pow( ( _currentPosition.y - c.getPosition(corner_t::BOTTOM_LEFT).y ), 2 ) );

    float min_dist = 10000;
    corner_t piu_vicino;

    if( dist_UP_SX < min_dist ) { min_dist = dist_UP_SX; piu_vicino = corner_t::TOP_LEFT; }
    if( dist_UP_DX < min_dist ) { min_dist = dist_UP_DX; piu_vicino = corner_t::TOP_RIGHT; }
    if( dist_DWN_SX < min_dist ) { min_dist = dist_DWN_SX; piu_vicino = corner_t::BOTTOM_LEFT; }
    if( dist_DWN_DX < min_dist ) { min_dist = dist_DWN_DX; piu_vicino = corner_t::BOTTOM_RIGHT; }

    return piu_vicino;
}




//*****************************************************************************************************************************************
//****************************************************************************************************************************************

void Drone :: crea_nodi_per_orienteering()
{
    for( int i = 0; i < MAX_NUMBER_TARGET; i++ ){
        switch ( i ) {
        case 0:
            targets[0].pos.x = 8;
            targets[0].pos.y = 6.5;
            targets[0].precisione.data = 2;
            break;
        case 1:
            targets[1].pos.x = 6;
            targets[1].pos.y = 4.5;
            targets[1].precisione.data = 3;
            break;
        case 2:
            targets[2].pos.x = 1;
            targets[2].pos.y = 6.5;
            targets[2].precisione.data = 5;
            break;
        case 3:
            targets[3].pos.x = 1;
            targets[3].pos.y = 3.5;
            targets[3].precisione.data = 7;
            break;
        case 4:
            targets[4].pos.x = 9;
            targets[4].pos.y = 4.5;
            targets[4].precisione.data = 11;
            break;
        case 5:
            targets[5].pos.x = 3.5;
            targets[5].pos.y = 7.5;
            targets[5].precisione.data = 13;
            break;
        case 6:
            targets[6].pos.x = 1;
            targets[6].pos.y = 8.5;
            targets[6].precisione.data = 17;
            break;
        case 7:
            targets[7].pos.x = 1.5;
            targets[7].pos.y = 1;
            targets[7].precisione.data = 19;
            break;
        case 8:
            targets[8].pos.x = 4.5;
            targets[8].pos.y = 1;
            targets[8].precisione.data = 23;
            break;
        case 9:
            targets[9].pos.x = 8;
            targets[9].pos.y = 2.5;
            targets[9].precisione.data = 29;
            break;
        case 10:
            targets[10].pos.x = 8.5;
            targets[10].pos.y = 1;
            targets[10].precisione.data = 31;
            break;
        case 11:
            targets[11].pos.x = 6;
            targets[11].pos.y = 9;
            targets[11].precisione.data = 37;
            break;
        case 12:
            targets[12].pos.x = 8;
            targets[12].pos.y = 8;
            targets[12].precisione.data = 41;
            break;
        case 13:
            targets[13].pos.x = 3.5;
            targets[13].pos.y = 2.5;
            targets[13].precisione.data = 43;
            break;
        case 14:
            targets[14].pos.x = 2.5;
            targets[14].pos.y = 5;
            targets[14].precisione.data = 47;
            break;

        default:
            assert( 1 );
        }
    }
}

void Drone::setDestination(float x, float y)
{
    _destPosition.x = x;
    _destPosition.y = y;
}

bool Drone::destinationReached()
{
//    return _positionReached2(_destPosition);
    return _positionReached(_destPosition);

}

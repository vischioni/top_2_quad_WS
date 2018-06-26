#ifndef COMMON_RESOURCES_H
#define COMMON_RESOURCES_H

#include <cassert>

#include "ros/ros.h"

#include "std_msgs/Bool.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Point.h"
//#include "simulatore_drone/my_message.h"

const int MAX_NUMBER_TARGET = 15; //target presenti nel simulatore
const int LARGHEZZA_CAMPO = 10;
const int ALTEZZA_CAMPO = 10;
const float TOLLERANZA = 0.1;  //distanza minima prima di considerare un obiettivo raggiunto
const float EPS = 0.1;  //per il confronto == tra valori float
//const float PI = 3.14159265358979323846;
const float D_MAX = 20; //massima distanza che può percorrere il drone nel path

typedef geometry_msgs::Point Point;
typedef std_msgs::Bool Bool;
//typedef simulatore_drone::my_message My_message;
typedef std_msgs::Int32 Int32;
typedef std_msgs::Float64 Float64;

typedef enum { SINISTRA, DESTRA } drone_dir_t; //verso cui si sposta durante il zig-zag
typedef enum { UP_SX, UP_DX, DWN_SX, DWN_DX } corner_t; //spigoli del campo
typedef enum { SUPERIORE, INFERIORE, ATTENDI } lato_t; //lati del campo; ATTENDI serve per indicare che il drone non ho da spostarsi verso un lato ma rimanere sul lato mentre fa una traslazione laterale
typedef enum { UP, DWN, DX, SX, fermo } direction_t;  //spostamenti possibili per i target UP = 0; .... ; fermo = 4

//valore assoluto
float my_abs( float num ){
    if( num < 0 )
        return -num;
    return num;
}
#endif

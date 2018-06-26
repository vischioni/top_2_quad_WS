#ifndef SUPPORT_TYPES_H
#define SUPPORT_TYPES_H

#include "ros/ros.h"

#include "std_msgs/Bool.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Point.h"
#include "simulatore_drone/my_message.h"

const int MAX_NUMBER_TARGET = 15;//2;//5;//15; //target presenti nel simulatore
const int LARGHEZZA_CAMPO =  10;//2;//4;//10;
const int ALTEZZA_CAMPO = 10;//2;//4;//10;
//const float TOLLERANZA = 0.1;  //distanza minima prima di considerare un obiettivo raggiunto
const float EPS = 0.01;  //per il confronto == tra valori float
const float PI = 3.14159265358979323846;
const float D_MAX = 20; //massima distanza che può percorrere il drone nel path

typedef geometry_msgs::Point Point;
typedef std_msgs::Bool Bool;
typedef simulatore_drone::my_message My_message;
typedef std_msgs::Int32 Int32;
typedef std_msgs::Float64 Float64;

enum class corner_t { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT }; //spigoli del campo
enum class lato_t { TOP, BOTTOM, STOP }; //lati del campo; ATTENDI serve per indicare che il drone non ho da spostarsi verso un lato ma rimanere sul lato mentre fa una traslazione laterale
enum class direction_t { UP, DOWN, RIGHT, LEFT, STOP };  //spostamenti possibili per i target UP = 0; .... ; fermo = 4
enum class drone_dir_t { LEFT, RIGHT }; //verso cui si sposta durante il zig-zag

#endif

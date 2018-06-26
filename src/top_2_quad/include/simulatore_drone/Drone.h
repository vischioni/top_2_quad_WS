#ifndef DRONE_H
#define DRONE_H

#include "simulatore_drone/support_methods.h"
#include "simulatore_drone/support_types.h"
#include "simulatore_drone/Corners.h"
#include "simulatore_drone/Greedy.h"

#include <cmath>
#include <string>


class Drone
{
public:
    Drone(float height = 1);
    ~Drone();

    void callBackDronePos(const geometry_msgs::Point::ConstPtr& drone); //memorizza la posizioine attuale del drone
    void callBackSensorPos(const simulatore_drone::my_message::ConstPtr& prox_sens); //posizione del sensore quando rileva un target

    void reset();          //reset delle cordinate e dei campi

    Point getPosition();
    void move();  //fa un passetto verso la pos finale

    void performRecognition(Corners corners);
    bool allTargetsVisited();

    bool isReset();
    bool isRead();
    void crea_nodi_per_orienteering(); //setta cordinate dei 15 target in maniera statica per poi passarli alla classe orienteering che calcola il percorso

    void setDestination(float x, float y);
    bool destinationReached();
private:
    const float _STEP;

    //---------     VARIABILI private   --------------
    typedef struct {
      Point pos;            // posizione in cui il sensore ha rilevato il target
      Int32 id;             // numero univoco che identifica il target
      bool visitato = false;// durante il controllo è stato trovato dal sensore
      Float64 precisione; //qualità del rilevamneto
    } target_list;


    target_list targets[ MAX_NUMBER_TARGET ];  //posizioni dei target lette dal sensore di prossimità

    typedef struct{
        Point pos;
        bool raggiunto = false;
        int index; //è quello che mi lega a targets[ index ]
    } punto;
    punto punti_path_OP[ MAX_NUMBER_TARGET + 2 ]; //al più path_OP comprende tutti i target

    Point _currentPosition;    // posizione corrente del drone
    Point _destPosition;       // posizione finale che deve raggiungere
    Point _stepPosition;       // current_pos + step per far avanzare il drone
    My_message _myMsg;


    float _stepX, _stepY;  //entità spostamento durante il movimento del drone, MODIFICATI A RUNTIME
    float _distX, _distY;  //distanza in valore assoluto tra drone e destinazione sugli assi cartesiani x, y

    corner_t _startCorner;  //punto di PARTENZA della modalità zig-zag

    int _closestTarget; //memorizzo l'ultima target visitato, quello su cui si trova il drone
    bool _pathOPRead;  //per sapere se ho già letto tutti i punti di path_OP



    //funzioni private
    //********************************************************************************************************************************
    //********************************************************************************************************************************

    void _calcStep();
    void _move();
    void _resetTarget( int target_i );
    void _resetAllTargets();
    void _printTargetsPos();
    void spiral( int t );
    bool _positionReached( Point pos );
    bool _positionReached2( Point pos );

    void _setDestination( Point dest );
    corner_t _closestCorner(Corners c );

    void _ZigZag(Corners c);
    void set_raggiunto_punto_path_OP( int target_index ); //cerca in punti_path_OP il punto index == target_piu_vicino e lo segna come raggiunto così si può passare al punto successivo
    bool is_all_punti_path_OP_visitati();


    ///******************************************************************************************************
    bool greedy = false; //sceglie sempre il target più vicino
    bool euristica = !greedy; //applica l'euristica implementata in orienteering.cpp per scegliere un percorso
    ///*****************************************************************************************************

    float _distPercorsa;
    float _newDistToEnd;
    float _oldDistToEnd; //la distanza che separa il penultimo target selezionato dalla fine
    bool _budget; //c'è ancora del budget disponibile da utilizzare per cercare altri target
    float _totalScore;
    float _minDist;

    int _pointIndex;
    int _pathOPSize;
    bool _calcolo_fatto;

    bool _destReached;
    bool _readData;

    float _height;
    //bool _controllo_terminato; //quando a finito di visitare i target stoppa la simulazione
};

#endif


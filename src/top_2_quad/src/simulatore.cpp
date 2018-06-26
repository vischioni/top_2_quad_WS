#include "simulatore_drone/Drone.h"
#include "simulatore_drone/Target.h"
#include "simulatore_drone/Corners.h"
#include "simulatore_drone/Greedy.h"
#include "simulatore_drone/support_types.h"
#include "simulatore_drone/support_methods.h"
#include "orienteering/Point.hh"
#include "orienteering/OP.hh"
#include "orienteering/Path.hh"

#include "simulatore_drone/Reader.hh"


#include <list>

std::list<Target *> targets;
std::list<ros::Subscriber> targetsSubscribers;
std::list<ros::Publisher> targetsPublishers;

Drone drone;
Drone drone2(2);
Corners corners;
Greedy greedy;

std::list<Orienteering::Point *> points;

bool _targetsRead()
{
    Target *t;
    for(std::list<Target*>::iterator it = targets.begin(); it != targets.end(); ++it)
    {
        t = *it;
        if(!t->isRead())
            return false;
    }
    return true;
}


void posizionaTargets()
{
    auto it = targets.begin();
    (*it)->setPosition(8, 6.5); // 0
    ++it;
    (*it)->setPosition(6, 4.5); // 1
    ++it;
    (*it)->setPosition(1, 6.5); // 2
    ++it;
    (*it)->setPosition(1, 3.5); // 3
    ++it;
    (*it)->setPosition(9, 4.5); // 4
    ++it;
    (*it)->setPosition(3.5, 7.5); // 5
    ++it;
    (*it)->setPosition(1, 8.5); // 6
    ++it;
    (*it)->setPosition(1.5, 1); // 7
    ++it;
    (*it)->setPosition(4.5, 1); // 8
    ++it;
    (*it)->setPosition(8.5, 2.5); // 9
    ++it;
    (*it)->setPosition(8.5, 1); // 10
    ++it;
    (*it)->setPosition(6, 9); // 11
    ++it;
    (*it)->setPosition(8, 8); // 12
    ++it;
    (*it)->setPosition(3.5, 2.5); // 13
    ++it;
    (*it)->setPosition(2.5, 5); // 14
}

void creaPunti()
{
    points.push_back(new Orienteering::Point(8, 6.5, 2, 24));      // 0
    points.push_back(new Orienteering::Point(6, 4.5, 3, 25));      // 1
    points.push_back(new Orienteering::Point(1, 6.5, 5, 23));      // 2
    points.push_back(new Orienteering::Point(1, 3.5, 7, 26));      // 3
    points.push_back(new Orienteering::Point(9, 4.5, 11, 46));     // 4
    points.push_back(new Orienteering::Point(3.5, 7.5, 13, 28));   // 5
    points.push_back(new Orienteering::Point(1, 8.5, 17, 30));     // 6
    points.push_back(new Orienteering::Point(1.5, 1, 19, 48));     // 7
    points.push_back(new Orienteering::Point(4.5, 1, 23, 32));     // 8
    points.push_back(new Orienteering::Point(8.5, 2.5, 29, 34));   // 9
    points.push_back(new Orienteering::Point(8.5, 1, 31, 36));     // 10
    points.push_back(new Orienteering::Point(6, 9, 37, 38));       // 11
    points.push_back(new Orienteering::Point(8, 8, 41, 40));       // 12
    points.push_back(new Orienteering::Point(3.5, 2.5, 43, 42));   // 13
    points.push_back(new Orienteering::Point(2.5, 5, 47, 44));     // 14
}

int main( int argc, char **argv )
{
    Bool start_sim;
    Bool stop_sim;
    start_sim.data = true;
    stop_sim.data = true;
//    Orienteering::Path::iterator it1, it2;

    ros::init(argc, argv, "simulatore");

    ros::NodeHandle n;

    std::list<Target*>::iterator itT;
    std::list<ros::Publisher>::iterator itP;

    Orienteering::OP *op = new Orienteering::OP();



    if(argc == 4)
    {
        Reader r;
        char *file;
    file = argv[1];
    r.read(file);
    r.getPoints(points);


    op->_II = atoi(argv[2]);
    op->_KK = atoi(argv[3]);

    for(Orienteering::Point *p : points)
        p->setFlag(false);
    for(auto it = points.begin(); it != points.end(); ++it)
    {
        Orienteering::Point *p = *it;
        op->addPoint(*it);
    }


    op->setBudget(r.getBudget());
    op->setStart(r.getStart());
    op->setEnd(r.getEnd());


    op->doThings();
    for(Orienteering::Point *p : points)
        p->setFlag(false);

    greedy.calc(points, r.getBudget(), r.getStart(), r.getEnd());


    // DEBUG
    std::cout << "-----------------------------------------------------------------------------------------\n";
    std::cout << file << "\n";
    std::cout << "Heuristic\n";
    op->print();
    std::cout << "Greedy\n";
    std::cout << "Path: ";

    for(Orienteering::Point *p : greedy.getPath1()->_points)
    {
        std::cout << p->getHandler() << " ";
    }
    std::cout << "Length: " << greedy.getPath1()->getLength() << " " << "Score: " << greedy.getPath1()->getScore() << std::endl;
    std::cout << "Path: ";
    for(Orienteering::Point *p : greedy.getPath2()->_points)
    {
        std::cout << p->getHandler() << " ";
    }
    std::cout << "Length: " << greedy.getPath2()->getLength() << " " << "Score: " << greedy.getPath2()->getScore() << std::endl;
    std::cout << "-----------------------------------------------------------------------------------------\n";
    std::cout << std::endl;



    // EXEC

    std::cout << file;
    std::cout << "\t";
    std::cout << op->_ellipse.size();
    std::cout << "\t";
    std::cout << op->_budget;
    std::cout << "\t";
    std::cout << op->getScore();
    std::cout << "\t";
    std::cout << greedy.getScore();
    std::cout << "\n";
}
    else
    {

        Orienteering::Point *pstart = new Orienteering::Point(5, 5, 0, 0);
        Orienteering::Point *pend = new Orienteering::Point(5, 5, 0, 0);
        creaPunti();
        for(Orienteering::Point *p : points)
            p->setFlag(false);
        for(auto it = points.begin(); it != points.end(); ++it)
        {
            Orienteering::Point *p = *it;
            op->addPoint(*it);
        }
        op->setStart(pstart);
        op->setEnd(pend);
        op->setBudget(12);
        op->_II = 2;
        op->_KK = 2;

        op->doThings();

        op->print();

    // ---------   TARGETS   -------------

    for(int i = 0; i < MAX_NUMBER_TARGET; ++i)
    {
        Target *t = new Target(i);
        targets.push_back(t);
        targetsSubscribers.push_back(n.subscribe("vrep_interface/targetGetPose_" + std::to_string(i), 1, &Target::callBack, t));
        targetsPublishers.push_back(n.advertise<geometry_msgs::Point>("vrep_interface/targetSetPose_" + std::to_string(i), 1 ));
    }

        //--------  DRONE   ----------------
        ros::Subscriber drone_GET_position = n.subscribe("vrep_interface/droneGetPose", 1, &Drone::callBackDronePos, &drone);
        ros::Publisher drone_SET_position = n.advertise<geometry_msgs::Point>("vrep_interface/droneSetPose", 1 );

        //--------- SENSORE DI PROSSIMITÀ  --------
        ros::Subscriber prox_sens = n.subscribe("/vrep_interface/sensoreProssimita", 1, &Drone::callBackSensorPos, &drone);

        //--------  DRONE 2   ----------------
        ros::Subscriber drone_GET_position2 = n.subscribe("vrep_interface/droneGetPose2", 1, &Drone::callBackDronePos, &drone2);
        ros::Publisher drone_SET_position2= n.advertise<geometry_msgs::Point>("vrep_interface/droneSetPose2", 1 );

        //--------- SENSORE DI PROSSIMITÀ 2 --------
        ros::Subscriber prox_sens2 = n.subscribe("/vrep_interface/sensoreProssimita2", 1, &Drone::callBackSensorPos, &drone2);

        //--------    CORNER     ----------------
        ros::Subscriber Spigolo_UP_SX = n.subscribe("vrep_interface/angolo_DWN_SX_Get_Pose", 1, &Corners::callBackBottomLeft, &corners );

        //-----  START/STOP SIMULAZIONE  --------------
        ros::Publisher start = n.advertise<std_msgs::Bool>("startSimulation", 1);
        ros::Publisher stop = n.advertise<std_msgs::Bool>("stopSimulation", 1);
    // END CALLBACK

    ros::Rate loop_rate( 20 ); //a message every 1/20 second (20 Hz)

    bool started_sim = false;

    bool stop1 = false;
    bool stop2 = false;
    posizionaTargets();
ros::Duration(5).sleep();
    drone.setDestination(4.6,5);
    drone2.setDestination(5.6,5);

    std::vector<Orienteering::Path *> paths;
    for(Orienteering::Path *p : op->getPathsTOP() )
    {
        paths.push_back(p);
        paths.back()->push_back(p->getEnd());
    }

    Orienteering::Path::iterator point1_it = paths.at(0)->begin();
    Orienteering::Path::iterator point2_it = paths.at(1)->begin();
//    Orienteering::Point *p1 = *point1_it;
//    Orienteering::Point *p2 = *point2_it;

//    drone.setDestination(p1->getX(), p1->getY());
//    drone2.setDestination(p2->getX(), p2->getY());


    while( ros::ok() )
    {

        if( !started_sim)
        {
            start.publish( start_sim ); //avvia simulazione
            ROS_INFO( "Simulazione iniziata" );
            itT = targets.begin();
            itP = targetsPublishers.begin();
            for(; itT != targets.end(); ++itT, ++itP)
            {
                Target *t = *itT;
                (*itP).publish(t->getPosition());
            }

        }

        // XXX
        if(stop1 && stop2)
        {
            stop.publish( stop_sim );
            ROS_INFO( "Simulazione terminata" );
            return 0;
        }
        if(drone.isRead() && drone2.isRead())
        {
                    started_sim = true; //non serve più mandare il segnale per avviare la simulazione
        }

        if(started_sim)
        {
            if(!drone.destinationReached())
                drone.move();
            else if(!stop1)
            {
                if(point1_it == paths.at(0)->end())
                {
                    stop1 = true;
                    ROS_INFO_STREAM("[DRONE 1]: Missione completata");
                }
                else
                {
                    Orienteering::Point *pp = *point1_it;
                    drone.setDestination(pp->getX(), pp->getY());
                    ROS_INFO_STREAM("[DRONE 1]: Nuova destinazione (" << pp->getX() <<", " << pp->getY() << ")");
                    point1_it++;

                }
            }

            if(!drone2.destinationReached())
                drone2.move();
            else if(!stop2)
            {
                if(point2_it == paths.at(1)->end())
                {
                    stop2 = true;
                    ROS_INFO_STREAM("[DRONE 2]: Missione completata");
                }
                else
                {
                    Orienteering::Point *pp = *point2_it;
                    drone2.setDestination(pp->getX(), pp->getY());
                    ROS_INFO_STREAM("[DRONE 2]: Nuova destinazione (" << pp->getX() <<", " << pp->getY() << ")");
                    point2_it++;
                }
            }


            drone_SET_position.publish( drone.getPosition() );
            drone_SET_position2.publish( drone2.getPosition() );

        }

        loop_rate.sleep(); //wait
        ros::spinOnce();
    } //fine while( ros::ok() )
}
    return 0;
}



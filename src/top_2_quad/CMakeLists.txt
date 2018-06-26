# %Tag(FULLTEXT)%/
cmake_minimum_required(VERSION 2.8.3)
project(top_2_quad)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS
                    roscpp
                    rospy
                    std_msgs
                    geometry_msgs
                    genmsg
                    roslaunch
                    image_transport
                    sensor_msgs
                    message_generation 
)

#find_package(message_generation)
#catkin_package(CATKIN_DEPENDS message_runtime)


find_package(Boost REQUIRED COMPONENTS system)
add_message_files(FILES my_message.msg)
generate_messages(DEPENDENCIES std_msgs geometry_msgs)

## Declare a catkin package
catkin_package(CATKIN_DEPENDS message_runtime)

## Build talker and listener
include_directories(include)
include_directories(${catkin_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})

#set(VREP_DIR "$ENV{vrep_DIR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#add_library(drone src/drone.cpp include/simulatore_drone/drone.h include/simulatore_drone/common_resources.h ) #include/.. solo per vedere file.h in QT
#target_link_libraries(drone ${catkin_LIBRARIES} ${Boost_LIBRARIES})

#add_library(target src/target.cpp include/simulatore_drone/target.h)
#target_link_libraries(target ${catkin_LIBRARIES} ${Boost_LIBRARIES})

#add_library(spigoli src/spigoli.cpp include/simulatore_drone/spigoli.h include/simulatore_drone/common_resources.h ) #include/.. solo per vedere file.h in QT
#target_link_libraries(spigoli ${catkin_LIBRARIES} ${Boost_LIBRARIES})

#add_library(orienteering src/orienteering.cpp include/simulatore_drone/orienteering.h include/simulatore_drone/common_resources.h ) #include/.. solo per vedere file.h in QT
#target_link_libraries(orienteering ${catkin_LIBRARIES} ${Boost_LIBRARIES})


# -- MIE ---

add_library(Drone src/Drone.cpp include/simulatore_drone/Drone.h include/simulatore_drone/support_types.h include/simulatore_drone/support_methods.h) #include/.. solo per vedere file.h in QT
target_link_libraries(Drone ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_library(Corners src/Corners.cpp include/simulatore_drone/Corners.h include/simulatore_drone/support_types.h include/simulatore_drone/support_methods.h) #include/.. solo per vedere file.h in QT
target_link_libraries(Corners ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_library(Target src/Target.cpp include/simulatore_drone/Target.h include/simulatore_drone/support_types.h include/simulatore_drone/support_methods.h)
target_link_libraries(Target ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_library(Greedy src/Greedy.cpp include/simulatore_drone/Greedy.h include/simulatore_drone/support_types.h include/simulatore_drone/support_methods.h)
target_link_libraries(Greedy ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_library(Point src/orienteering/Point.cc include/orienteering/Point.hh)
target_link_libraries(Point ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_library(Path src/orienteering/Path.cc include/orienteering/Path.hh)
target_link_libraries(Path ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_library(OP src/orienteering/OP.cc include/orienteering/OP.hh include/orienteering/utils.hh)
target_link_libraries(OP ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_library(Reader src/Reader.cpp include/simulatore_drone/Reader.hh include/orienteering/utils.hh include/simulatore_drone/support_types.h include/simulatore_drone/support_methods.h)
target_link_libraries(Reader ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_executable(simulatore src/simulatore.cpp)
target_link_libraries(simulatore Drone Target Corners Greedy Point Path OP Reader ${catkin_LIBRARIES} ${Boost_LIBRARIES})


target_link_libraries(Drone ${catkin_LIBRARIES})
target_link_libraries(Corners ${catkin_LIBRARIES})
target_link_libraries(Target ${catkin_LIBRARIES})
target_link_libraries(Greedy ${catkin_LIBRARIES})

# ---

target_link_libraries(Point ${catkin_LIBRARIES})
target_link_libraries(Path ${catkin_LIBRARIES})
target_link_libraries(OP ${catkin_LIBRARIES})
target_link_libraries(Reader ${catkin_LIBRARIES})



target_link_libraries(simulatore ${catkin_LIBRARIES})


#add_executable(simulatore src/simulatore.cpp)
#target_link_libraries(simulatore drone target spigoli orienteering ${catkin_LIBRARIES} ${Boost_LIBRARIES})
#target_link_libraries(simulatore target spigoli ${catkin_LIBRARIES} ${Boost_LIBRARIES})

# %EndTag(FULLTEXT)%

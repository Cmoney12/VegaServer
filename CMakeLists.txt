cmake_minimum_required(VERSION 3.19)
project(VegaServer)

set(CMAKE_CXX_STANDARD 14)
FIND_PACKAGE(Boost REQUIRED COMPONENTS system thread regex)
FIND_PACKAGE(Boost)
find_package (bson-1.0 1.7 REQUIRED)
#find_package(bson-1.7 REQUIRED)
IF(Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)
SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})


add_executable(VegaServer main.cpp chat_message.h)

target_link_libraries(VegaServer ${USED_LIBS} Boost::boost mongo::bson_shared)
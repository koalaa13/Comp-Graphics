cmake_minimum_required(VERSION 3.15)
project(HW2)

set(CMAKE_CXX_STANDARD 14)

add_executable(drawLine main.cpp Utils.h ImageFile.cpp ImageFile.h PNMImage.cpp PNMImage.h Line.h Line.cpp)

add_executable(testing testing.cpp Utils.h ImageFile.cpp ImageFile.h PNMImage.cpp PNMImage.h Line.h Line.cpp)

#if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic -s -O3")
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
#endif()
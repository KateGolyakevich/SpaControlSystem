cmake_minimum_required(VERSION 3.31)
project(SpaControlSystem)

set(CMAKE_CXX_STANDARD 20)

add_executable(SpaControlSystem src/main.cpp
        include/models/Person.h
        include/models/Client.h
        include/models/Employee.h
        include/models/Service.h
        include/models/SpaPackage.h)

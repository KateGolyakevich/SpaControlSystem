cmake_minimum_required(VERSION 3.31)
project(SpaControlSystem)

set(CMAKE_CXX_STANDARD 20)

add_executable(SpaControlSystem src/main.cpp
        include/models/Person.h
        include/models/Client.h
        include/models/Employee.h
        include/models/Service.h
        include/models/SpaPackage.h
        include/models/Room.h
        include/models/InventoryItem.h
        include/models/Appointment.h
        include/core/Exceptions.h
        include/core/Repository.h
        include/core/FileManager.h
        include/core/SpaSystem.h)

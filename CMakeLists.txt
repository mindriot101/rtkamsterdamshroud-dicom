cmake_minimum_required(VERSION 2.6)

set(main srwamsterdamshroud)

project(${main})

find_package(ITK REQUIRED CONFIG)

include(${ITK_USE_FILE})

add_executable(${main} main.cpp)
target_include_directories(${main} PUBLIC
    ${ITK_INCLUDE_DIRS}
    InsightToolkit-5.0.1/build/Modules/Remote/RTK/applications/rtkamsterdamshroud
    InsightToolkit-5.0.1/Modules/IO/DCMTK/include
    )
target_link_libraries(${main} ${ITK_LIBRARIES})

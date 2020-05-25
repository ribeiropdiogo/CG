cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project Name
PROJECT(generator)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
include_directories(includes)
include_directories(../engine/includes)
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
add_executable(${PROJECT_NAME} main.cpp Plane.cpp includes/Plane.h Box.cpp includes/Box.h includes/Cone.h Cone.cpp Figure.cpp includes/Figure.h includes/Sphere.h Sphere.cpp includes/PatchFig.h PatchFig.cpp includes/BezierSuf.h BezierSuf.cpp Torus.cpp includes/Torus.h includes/MobiusStrip.h MobiusStrip.cpp)

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

if  (WIN32)

    message(STATUS "GLUT_DIR set to: " ${GLUT_DIR})
    set(GLUT_DIR "" CACHE PATH "Path to GLUT folder")
    if (NOT EXISTS "${GLUT_DIR}/GL/glut.h" OR NOT EXISTS "${GLUT_DIR}/glut32.lib")
        message(ERROR ": GLUT not found")
    endif (NOT EXISTS "${GLUT_DIR}/GL/glut.h" OR NOT EXISTS "${GLUT_DIR}/glut32.lib")
    include_directories(${GLUT_DIR})
    link_directories(${GLUT_DIR})
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_DIR}/glut32.lib )
    if (EXISTS "${GLUT_DIR}/glut32.dll")
        file(COPY ${GLUT_DIR}/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
    endif(EXISTS "${GLUT_DIR}/glut32.dll")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

else (WIN32) #Linux and Mac

    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} Wno-deprecated")
    find_package(GLUT REQUIRED)
    include_directories(${GLUT_INCLUDE_DIR})
    link_directories(${GLUT_LIBRARY_DIRS})
    add_definitions(${GLUT_DEFINITIONS})
    find_package(DevIL REQUIRED)
    link_libraries(${IL_LIBRARIES} )
    include_directories(${IL_INCLUDE_DIR})

    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${IL_LIBRARIES})
    if(NOT GLUT_FOUND)
        message(ERROR ": GLUT not found!")
    endif(NOT GLUT_FOUND)

endif(WIN32)
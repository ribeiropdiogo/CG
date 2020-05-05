cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project Name
PROJECT(engine)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
include_directories(includes)

add_library(${PROJECT_NAME} includes/DrawEvent.h DrawEvent.cpp includes/TransformEvent.h TransformEvent.cpp includes/Group.h Group.cpp includes/Engine.h includes/EngineMotion.h EngineMotion.cpp Engine.cpp includes/Vec3.h Vec3.cpp includes/CRSpline.h CRSpline.cpp)

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

if  (WIN32)

    message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
    set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")
    if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
        message(ERROR ": GLUT not found")
    endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")

    if (NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")
        message(ERROR ": GLEW not found")
    endif (NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")

    include_directories(${TOOLKITS_FOLDER}/glut ${TOOLKITS_FOLDER}/glew)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES}
            ${TOOLKITS_FOLDER}/glut/glut32.lib
            ${TOOLKITS_FOLDER}/glew/glew32.lib )

    if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll")
        file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll
                DESTINATION ${CMAKE_BINARY_DIR})
        file(COPY ${TOOLKITS_FOLDER}/glew/glew32.dll
                DESTINATION ${CMAKE_BINARY_DIR})
    endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

else (WIN32) #Linux and Mac

    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} Wno-deprecated")
    find_package(GLUT REQUIRED)
    include_directories(${GLUT_INCLUDE_DIR})
    link_directories(${GLUT_LIBRARY_DIRS})
    add_definitions(${GLUT_DEFINITIONS})

    if (NOT APPLE)
        find_package(GLEW REQUIRED)
        include_directories(${GLEW_INCLUDE_DIRS})
        link_libraries(${GLEW_LIBRARIES})
    endif(NOT APPLE)

    find_package(DevIL REQUIRED)
    link_libraries(${IL_LIBRARIES} )
    include_directories(${IL_INCLUDE_DIR})

    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${IL_LIBRARIES})
    if(NOT GLUT_FOUND)
        message(ERROR ": GLUT not found!")
    endif(NOT GLUT_FOUND)



endif(WIN32)

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
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
    if(NOT GLUT_FOUND)
        message(ERROR ": GLUT not found!")
    endif(NOT GLUT_FOUND)

endif(WIN32)
cmake_minimum_required(VERSION 3.20.0)

project(Navras)
set (target_name ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set include path
include_directories(${CMAKE_SOURCE_DIR}/ext/)
include_directories(${PROJECT_SOURCE_DIR}/ext/Assimp/include/)
include_directories(${CMAKE_SOURCE_DIR}/ext/ffmpeg/include/)
include_directories(${CMAKE_SOURCE_DIR}/ext/glm/)
include_directories(${CMAKE_SOURCE_DIR}/ext/SOIL/)

# set library path
link_directories(${CMAKE_SOURCE_DIR}/ext/Assimp/lib/)

set(PROJ_LIBS
    X11
    GL
    m
    GLEW
    SOIL
    assimp
)


file(GLOB PROJ_SOURCES
     "src/helper/*.cpp"
     "src/shader/*.cpp"
     "src/effects/*.cpp"
     "src/scenes/*.cpp"
)



#file(GLOB_RECURSE SRC_FILES comp1/src/*.c comp2/src/*.c comp3/src/*.c)
# set(HELPER_SRC_FILES src/helper/*.cpp)

# set(OBJS    
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/helper/*.o
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/shader/*.o
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/*.o
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/scenes/*.o
#     )

# add_library( navras_obj  
#         ${HELPER_SRC_FILES} ${SHADER_SRC_FILES} 
#         ${EFFECTS_SRC_FILES} ${SCENES_SRC_FILES} 
#         )

add_library( srcobjects SHARED ${PROJ_SOURCES})
add_executable( ${target_name} src/main.cpp )
target_link_libraries( ${target_name} ${PROJ_LIBS} srcobjects)

#target_link_libraries(model PRIVATE mw)




cmake_minimum_required(VERSION 3.20.0)

project(Navras)
set (target_name ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set include path
# include_directories(${PROJECT_SOURCE_DIR}/ext/Assimp/include/)
include_directories(${CMAKE_SOURCE_DIR}/ext/)
include_directories(${CMAKE_SOURCE_DIR}/ext/Assimp/include/)
include_directories(${CMAKE_SOURCE_DIR}/ext/ffmpeg/include/)
include_directories(${CMAKE_SOURCE_DIR}/ext/glm/)
include_directories(${CMAKE_SOURCE_DIR}/ext/SOIL/)

# set library path
link_directories(${CMAKE_SOURCE_DIR}/ext/Assimp/bin/)
link_directories(${CMAKE_SOURCE_DIR}/ext/AL/lib/)
link_directories(${CMAKE_SOURCE_DIR}/ext/ffmpeg/bin/)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


set(PROJ_LIBS
    GL
    openal
    m
    GLEW
    SOIL
    assimp
    X11
    ffmpeg
    swscale
)


file(GLOB PROJ_SOURCES
    "src/debug/*.cpp"
    "src/helper/*.cpp"
    "src/shaders/*.cpp"
    "src/effects/*.cpp"
    "src/scenes/*.cpp"
    "src/main.cpp"
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

# add_library( srcobjects SHARED ${PROJ_SOURCES})
# add_executable( ${target_name} src/main.cpp )
add_executable( ${target_name} ${PROJ_SOURCES} src/main/main_xwindows.cpp)
target_link_libraries( ${target_name} ${PROJ_LIBS} )

#target_link_libraries(model PRIVATE mw)




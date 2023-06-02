clear


main_source_file="src/main/main_xwindows.cpp"

gcc -o main -I ext/ -I ext/Assimp/include/ -I ext/ffmpeg/include src/effects/*.cpp 
# src/helper/*.cpp src/shaders/*.cpp ${main_source_file} \ 
# src/scenes/*.cpp src/*.cpp   \
# src/*.cpp 
# -lX11 -lGL -lm -lGLEW -lSOIL -lassimp && ./main


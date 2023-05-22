clear


main_source_file="src/main/main_xwindows.cpp"

gcc -o main ${main_source_file} src/*.cpp -lX11 -lGL -lm -lGLEW -lSOIL -lassimp && ./main


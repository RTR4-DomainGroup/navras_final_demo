cls

del Navras.exe

mkdir .\obj\

cl.exe /Fo.\obj\ /c /EHsc /MP /I C:\glew\include /I ext\ /I ext\Assimp\include\ /I ext\ffmpeg\include src\*.cpp src\effects\*.cpp src\helper\*.cpp src\shaders\*.cpp src\scenes\*.cpp src\main\main_windows.cpp

rc.exe res\resources.rc

link.exe  /incremental /debug:fastlink /OUT:"Navras.exe" .\obj\*.obj /Libpath:C:\glew\lib\Release\x64 /Libpath:ext\ /Libpath:ext\Assimp\lib\ res\resources.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS ^
/NODEFAULTLIB:library MSVCRTD.lib /NODEFAULTLIB:library LIBCMT.lib 

@REM del .\obj\*.obj

del res\resources.res

cmd /C execute.bat


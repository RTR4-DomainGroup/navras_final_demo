cls

del Navras.exe

cl.exe /c /EHsc /I C:\glew\include /I ext\ /I ext\Assimp\include\ src\*.cpp src\effects\*.cpp src\helper\*.cpp src\shaders\*.cpp src\scenes\*.cpp

rc.exe res\resources.rc

link.exe /OUT:"Navras.exe" *.obj /Libpath:C:\glew\lib\Release\x64 /Libpath:ext\ res\resources.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS ^
/NODEFAULTLIB:library MSVCRTD.lib /NODEFAULTLIB:library LIBCMT.lib 

del *.obj

del res\resources.res

set PATH=%PATH%;.\ext\AL\bin\

Navras.exe

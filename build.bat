cls

del Navras.exe

cl.exe /c /EHsc /I C:\glew\include /I lib\external\ src/*.cpp src/effects/*.cpp src/helper/*.cpp src/shaders/*.cpp src/scenes/*.cpp

rc.exe res/resources.rc

link.exe /OUT:"Navras.exe" *.obj /Libpath:C:\glew\lib\Release\x64 /Libpath:lib\external res/resources.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS ^
/NODEFAULTLIB:library MSVCRTD.lib /NODEFAULTLIB:library LIBCMT.lib 

del *.obj

del res\resources.res

Navras.exe

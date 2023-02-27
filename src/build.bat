cls

del *.obj

del Navras.exe

cl.exe /c /EHsc /I C:\glew\include *.cpp helper/*.cpp shaders/*.cpp

rc.exe ../res/OGL.rc

link.exe /OUT:"Navras.exe" *.obj /Libpath:C:\glew\lib\Release\x64 ../res/OGL.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

Navras.exe

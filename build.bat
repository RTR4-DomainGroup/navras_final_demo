cls

del Navras.exe

cl.exe /c /EHsc /I C:\glew\include /I lib\ src\*.cpp src\effect\*.cpp src\helper\*.cpp src\shaders\*.cpp src\scenes\*.cpp

rc.exe res\resources.rc

link.exe /OUT:"Navras.exe" *.obj /Libpath:C:\glew\lib\Release\x64 /Libpath:lib\ res\resources.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS ^
/NODEFAULTLIB:library MSVCRTD.lib /NODEFAULTLIB:library LIBCMT.lib 

del *.obj

del res\resources.res

set PATH=%PATH%;.\lib\

Navras.exe

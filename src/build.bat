cls

cl.exe /c /EHsc /I C:\glew\include *.cpp

link.exe *.obj /Libpath:C:\glew\lib\Release\x64 user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

main.exe

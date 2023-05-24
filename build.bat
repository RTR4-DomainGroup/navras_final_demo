cls

git update-index --skip-worktree inc/helper/constants.h
@REM git update-index --no-skip-worktree inc/helper/constants.h

del Navras.exe
@REM del .\obj\*.obj
@REM del res\resources.res

mkdir .\obj\

cl.exe /GL /MP /incremental /Fo.\obj\ /c /EHsc /I C:\glew\include /I ext\ /I ext\Assimp\include\ /I ext\ffmpeg\include src\*.cpp src\effects\*.cpp src\helper\*.cpp src\shaders\*.cpp src\scenes\*.cpp src\debug\*.cpp src\main\main_windows.cpp && ^
rc.exe res\resources.rc && ^
link.exe /LTCG /debug:fastlink /OUT:"Navras.exe" .\obj\*.obj /Libpath:C:\glew\lib\Release\x64 ^
/Libpath:ext\ /Libpath:ext\Assimp\lib\ res\resources.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS ^
/NODEFAULTLIB:library MSVCRTD.lib /NODEFAULTLIB:library LIBCMT.lib && ^
cmd /C execute.bat


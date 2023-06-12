cls

@REM option 1
@REM git update-index --skip-worktree inc/helper/constants.h
@REM git update-index --no-skip-worktree inc/helper/constants.h

@REM option 2
@REM git update-index --assume-unchanged inc/helper/constants.h
@REM git update-index --no-assume-unchanged inc/helper/constants.h

del Navras.exe
del log.txt
@REM del .\obj\*.obj
@REM del res\resources.res

mkdir .\obj\

cl.exe /fp:fast /GL /MP /incremental /Fo.\obj\ /c /EHsc /I C:\glew\include /I ext\ /I ext\Assimp\include\ /I ext\ffmpeg\include /I ext\freetype-2.13.0\include src\*.cpp src\effects\*.cpp src\helper\*.cpp src\shaders\*.cpp src\scenes\*.cpp src\debug\*.cpp src\main\main_windows.cpp && ^
rc.exe res\resources.rc && ^
link.exe /LTCG /debug:fastlink /OUT:"Navras.exe" .\obj\*.obj /Libpath:C:\glew\lib\Release\x64 ^
/Libpath:ext\ /Libpath:ext\Assimp\lib\ /LIBPATH:ext\freetype-2.13.0\objs\x64\Release res\resources.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS ^
/NODEFAULTLIB:library MSVCRTD.lib /NODEFAULTLIB:library msvcrt.lib /NODEFAULTLIB:library LIBCMT.lib && ^
cmd /C execute.bat


@echo off

IF NOT EXIST build mkdir build
pushd build

REM raylib debug build
set CompilerFlags=-nologo -MT -FC -Z7 -EHsc -W2 -D_CRT_SECURE_NO_WARNINGS /Fe"find_ray" /I"..\lib\raylib-5.0_win64_msvc16\include"
set LinkerFlags=raylib.lib user32.lib gdi32.lib shell32.lib winmm.lib msvcrt.lib /NODEFAULTLIB:libcmt /LIBPATH:"..\lib\raylib-5.0_win64_msvc16\lib" /LIBPATH:"..\lib\glfw-3.3.5.bin.WIN64\lib-vc2022"
cl %CompilerFlags% ..\src\main.c /link %LinkerFlags%

REM imgui debug build
set CompilerFlags=-nologo -MT -FC -Z7 -EHsc -W2 -D_CRT_SECURE_NO_WARNINGS /Fe"find_im" /I"..\lib\glfw-3.3.5.bin.WIN64\include" /I"..\lib\imgui-master" /I"..\lib\glew-2.1.0\include"
set LinkerFlags=user32.lib gdi32.lib shell32.lib msvcrt.lib glfw3_mt.lib glew32s.lib opengl32.lib  /NODEFAULTLIB:libcmt /LIBPATH:"..\lib\glfw-3.3.5.bin.WIN64\lib-vc2022" /LIBPATH:"..\lib\glew-2.1.0\lib\Release\x64"
set ImguiDirectory=..\src\imgui\
cl %CompilerFlags% ..\src\main.cpp %ImguiDirectory%imgui.cpp %ImguiDirectory%imgui_demo.cpp %ImguiDirectory%imgui_draw.cpp %ImguiDirectory%imgui_tables.cpp %ImguiDirectory%imgui_widgets.cpp %ImguiDirectory%imgui_impl_glfw.cpp %ImguiDirectory%imgui_impl_opengl3.cpp /link %LinkerFlags%

popd

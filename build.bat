@echo off
SET compiler_dir="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.40.33807\bin\Hostx64\x64\cl.exe"

SET header_file_glfw3="D:\libraries\glfw-3.3.8\include"
SET header_file_glad="D:\libraries\glad\include"
SET lib_file_glfw3="D:\libraries\glfw-3.3.8\build\x64\lib\glfw3.lib"
SET glad_main_file="D:\libraries\glad\src\glad.c"
SET glm_header_files="D:\libraries\glm"

REM Go to build folder - if not exist, create it

IF NOT EXIST build mkdir build

pushd build

%compiler_dir% /EHsc /Zi /MDd^
  /DEBUG:FULL^
  /INCREMENT:NO^
  /std:c++17^
  /Fe:"iso-trade"^
  ../src/iso_trade.cpp^
  %glad_main_file%^
  /I %header_file_glfw3% /I%header_file_glad% /I%glm_header_files%^
  OpenGL32.lib User32.lib gdi32.lib Shell32.lib %lib_file_glfw3%

popd


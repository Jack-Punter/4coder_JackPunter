@echo off

set codeDir=%cd%

IF NOT EXIST %codeDir%\build mkdir %codeDir%\build

pushd %codeDir%\build\

call %codeDir%\..\bin\buildsuper_x64-win.bat %codeDir%\JackPunter.cpp

echo End of compile
copy custom_4coder.dll %codeDir%\..\..\custom_4coder.dll
copy custom_4coder.pdb %codeDir%\..\..\custom_4coder.pdb

popd
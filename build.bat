@echo off

set codeDir=%cd%

mkdir %codeDir%\build

pushd %codeDir%\build\

call %codeDir%\..\bin\buildsuper_x64-win.bat %codeDir%\JackPunter.cpp

echo End of compile
copy custom_4coder.dll %codeDir%\..\..\custom_4coder.dll
copy custom_4coder.pdb %codeDir%\..\..\custom_4coder.pdb

popd
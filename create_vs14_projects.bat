@echo off

rem Generate solutions for Visual Studio 14 (2015)
Utils\premake5 vs2015

timeout /t 5 /nobreak

@echo off

rem If you are having compilation errors which does not generate an ouput file:
rem Go to MinGW dir and rename mingw32 to mingw32old

g++ -Wall test.cpp Socket/Socket.cpp ServerSocket/ServerSocket.cpp -std=c++11 -lws2_32 -o WindowsTest

pause

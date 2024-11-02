@echo OFF
cls

if not exist "build" ( mkdir "build" )
if not exist "final" ( mkdir "final" )

cmake -S . -B build
cmake --build build --config=Release
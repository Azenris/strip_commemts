@echo OFF
cls
SET mypath=%~dp0
pushd %mypath%\final\debug\
start strip_comments.exe
popd
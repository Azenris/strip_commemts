@echo OFF
cls
tools\timer.exe start build/build_time_data.bin

:: ----------------------------------------------------
:: CMAKE BUILD
:: ----------------------------------------------------
if "%1" == "cmake" (
	if not exist "build" ( mkdir "build" )
	if not exist "final" ( mkdir "final" )
	cmake -S . -B build
	cmake --build build --config=Debug
	goto build_end
)

:: ----------------------------------------------------
:: RAW BUILD
:: ----------------------------------------------------

:: Run vcvars64 to setup enviroment
if not defined VSCMD_ARG_TGT_ARCH (
	call "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/vcvars64.bat"
)

:: MUST be after the enviroment setup
setlocal EnableDelayedExpansion

:: ----------------------------------------------------
:: DEPS
:: ----------------------------------------------------

set deps=C:/vcpkg/packages

:: Optional - Can be ignored if staticBuild is set to 0
:: ZLIB RELEASE
set zlib_Lib_Release="%deps%/zlib_x64-windows-static/lib/zlib.lib"
set zlib_Include_Release="%deps%/zlib_x64-windows-static/include"
:: ZLIB DEBUG
set zlib_Lib_Debug="%deps%/zlib_x64-windows-static/debug/lib/zlibd.lib"
set zlib_Include_Debug="%deps%/zlib_x64-windows-static/include"

:: ----------------------------------------------------
:: OPTIONS
:: ----------------------------------------------------
SET platform=PLATFORM_WINDOWS
set debugMode=1
set asanEnabled=0
set includeZlib=0
set name=strip_comments
set outputDir=final
set buildDir=build
set linker=
set flags=-std:c++20 -Zc:preprocessor -Zc:strictStrings -GR- -EHsc
set warnings=-WX -W4 -wd4189 -wd4201 -wd4324 -wd4505
set includes=-Ithird_party/ -Isrc/ -Iassets/
set defines=-D_CRT_SECURE_NO_WARNINGS
set links=-link

:: ----------------------------------------------------
:: 
:: ----------------------------------------------------
if %includeZlib% == 1 (
	set defines=%defines% -DINCLUDE_ZLIB
	if %debugMode% == 1 (
		set zlibLib=%zlib_Lib_Debug%
		set includes=%includes% -I%zlib_Include_Debug%
	) else (
		set zlibLib=%zlib_Lib_Release%
		set includes=%includes% -I%zlib_Include_Release%
	)
)

if %debugMode% == 1 (
	set linker=%linker% -SUBSYSTEM:CONSOLE
	set outputDir=%outputDir%/debug
	set defines=%defines% -DDEBUG -D_DEBUG -D_MT -DBUILD_TYPE=\"DEBUG\"
	set flags=%flags% -Z7 -FC -MTd
	if %asanEnabled% == 1 (
		set flags=!flags! -fsanitize=address -Zi -Fd!outputDir!/asan.pdb
	)
) else (
	set linker=%linker% -SUBSYSTEM:WINDOWS
	set outputDir=%outputDir%/release
	set defines=%defines% -DNDEBUG -DBUILD_TYPE=\"RELEASE\"
	set flags=%flags% -MT -O2 -Ot -GF
)

if %platform% == PLATFORM_WINDOWS (
	set defines=%defines% -D__PLATFORM_WINDOWS__
)

if not exist %outputDir% ( mkdir "%outputDir%" )
if not exist %buildDir% ( mkdir "%buildDir%" )

:: ----------------------------------------------------
:: 
:: ----------------------------------------------------
echo --------------------
echo Platform: %platform%
echo Output::  %outputDir%
echo --------------------

cl -nologo %flags% %warnings% %defines% -Fe%outputDir%/%name%.exe -Fo%buildDir%/ src/main.cpp %includes% %links% -INCREMENTAL:NO %linker%
if not !ERRORLEVEL! == 0 ( goto build_failed )

:build_success
echo Build success!
goto build_end

:build_failed
echo Build failed.
goto build_end

:build_end
tools\timer.exe end build/build_time_data.bin

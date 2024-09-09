@echo off
set dllPath=%~1
set solutionDir=%~2
set projectDir=%~3
set arch=%~4
set config=%~5

echo Running pre-build for %config%

where /q GmlCppExtFuncs
if %ERRORLEVEL% EQU 0 (
	echo Running GmlCppExtFuncs...
	GmlCppExtFuncs ^
	--prefix winwin^
	--cpp "%projectDir%autogen.cpp"^
	--gml "%solutionDir%winwin_23/extensions/winwin/autogen.gml"^
	--include "winwin.h"^
	--include "winwin_config.h"^
	--index "%projectDir%winwin.h"^
	--index "%projectDir%winwin_auto.h"^
	--index "%projectDir%winwin_config.h"^
	--struct 1^
	%projectDir%winwin.cpp^
	%projectDir%winwin_*.cpp
)
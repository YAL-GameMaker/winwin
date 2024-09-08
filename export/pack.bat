@echo off

goto gms23only

if not exist "winwin-for-GMS1" mkdir "winwin-for-GMS1"
cmd /C copyre ..\winwin.gmx\extensions\winwin.extension.gmx winwin-for-GMS1\winwin.extension.gmx
cmd /C copyre ..\winwin.gmx\extensions\winwin winwin-for-GMS1\winwin
cmd /C copyre ..\winwin.gmx\datafiles\winwin.html winwin-for-GMS1\winwin\Assets\datafiles\winwin.html
cd winwin-for-GMS1
cmd /C 7z a winwin-for-GMS1.7z *
move /Y winwin-for-GMS1.7z ../winwin-for-GMS1.gmez
cd ..

if not exist "winwin-for-GMS2\extensions" mkdir "winwin-for-GMS2\extensions"
if not exist "winwin-for-GMS2\datafiles" mkdir "winwin-for-GMS2\datafiles"
if not exist "winwin-for-GMS2\datafiles_yy" mkdir "winwin-for-GMS2\datafiles_yy"
cmd /C copyre ..\winwin_yy\extensions\winwin winwin-for-GMS2\extensions\winwin
cmd /C copyre ..\winwin_yy\datafiles\winwin.html winwin-for-GMS2\datafiles\winwin.html
cmd /C copyre ..\winwin_yy\datafiles_yy\winwin.html.yy winwin-for-GMS2\datafiles_yy\winwin.html.yy
cd winwin-for-GMS2
cmd /C 7z a winwin-for-GMS2.zip *
move /Y winwin-for-GMS2.zip ../winwin-for-GMS2.yymp
cd ..

:gms23only
if not exist "winwin-for-GMS2.3+\extensions" mkdir "winwin-for-GMS2.3+\extensions"
if not exist "winwin-for-GMS2.3+\datafiles" mkdir "winwin-for-GMS2.3+\datafiles"
cmd /C copyre ..\winwin_23\extensions\winwin winwin-for-GMS2.3+\extensions\winwin
cmd /C copyre ..\winwin_23\datafiles\winwin.html winwin-for-GMS2.3+\datafiles\winwin.html
cd winwin-for-GMS2.3+
cmd /C 7z a winwin.zip *
move /Y winwin.zip ../winwin.yymps
cd ..

del /Q winwin-demo.yyz
cd ..\winwin_23
cmd /C 7z winwin-demo.zip *
move winwin-demo.zip ..\export\winwin-demo.yyz
move ..\export

pause
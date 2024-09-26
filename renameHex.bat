@echo off
rem This program receives as input two arguments:
rem Arg1 is the name of the image (including directory)
rem Arg2 is the extension of the image file
rem In other words, this program is expected to be called from a
rem post bild step of "copyWithDate ${ImageDir} ${ImageName}"
rem This program produces one output:
rem It renames the ImageName to be the value of newName, which is set below

set newName=ODCFW40442

copy %1 %newName%.%2
goto end
:bad
:end


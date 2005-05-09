@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

rem BEWARE, this may cause a "line too long" error if called repeatedly from the same dos box
rem Try to check, whether these vars are already set
if "%VSINSTALLDIR%"=="" call "%VS71COMNTOOLS%\vsvars32.bat"
if "%TortoiseVars%"=="" call ..\TortoiseVars.bat

set APPS=TortoiseSVN TortoiseMerge
set TARGETS=pdf chm html

rmdir /s /q output > NUL
mkdir output > NUL

rem Determine available languages for documentation by looking for the .po files

if "%1"=="" (
  FOR %%L IN (po\*.po) DO (
    CALL :doit %%~nL %~dp0
  )
  CALL :doit en %~dp0
) else (
  CALL :doit %1 %~dp0
)

Goto :EOF
rem | End of Batch execution -> Exit script
rem +----------------------------------------------------------------------

:doit

echo.
echo Generating Documentation for: %APPS%
echo Language: %1 %2
echo Targets: %TARGETS%
echo ----------------------------------------------------------------------
echo.

rem do *NOT* create english translation from english :-)

if NOT %1 EQU en (
  echo Translating: %1
  SET POFILE=po\%1.po
  SET SRCDIR=source\en
  SET TARGDIR=%2source\%1

  rmdir /s /q !TARGDIR! 
  mkdir !TARGDIR!

  rem --------------------
  rem Collect files to translate
  rem No real recursion, only one level deep

  cd !SRCDIR!

  SET CHAPTERS=

  FOR %%F IN (*.xml) DO SET CHAPTERS=!CHAPTERS! %%F

  FOR /D %%D IN (*) DO (
    mkdir !TARGDIR!\%%D

    FOR %%F IN (%%D\*.xml) DO (
      SET CHAPTERS=!CHAPTERS! %%F
    )
  )

  cd %~dp0

  rem --------------------
  rem po File has to be copied to the same dir as xml2po.py
  rem otherwise the path to the po file will be in the translated docs.

  copy !POFILE! . > NUL

  FOR %%F in (!CHAPTERS!) DO (
    echo %%F
    xml2po.py -p %1.po !SRCDIR!\%%F > !TARGDIR!\%%F
  )

  del %1.po

  echo.
  echo Temporary solution
  echo Ugly hack to prevent the automation appendix from being translated.
  echo Copying tsvn_app_automation.xml from english source
  copy %SRCDIR%\tsvn_app_automation.xml %TARGDIR%

)

echo ----------------------------------------------------------------------
echo.
echo Generating: %1 %APPS%
cd source
FOR %%A in (%APPS%) DO call MakeDoc %%A %1 %TARGETS%
cd ..

Goto :EOF
rem | End of Subroutine -> return to caller
rem +----------------------------------------------------------------------

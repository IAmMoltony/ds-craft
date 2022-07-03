@echo off
rem this script goes through every png and grit file in gfx and
rem runs it through grit
rem make sure that <devkitpro dir>/tools/bin is on path

setlocal enabledelayedexpansion
for /f "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
    set "DEL=%%a"
)

cd gfx
del *.h > nul 2>&1
del *.s > nul 2>&1
del OK > nul 2>&1
if "%1"=="onlyfonts" goto :fonts
echo.
for /r %%i in (*.png *.bmp) do (
    echo | set /p="Processing %%i..."
    if not exist "%%~ni.grit" (
        call :colecho 0e "No %%~ni.grit"
        echo.
    ) else (
        grit %%~ni.png -ff%%~ni.grit > nul 2>&1
        if not errorlevel 0 (
            call :colecho 0c "Error!"
            echo.
            exit /b 1
        )
        move %%~ni.s ..\source\img\%%~ni_img.s >nul 2>&1
        move %%~ni.h ..\build\%%~ni_img.h >nul 2>&1
        call :colecho 0a "OK"
        echo.
    )
)
cd ..
goto :eof

:colecho
echo off
<nul set /p ".=%DEL%" > "%~2"
findstr /v /a:%1 /R "^$" "%~2" nul
del "%~2" > nul 2>&1
goto :eof
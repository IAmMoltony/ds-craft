@echo off
if not exist "releases" mkdir releases
if exist "releases\ds-craft.nds" del "releases\ds-craft.nds"

if "%1"=="" (
    echo Usage: %0 ^<release version^>
    echo Example: %0 alpha2.0.0
    exit /B 1
)

echo Creating ds-craft-%1.nds in releases...
make clean build
copy ds-craft.nds releases
cd releases
ren ds-craft.nds ds-craft-%1.nds
cd ..
echo ok!

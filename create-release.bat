@echo off
if not exist "releases" mkdir releases
if exist "releases\ds-craft.nds" (
    echo ds-craft.nds exists in releases directory. please delete or move it.
    exit /B 1
)

if "%1"=="" (
    echo Usage: %0 ^<release version^>
    echo Example: %0 alpha2.0.0
    exit /B 1
)

echo Creating ds-craft-%1.nds in releases...
make
copy ds-craft.nds releases
cd releases
ren ds-craft.nds ds-craft-%1.nds
cd ..
echo ok!
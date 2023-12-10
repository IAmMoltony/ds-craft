<#
.SYNOPSIS
   List all TODOs in the project.

.DESCRIPTION
   This script clears the screen and prints
   the TODOs in the source files, header
   files, scripts and the readme file. If the
   -NoClear option is specified, then the
   screen will not be cleared and the screen
   will just display the TODOs.

   Note: this script expects you to run it
   from the project's root directory.

.EXAMPLE
   .\scripts\lstodos.ps1
   Clear the screen and list TODOs.

.EXAMPLE
   .\scripts\lstodos.ps1 -NoClear
   Only list TODOs, without clearing the
   screen.
#>

param(
    [switch]$NoClear
)

if (-not $NoClear) {
    Clear-Host
}

$todoPattern = "@todo|TODO"

Get-Content .\source\* | Select-String -Pattern $todoPattern 
Get-Content .\include\* | Select-String -Pattern $todoPattern
Get-Content .\scripts\*.py | Select-String -Pattern $todoPattern
Get-Content .\README.md | Select-String -Pattern $todoPattern

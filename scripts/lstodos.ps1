Clear-Host
$todoPattern = "@todo|TODO"

Get-Content .\source\* | Select-String -Pattern $todoPattern 
Get-Content .\include\* | Select-String -Pattern $todoPattern
Get-Content .\scripts\*.py | Select-String -Pattern $todoPattern
Get-Content .\README.md | Select-String -Pattern $todoPattern

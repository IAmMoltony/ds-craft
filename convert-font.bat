@echo off
rem Add (devkitpro)/tools/bin to PATH to get access to grit
grit gfx/%1.bmp -W3 -gb -gB8
mv %1.h build
mv %1.s source/img
@echo off
grit gfx/%1.bmp -W3 -gb -gB8
move %1.h build/%1.h
move %1.s source/img/%1.s
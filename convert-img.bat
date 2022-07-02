@echo off
grit gfx/%1.png -W3 -gb -gB16
move %1.h build/%1_img.h
move %1.s source/img/%1_img.s

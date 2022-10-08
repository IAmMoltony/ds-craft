#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
# DATA is a list of directories containing binary data
# AUDIO is a list of directories containing sound and music files
# GRAPHICS is a list of directories containing files to be processed by grit
#
# All directories are specified relative to the project directory where
# the makefile is found
#
#---------------------------------------------------------------------------------
TARGET		:=	$(notdir $(CURDIR))
BUILD		:=	build
SOURCES		:=	source
DATA		:=	data
INCLUDES	:=	include
GRAPHICS	:=	gfx
AUDIO       :=  audio
NITRODATA   :=  nitrofs

CURDIR_BASENAME := $(shell basename $(CURDIR))

ifeq ($(CURDIR_BASENAME),build)
VERSION_MAJOR  := $(shell sed '1!d' ../$(NITRODATA)/game.ver)
VERSION_MINOR  := $(shell sed '2!d' ../$(NITRODATA)/game.ver)
VERSION_PATCH  := $(shell sed '3!d' ../$(NITRODATA)/game.ver)
VERSION_PREFIX := $(shell sed '4!d' ../$(NITRODATA)/game.ver)
else
VERSION_MAJOR  := $(shell sed '1!d' $(NITRODATA)/game.ver)
VERSION_MINOR  := $(shell sed '2!d' $(NITRODATA)/game.ver)
VERSION_PATCH  := $(shell sed '3!d' $(NITRODATA)/game.ver)
VERSION_PREFIX := $(shell sed '4!d' $(NITRODATA)/game.ver)
endif

GAME_ICON      := ../icon.bmp
GAME_TITLE     := DS-Craft
GAME_SUBTITLE1 := Minecraft clone for NDS
ifeq ($(strip $(VERSION_PATCH)),0)
GAME_SUBTITLE2 := Version $(VERSION_PREFIX)$(VERSION_MAJOR).$(VERSION_MINOR)
else
GAME_SUBTITLE2 := Version $(VERSION_PREFIX)$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)
endif

EMULATOR := E:\melonds\melonds
ifeq ($(CURDIR_BASENAME),build)
SOUNDBANK := ../$(NITRODATA)/soundbank.bin
else
SOUNDBANK := $(NITRODATA)/soundbank.bin
endif

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro)
endif

include $(DEVKITARM)/base_rules

PORTLIBS	:=	$(PORTLIBS_PATH)/nds $(PORTLIBS_PATH)/armv5te
LIBNDS		:=	$(DEVKITPRO)/libnds

ifeq ($(strip $(GAME_TITLE)),)
GAME_TITLE	:=	$(notdir $(OUTPUT))
endif

ifeq ($(strip $(GAME_SUBTITLE1)),)
GAME_SUBTITLE1	:=	built with devkitARM
endif

ifeq ($(strip $(GAME_SUBTITLE2)),)
GAME_SUBTITLE2	:=	http://devkitpro.org
endif

ifeq ($(strip $(GAME_ICON)),)
GAME_ICON      :=      $(DEVKITPRO)/libnds/icon.bmp
endif

ifneq ($(strip $(NITRO_FILES)),)
_ADDFILES	:=	-d $(NITRO_FILES)
endif

#---------------------------------------------------------------------------------
%.nds: %.arm9
	$(SILENTCMD)ndstool -c $@ -9 $< -b $(GAME_ICON) "$(GAME_TITLE);$(GAME_SUBTITLE1);$(GAME_SUBTITLE2)" $(_ADDFILES)
	@echo built ... $(notdir $@)

#---------------------------------------------------------------------------------
%.nds: %.elf
	$(SILENTCMD)ndstool -c $@ -9 $< -b $(GAME_ICON) "$(GAME_TITLE);$(GAME_SUBTITLE1);$(GAME_SUBTITLE2)" $(_ADDFILES)
	$(SILENTMSG) built ... $(notdir $@)

#---------------------------------------------------------------------------------
%.arm9: %.elf
	$(SILENTCMD)$(OBJCOPY) -O binary $< $@
	$(SILENTMSG) built ... $(notdir $@)

#---------------------------------------------------------------------------------
%.arm7: %.elf
	$(SILENTCMD)$(OBJCOPY) -O binary $< $@
	$(SILENTMSG) built ... $(notdir $@)

#---------------------------------------------------------------------------------
%.elf: $(SOUNDBANK)
	$(SILENTMSG) linking $(notdir $@)
	$(SILENTCMD)$(LD) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH		:=	-mthumb -mthumb-interwork -march=armv5te -mtune=arm946e-s

CFLAGS	:=	-g -Wall -Wextra -Wno-free-nonheap-object -Wno-unknown-pragmas -O2 -Os\
 			-march=armv5te -mtune=arm946e-s \
			$(ARCH) -I../include -Wno-switch -Wno-ignored-qualifiers \
			-Wno-unused-parameter\
			-fomit-frame-pointer\
			-ffast-math

CFLAGS	+=	$(INCLUDE) -DARM9
CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions -Wno-reorder

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:= -lmm9 -lfat -lnds9 -lfilesystem -lnds9d
 
 
#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(LIBNDS)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------


ifneq ($(BUILDDIR),$(CURDIR))
#---------------------------------------------------------------------------------
export TOPDIR   :=  $(CURDIR)
export OUTPUT	:=	$(CURDIR)/$(TARGET)
 
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
					$(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

ifneq ($(strip $(NITRODATA)),)
	export NITRO_FILES  :=  $(CURDIR)/$(NITRODATA)
endif

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))
BMPFILES	:=	$(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.bmp)))
PNGFILES	:=	$(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.png)))

export AUDIOFILES := $(foreach dir,$(notdir $(wildcard $(AUDIO)/*.wav)),$(CURDIR)/$(AUDIO)/$(dir))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export OFILES := $(PNGFILES:.png=.o) $(BMPFILES:.bmp=.o) $(OFILES_BIN) $(OFILES_SOURCES)

#export OFILES_FILTER := $(filter-out $(SOUNDBANK).o,$(OFILES))
#$(info $(OFILES_FILTER))

export HFILES := $(PNGFILES:.png=.h) $(BMPFILES:.bmp=.h) $(addsuffix .h,$(subst .,_,$(BINFILES)))
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-iquote $(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD) -Iinclude
 
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean run
 
#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) BUILDDIR=`cd $(BUILD) && pwd` --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).nds $(SOUNDBANK)

#---------------------------------------------------------------------------------
run:
	@echo run ...
	@$(EMULATOR) $(TARGET).nds


#---------------------------------------------------------------------------------
else
 
DEPENDS	:=	$(OFILES:.o=.d)
 
#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).nds	: 	$(OUTPUT).elf
$(OUTPUT).nds   :   $(shell find ../$(NITRODATA))
$(OUTPUT).elf	:	$(OFILES)


#---------------------------------------------------------------------------------
# The bin2o rule should be copied and modified
# for each extension used in the data directories
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
%.bin.o %_bin.h : %.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

$(SOUNDBANK): $(AUDIOFILES)
	@mmutil $^ -hsoundbank.h -o$@ -d

#---------------------------------------------------------------------------------
# This rule creates assembly source files using grit
# grit takes an image file and a .grit describing how the file is to be processed
# add additional rules like this for each image extension
# you use in the graphics folders 
#---------------------------------------------------------------------------------
%.s %.h	: %.bmp %.grit
#---------------------------------------------------------------------------------
	grit $< -fts -o$*

#---------------------------------------------------------------------------------
%.s %.h	: %.png %.grit
#---------------------------------------------------------------------------------
	grit $< -fts -o$*


-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------

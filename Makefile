

# Copyright (c) 2015 Andy Little

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>

# modify to change your app name
APPLICATION_NAME = osd_maker.exe

######################## #######################################
# mod these to your compiler must be a C++11 gcc compiler

CC = C:/cpp/tdm-gcc/bin/g++
LD = C:/cpp/tdm-gcc/bin/g++
# Modify this to your quan include path
QUAN_INCLUDE_PATH = C:/cpp/lib/quan-trunk_lib/
WX_INCLUDE_PATH = C:/cpp/wxWidgets-3.0.0/include/
WX_LIB_PATH = C:/cpp/wxWidgets-3.0.0/lib/gcc481TDM_dll

# try comment this ...
#C_INCLUDE_PATH = /usr/include/i386-linux-gnu

# and uncomment this if you get compile errors
# On my Ubuntu system the C++ headers and default compiler are not c++11
# The C_INCLUDE_PATH is required to get it to compile

INCLUDES = -I$(QUAN_INCLUDE_PATH) -I$(WX_INCLUDE_PATH)
#INCLUDES = -I$(QUAN_INCLUDE_PATH) -I$(C_INCLUDE_PATH)

#########################################################

local_objects = document.o osd_bmp_app.o
local_document_objects = open_project.o mcm_file.o osd_object_database.o
local_gui_objects = font_preview.o main_frame.o main_frame_splitter.o panel.o \
panel_splitter.o project_tree.o view.o
local_graphics_api_objects_objects = db_bitmap.o db_font.o osd_device.o
local_gui_dialogs_objects = bitmap_resize_dialog.o new_bitmap_dialog.o
local_gui_font_preview_objects = grid_cell_bmp_renderer.o
local_gui_main_frame_objects = OnCreateStaticBitmapFile.o \
OnCreateStaticFontFile.o
local_gui_view_objects = drawing.o paint_layout_view.o

quan_gx_wxwidgets_objects = draw_box.o draw_circle.o draw_line.o draw_poly_line.o \
graphics_context.o draw_filled_box.o

objects = $(local_objects) $(local_document_objects) $(local_gui_objects) \
$(local_graphics_api_objects_objects) $(local_gui_dialogs_objects) \
$(local_gui_font_preview_objects) $(local_gui_main_frame_objects) \
$(local_gui_view_objects) $(quan_gx_wxwidgets_objects) static_rgb_colours.o \
writable_bitmap.o dynamic_bitmap.o

CFLAGS = -Wall -std=gnu++11 -Os -D__WXMSW__

# Current wxWidgets lib (3.0.0) gives loads of warning
# about deprecated-declarations
CFLAGS += -Wno-deprecated-declarations

#LFLAGS =  -static
LFLAGS =

#WX_LIBS = -Wl,--subsystem,windows -mwindows -lwx_mswu_xrc-3.0 \
#-lwx_mswu_webview-3.0 -lwx_mswu_html-3.0 -lwx_mswu_qa-3.0 -lwx_mswu_adv-3.0 \
#-lwx_mswu_core-3.0 -lwx_baseu_xml-3.0 -lwx_baseu_net-3.0 -lwx_baseu-3.0

WX_LIBS  = -Wl,--subsystem,windows -lwxmsw30u_xrc \
-lwxmsw30u_webview -lwxmsw30u_html -lwxmsw30u_adv \
-lwxmsw30u_core -lwxbase30u_xml -lwxbase30u_net \
-lwxbase30u -lwxpng -lwxzlib

.PHONY : clean all

all : $(APPLICATION_NAME)

$(local_objects) : %.o : %.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(local_document_objects) : %.o : document/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(local_gui_objects) : %.o : gui/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(local_graphics_api_objects_objects) : %.o : graphics_api/objects/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(local_gui_dialogs_objects) : %.o : gui/dialogs/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(local_gui_font_preview_objects) : %.o : gui/font_preview/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(local_gui_main_frame_objects) : %.o : gui/main_frame/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(local_gui_view_objects) : %.o : gui/view/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(quan_gx_wxwidgets_objects) : %.o : $(QUAN_INCLUDE_PATH)quan_matters/src/wxwidgets/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

#serial_port.o : $(QUAN_INCLUDE_PATH)quan_matters/src/serial_port.cpp
#	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

static_rgb_colours.o : $(QUAN_INCLUDE_PATH)quan_matters/src/gx/static_rgb_colours.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

writable_bitmap.o : $(QUAN_INCLUDE_PATH)quan_matters/src/uav/osd/detail/writable_bitmap.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

dynamic_bitmap.o : $(QUAN_INCLUDE_PATH)quan_matters/src/uav/osd/dynamic/bitmap.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(APPLICATION_NAME) : $(objects)
	$(LD) -Wl,-static $(CFLAGS) $(LFLAGS) -o $(APPLICATION_NAME) $(objects) -L$(WX_LIB_PATH) $(WX_LIBS)

clean:
	-rm -rf *.o *.exe



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
CC = /opt/gcc-4.7.2/bin/g++
LD = /opt/gcc-4.7.2/bin/g++
# Modify this to your quan include path
QUAN_INCLUDE_PATH = /home/andy/website/quan-trunk/

# try comment this ...
C_INCLUDE_PATH = /usr/include/i386-linux-gnu

# and uncomment this if you get compile errors
# On my Ubuntu system the C++ headers and default compiler are not c++11
# The C_INCLUDE_PATH is required to get it to compile

#INCLUDES = -I$(QUAN_INCLUDE_PATH) 
INCLUDES = -I$(QUAN_INCLUDE_PATH) -I$(C_INCLUDE_PATH)

#########################################################

local_objects = document.o osd_bmp_app.o
local_document_objects = mcm_file.o osd_object_database.o
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

# could add -Os etc here
CFLAGS = -Wall -std=c++11 -Os

LFLAGS =

.PHONY : clean all

all : $(APPLICATION_NAME)

$(local_objects) : %.o : %.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

$(local_document_objects) : %.o : document/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

$(local_gui_objects) : %.o : gui/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

$(local_graphics_api_objects_objects) : %.o : graphics_api/objects/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

$(local_gui_dialogs_objects) : %.o : gui/dialogs/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

$(local_gui_font_preview_objects) : %.o : gui/font_preview/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

$(local_gui_main_frame_objects) : %.o : gui/main_frame/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

$(local_gui_view_objects) : %.o : gui/view/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

$(quan_gx_wxwidgets_objects) : %.o : $(QUAN_INCLUDE_PATH)quan_matters/src/wxwidgets/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

#serial_port.o : $(QUAN_INCLUDE_PATH)quan_matters/src/serial_port.cpp
#	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

static_rgb_colours.o : $(QUAN_INCLUDE_PATH)quan_matters/src/gx/static_rgb_colours.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

writable_bitmap.o : $(QUAN_INCLUDE_PATH)quan_matters/src/uav/osd/detail/writable_bitmap.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

dynamic_bitmap.o : $(QUAN_INCLUDE_PATH)quan_matters/src/uav/osd/dynamic/bitmap.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

$(APPLICATION_NAME) : $(objects)
	$(LD) $(CFLAGS) $(LFLAGS) -o $(APPLICATION_NAME) $(objects) -ldl `wx-config --libs`

clean:
	-rm -rf *.o *.exe

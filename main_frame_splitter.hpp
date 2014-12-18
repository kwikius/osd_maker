#ifndef OSD_BITMAP_MAKER_MAIN_FRAME_SPLITTER_HPP_INCLUDED
#define OSD_BITMAP_MAKER_MAIN_FRAME_SPLITTER_HPP_INCLUDED

#include <wx/splitter.h>

//#include "panel.hpp"
#include "view.hpp"
#include "panel_splitter.hpp"

struct main_frame_splitter : wxSplitterWindow{
   main_frame_splitter(wxWindow* parent);

   view * m_view;
   panel_splitter* m_panel_splitter;
   
};

#endif // OSD_BITMAP_MAKER_MAIN_FRAME_SPLITTER_HPP_INCLUDED

#ifndef OSD_BITMAP_MAKER_PANEL_SPLITTER_HPP_INCLUDED
#define OSD_BITMAP_MAKER_PANEL_SPLITTER_HPP_INCLUDED

#include <wx/splitter.h>

#include "panel.hpp"
#include "bitmap_preview.hpp"

struct panel_splitter : wxSplitterWindow{
   panel_splitter(wxWindow* parent);

   panel* m_panel;
   bitmap_preview * m_bitmap_preview;
   
};

#endif // OSD_BITMAP_MAKER_PANEL_SPLITTER_HPP_INCLUDED

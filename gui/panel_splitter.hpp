#ifndef OSD_BITMAP_MAKER_PANEL_SPLITTER_HPP_INCLUDED
#define OSD_BITMAP_MAKER_PANEL_SPLITTER_HPP_INCLUDED

#include <wx/splitter.h>

#include "panel.hpp"
#include "font_preview.hpp"

struct panel_splitter : wxSplitterWindow{
   panel_splitter(wxWindow* parent);

   panel* m_panel;
   font_preview * m_font_preview;
   bool Destroy();

};

#endif // OSD_BITMAP_MAKER_PANEL_SPLITTER_HPP_INCLUDED

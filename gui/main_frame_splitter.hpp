#ifndef OSD_BITMAP_MAKER_MAIN_FRAME_SPLITTER_HPP_INCLUDED
#define OSD_BITMAP_MAKER_MAIN_FRAME_SPLITTER_HPP_INCLUDED

#include <wx/splitter.h>

#include "view.hpp"
#include "tree_panel_splitter.hpp"

//class OsdBmpApp;
struct main_frame_splitter : wxSplitterWindow{
   main_frame_splitter(wxWindow* parent);
   bool Destroy();
   view* get_view() const { return m_view;}
   tree_panel_splitter* get_tree_panel_splitter() const {return m_tree_panel_splitter;}
   private:
   //friend class OsdBmpApp;
   view * m_view;
   tree_panel_splitter* m_tree_panel_splitter;
};

#endif // OSD_BITMAP_MAKER_MAIN_FRAME_SPLITTER_HPP_INCLUDED

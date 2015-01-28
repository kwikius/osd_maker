#ifndef OSD_BITMAP_MAKER_BITMAP_PREVIEW_HPP_INCLUDED
#define OSD_BITMAP_MAKER_BITMAP_PREVIEW_HPP_INCLUDED

#include <wx/wx.h>
#include <wx/grid.h>

#include "font.hpp"

   struct bitmap_preview : wxWindow {
      bitmap_preview (wxWindow* parent);
      // void OnPaint(wxPaintEvent & event);
      enum{
         idGrid = 1000
      };
      void set_font_handle(int font_handle);
      int get_font_handle()const{ return m_current_font_handle;}
      font * get_font() const;
      void reset();
   private:
      void OnGridCellLeftDblClick(wxGridEvent& event);
      void OnGridCellRightClick(wxGridEvent& event);
      int get_sel_font_element_handle(wxGridEvent & event)const ;
      DECLARE_EVENT_TABLE()
      wxGrid* m_grid;
      int m_current_font_handle;
      void export_font_element_as_bitmap(int handle);
   };

#endif // OSD_BITMAP_MAKER_BITMAP_PREVIEW_HPP_INCLUDED

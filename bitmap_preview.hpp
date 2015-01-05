#ifndef OSD_BITMAP_MAKER_BITMAP_PREVIEW_HPP_INCLUDED
#define OSD_BITMAP_MAKER_BITMAP_PREVIEW_HPP_INCLUDED

#include <wx/wx.h>
#include <wx/grid.h>

   struct bitmap_preview : wxWindow {
      bitmap_preview (wxWindow* parent);
      // void OnPaint(wxPaintEvent & event);
      enum{
         idGrid = 1000
      };
      void set_font_handle(int font_handle);
      void reset();
   private:
      void OnGridCellLeftDblClick(wxGridEvent& event);
      DECLARE_EVENT_TABLE()
      wxGrid* m_grid;
      int m_current_font_handle;
   };

#endif // OSD_BITMAP_MAKER_BITMAP_PREVIEW_HPP_INCLUDED

//#include <cctype>

#include <wx/bitmap.h>
#include <wx/rawbmp.h>
#include <quan/max.hpp>
#include "app.h"
#include "osd_image.hpp"
#include "document.hpp"
#include "view.hpp"
#include "bitmap_preview.hpp"
#include "font.hpp"

void bitmap_preview::reset()
{
   m_current_font_handle = -1;
}

void bitmap_preview::set_font_handle(int font_handle)
{
   m_current_font_handle = font_handle;
}

BEGIN_EVENT_TABLE(bitmap_preview,wxWindow)

  //     EVT_PAINT(bitmap_preview::OnPaint)
//     EVT_SIZE(view::OnSize)
//     EVT_SCROLLWIN(view::OnScroll)
//     EVT_LEFT_DOWN(view::OnMouseLeftDown)
//     EVT_LEFT_UP(view::OnMouseLeftUp)
//     EVT_MOTION(view::OnMouseMove)
//     EVT_CHAR(view::OnChar)
       EVT_GRID_CMD_CELL_LEFT_DCLICK(idGrid,bitmap_preview::OnGridCellLeftDblClick)

END_EVENT_TABLE()

void bitmap_preview::OnGridCellLeftDblClick(wxGridEvent& event)
{
   if (m_current_font_handle == -1){
      return;
   }

   font* selected_font = wxGetApp().get_document()->get_font(m_current_font_handle);
  // assert(( selected_font != nullptr ) &&__LINE__);
   if (selected_font == nullptr){
      return;
   }
   int const x = event.GetCol();
   int const y = event.GetRow();

  int const num_elements = 256;//selected_font->get_num_elements();
  int const num_rows = 8;
  int const rem = ((num_elements % num_rows)==0)?0:1;
  int const num_cols = (num_elements / num_rows) + rem;
  int const font_pos = num_cols * y + x;
  int const ch = font_pos ;//+ selected_font->get_begin();
  int event_handle = -1;
  if (! selected_font->get_handle_at(ch, event_handle)){
      wxMessageBox(wxT("Failed to get font image handle"));
      return ;
   }
   if ( event_handle == -1){
    wxMessageBox(wxT("Invalid font element handle"));
      return ;
   }
   wxGetApp().get_view()->sync_with_image_handle(event_handle);
}

bitmap_preview::bitmap_preview(wxWindow* parent)
     : wxWindow{parent, wxID_ANY},m_grid{nullptr},m_current_font_handle{-1}
{
  wxSize cell_size{14 *2,18 *2};
  int num_elements = 256;
  int const num_rows = 8;
  int const rem = ((num_elements % num_rows)==0)?0:1;
  int const num_cols = (num_elements / num_rows) + rem;
  wxSize grid_size{num_cols * cell_size.x,num_rows * cell_size.y};
  this->m_grid = new wxGrid{this,idGrid,wxPoint{0,0},grid_size*2};
  m_grid->CreateGrid(num_rows,num_cols);
  m_grid->SetDefaultColSize(cell_size.x);
  m_grid->SetDefaultRowSize(cell_size.y);
  m_grid->SetRowLabelSize(0);
  m_grid->SetColLabelSize(0);
  m_grid->DisableDragGridSize();
  for ( int y = 0; y < num_rows; ++y){
    for ( int x = 0; x < num_cols; ++x){
      int ar_pos = y * num_cols + x;
      if ( ar_pos < num_elements){
            char ch = static_cast<char>(ar_pos);
            m_grid->SetCellValue(y,x,wxString::Format(wxT("%c"),ch));
            m_grid->SetReadOnly(y,x);
      }
    }
  }      
}

#if 0
namespace {

   // add wxImage
   // integer scaling
   wxColour const * colour_array[4] = {wxLIGHT_GREY,wxBLACK,wxWHITE,wxBLUE};
   
   void draw_bitmap(size_t idx, wxDC& dc, osd_image::pos_type const & position)
   {

      auto doc = wxGetApp().get_document();
      osd_image* pimage = doc->get_osd_image_ptr(idx);
      assert(pimage && __LINE__);
      auto bmp = ConvertTo_wxBitmap(*pimage, colour_array);
      dc.DrawBitmap(*bmp,position.x,position.y);
      delete bmp;
   }
}

void bitmap_preview::OnPaint(wxPaintEvent & event)
{
   wxPaintDC dc(this);
   dc.SetBackground(* wxBLUE_BRUSH); // sets background brush but doesnt clear
   dc.Clear(); //       need to invoke to clear using current background brush
   
   // maybe walk through
   // and find the largest bitmap
   // need to be able to slect them..
   auto doc = wxGetApp().get_document();
  // auto num_elements = doc->get_num_bitmap_elements();
  // auto num_elements = 0U;
   wxSize window_size = this->GetSize();
   if ( num_elements > 0){
      osd_image::size_type border{20,20};
      osd_image::pos_type pos = border;
      size_t max_ysize = 0;
      for (size_t idx = 0; idx < num_elements; ++idx){
        osd_image::pos_type draw_pos;
        osd_image::size_type size;
        doc->get_bitmap_size(idx,size);
        max_ysize = quan::max(max_ysize,size.y);
        if ( (pos.x + 2 * border.x + size.x ) < static_cast<size_t>(window_size.x)){
           draw_pos = pos;
           pos.x += size.x + border.x;
        } else{
           pos.x = border.x;
           pos.y += max_ysize + border.y;
           max_ysize = size.y;
           if ( (pos.y + size.y + border.y ) < static_cast<size_t>(window_size.y) ){
             draw_pos = pos;
           }else{
              return; 
           }
         }
         draw_bitmap(idx, dc, draw_pos);
         
      }

   }
   // if there is any data
   // get dc
   // rows cols
   // find number of bitmaps

}
#endif

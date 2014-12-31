//#include <cctype>

#include <wx/bitmap.h>
#include <wx/rawbmp.h>
#include <quan/max.hpp>
#include "app.h"
#include "osd_image.hpp"
#include "document.hpp"
#include "bitmap_preview.hpp"
#include "window_ids.hpp"

BEGIN_EVENT_TABLE(bitmap_preview,wxWindow)

       EVT_PAINT(bitmap_preview::OnPaint)
//     EVT_SIZE(view::OnSize)
//     EVT_SCROLLWIN(view::OnScroll)
//     EVT_LEFT_DOWN(view::OnMouseLeftDown)
//     EVT_LEFT_UP(view::OnMouseLeftUp)
//     EVT_MOTION(view::OnMouseMove)
//     EVT_CHAR(view::OnChar)

END_EVENT_TABLE()

bitmap_preview::bitmap_preview(wxWindow* parent)
     : wxWindow(parent, wxID_ANY){
     window_ids::bitmap_preview = this->GetId();
     this->SetSize(0,0,400,200);
     this->SetWindowStyle(wxVSCROLL | wxHSCROLL);
     this->SetScrollbar(wxVERTICAL,50,10,110);
     this->SetScrollbar(wxHORIZONTAL,50,10,110);

}



namespace {
   // add wxImage
   // integer scaling
   wxColour const * colour_array[4] = {wxLIGHT_GREY,wxBLACK,wxWHITE,wxBLUE};
   
   void draw_bitmap(size_t idx, wxDC& dc, osd_image::pos_type const & position)
   {
#if 0
      auto doc = wxGetApp().get_document();
      osd_image* pimage = doc->get_osd_image_ptr(idx);
      assert(pimage);
      auto bmp = ConvertTo_wxBitmap(*pimage, colour_array);
      dc.DrawBitmap(*bmp,position.x,position.y);
      delete bmp;
#endif
   }


}

void bitmap_preview::OnPaint(wxPaintEvent & event)
{
#if 0
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
#endif
}

//#include <cctype>

#include <wx/bitmap.h>
#include <wx/rawbmp.h>
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

void bitmap_preview::OnPaint(wxPaintEvent & event)
{
   wxPaintDC dc(this);
   dc.SetBackground(* wxBLUE_BRUSH); // sets background brush but doesnt clear
   dc.Clear(); //       need to invoke to clear using current background brush
   
   auto doc = wxGetApp().get_document();
   auto num_elements = doc->get_num_bitmap_elements();
   if ( num_elements > 0){

      osd_image* pimage = doc->get_osd_image_ptr(0);
  
      assert(pimage);
  
      osd_image::size_type size;
      doc->get_bitmap_size(0,size);
      wxBitmap bmp(size.x,size.y);
      wxNativePixelData pixels(bmp);
      
      for ( size_t y = 0; y < size.y; ++y){
         for ( size_t x = 0; x < size.x; ++x){
            osd_image::colour c;
            pimage->get_pixel_colour({x,y},c);
            wxColour const * colour = wxBLUE;
            switch(c){
               case osd_image::colour::black:
               colour = wxBLACK;
               break;
               case osd_image::colour::white:
               colour = wxWHITE;
               break;
               case osd_image::colour::grey:
               colour = wxLIGHT_GREY;
               break;
               default:
               break;
            }
            wxNativePixelData::Iterator p(pixels);
            p.MoveTo(pixels,x,y);
            p.Red() = colour->Red();
            p.Green() = colour->Green();
            p.Blue() = colour->Blue();
           // p.Alpa() = colour->Alpha();
         }

      }
      dc.DrawBitmap(bmp,20,20);
     
   }
   // if there is any data
   // get dc
   // rows cols
   // find number of bitmaps
}

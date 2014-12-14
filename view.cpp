#include <cctype>
#include "view.hpp"
#include "panel.hpp"
#include "events.hpp"
#include "window_ids.hpp"
#include "document.hpp"
#include <quan/gx/primitives/simple_line.hpp>


//#include "aircraft_symbol.hpp"

BEGIN_EVENT_TABLE(view,wxWindow)

 EVT_PAINT(view::OnPaint)
 EVT_SIZE(view::OnSize)
 EVT_SCROLLWIN(view::OnScroll)
 EVT_LEFT_DOWN(view::OnMouseLeftDown)
 EVT_LEFT_UP(view::OnMouseLeftUp)
 EVT_MOTION(view::OnMouseMove)
 EVT_CHAR(view::OnChar)

END_EVENT_TABLE()

view::view(wxWindow* parent)
: wxWindow(parent, wxID_ANY)
,m_aircraft_selected{false}

{
    window_ids::view = this->GetId();
    this->SetWindowStyle(wxVSCROLL | wxHSCROLL);
    this->SetScrollbar(wxVERTICAL,50,10,110);
    this->SetScrollbar(wxHORIZONTAL,50,10,110);
    this->m_drawing_view.set_scale(1);
    this->Refresh();

}

void view::set_scale(double const & v)
{
   m_drawing_view.set_scale(v);
  this->Refresh();
}

quan::gx::abc_color::ptr view::get_colour(uint8_t colour_id)
{
   switch( colour_id){
      case document::black :
         return quan::gx::rgb::colors::black;
      break;
      case document::white :
         return quan::gx::rgb::colors::white;
      break;
      case document::grey :
         return quan::gx::rgb::colors::gray;
      break;
      case document::transparent :
      default:
         return quan::gx::rgb::colors::blue;
      break;
   }
}

void view::OnPaint(wxPaintEvent & event)
{
   wxPaintDC dc(this);
   dc.SetBackground(* wxWHITE_BRUSH); // sets background brush but doesnt clear
   dc.Clear(); //       need to invoke to clear using current background brush
   
   quan::gx::wxwidgets::graphics_context wc{
      &dc, 
      &this->m_drawing,
      &this->m_drawing_view,
      &this->m_device_window
   };

  auto doc = wxGetApp().get_document();

  vect2_i num_pixels= doc->get_bitmap_size();
  vect2_mm pixel_size = doc->get_pixel_size_mm();
   // bitmap always centered
  vect2_mm bitmap_size {num_pixels.x * pixel_size.x,num_pixels.y * pixel_size.y};
  quan::two_d::box<mm> frame{
      -bitmap_size.x, bitmap_size.y,bitmap_size.x, -bitmap_size.y
  };
  
  wc.draw_filled_box({frame,mm{},quan::gx::rgb::colors::blue});
  
  vect2_mm px_pos {-bitmap_size.x/2,bitmap_size.y/2};
  vect2_mm cur_px_pos = px_pos;
  mm border{1};
   for ( int32_t y = 0; y <num_pixels.y; ++y){
      for ( int32_t x = 0; x <num_pixels.x; ++x){
         // add color
         uint8_t colour;
         doc->get_pixel_colour(vect2_i{x,y},colour);
         quan::two_d::box<mm> cur_px_box {
            cur_px_pos.x + border, 
            cur_px_pos.y - border,
            cur_px_pos.x + pixel_size.x -border,
            cur_px_pos.y - pixel_size.y +border
         };
         
         quan::gx::primitives::box<mm> cur_px{cur_px_box,mm{0},get_colour(colour)};
         wc.draw_filled_box({cur_px});
         cur_px_pos.x += pixel_size.x;
      }
      cur_px_pos.x = px_pos.x;
      cur_px_pos.y -= pixel_size.y;
   }
}

// update device_window size data when size changes
void view::OnSize(wxSizeEvent & event)
{
   this->GetClientSize(
         &this->m_device_window.m_size_px.x,
         &this->m_device_window.m_size_px.y
   );
   
   // find window size in mm by getting size of pixel on display
   // may be an ondisplay change function so dont need to continaully update
   vect2_i display_size_in_px;
   wxDisplaySize(&display_size_in_px.x,&display_size_in_px.y);

   wxSize detail_display_size_mm = wxGetDisplaySizeMM();
   vect2_mm display_size_in_mm{mm{detail_display_size_mm.x},mm{detail_display_size_mm.y}};
   
   this->m_device_window.m_size_mm.x 
   = (this->m_device_window.m_size_px.x * display_size_in_mm.x) / display_size_in_px.x;
   this->m_device_window.m_size_mm.y 
   = (this->m_device_window.m_size_px.y * display_size_in_mm.y) / display_size_in_px.y;
  
   this->Refresh();

}

void view::OnScroll(wxScrollWinEvent & event)
{
  if (event.GetOrientation() == wxHORIZONTAL){
     this->OnHScroll(event);
  }else{
     this->OnVScroll(event);
  }
  this->Refresh();
}

void view::OnHScroll(wxScrollWinEvent & event)
{
   this->m_drawing_view.set_x_scroll_ratio((event.GetPosition() - 50 )/100.0); 
}

void view::OnVScroll(wxScrollWinEvent & event)
{
   this->m_drawing_view.set_y_scroll_ratio( -(event.GetPosition() - 50 )/100.0);
}

/*
   if returns true then event_pos is over a valid image pixel
   The image pixel is put int result_pos
*/
bool view::get_image_pixel(vect2_d const & event_pos, vect2_i & result_pos)
{
   quan::gx::wxwidgets::graphics_info_context ic{
      &this->m_drawing,
      &this->m_drawing_view,
      &this->m_device_window
   };

   // in drawing 
   auto drawing_pos = ic.device_to_drawing(event_pos);
   auto doc = wxGetApp().get_document();
   vect2_i num_pixels= doc->get_bitmap_size();
   vect2_mm pixel_size = doc->get_pixel_size_mm();
   // bitmap always centered
   vect2_mm bitmap_size {num_pixels.x * pixel_size.x,num_pixels.y * pixel_size.y};
   vect2_mm offset{-bitmap_size.x/2 + pixel_size.x/2,bitmap_size.y/2 -pixel_size.y/2};
   for ( int32_t y = 0; y <num_pixels.y; ++y){
      mm py = offset.y  - pixel_size.y * y;
      if ( quan::abs( drawing_pos.y - py) < (pixel_size.y /2)) {
         for ( int32_t x = 0; x <num_pixels.x; ++x){
            mm px = offset.x + pixel_size.x * x;
            if ( quan::abs( drawing_pos.x - px) < (pixel_size.x /2)) {
               result_pos.x = x;
               result_pos.y = y;
               return true;
            }
         }
      }
   }
   return false;
}

void view::OnMouseLeftDown(wxMouseEvent & event)
{
   event.Skip();

#if 0
   vect2_i result_pos;
   if(get_image_pixel({event.GetX(),event.GetY()},result_pos) == true){
       wxMessageBox(wxString::Format(wxT("[%i,%i]"),result_pos.x,result_pos.y));
   };
   quan::gx::wxwidgets::graphics_info_context ic{
      &this->m_drawing,
      &this->m_drawing_view,
      &this->m_device_window
   };

   // in drawing 
   auto mouse_down_pos = ic.device_to_drawing({event.GetX(),event.GetY()});
   auto doc = wxGetApp().get_document();
   vect2_i num_pixels= doc->get_bitmap_size();
   vect2_mm pixel_size = doc->get_pixel_size_mm();
   // bitmap always centered
   vect2_mm bitmap_size {num_pixels.x * pixel_size.x,num_pixels.y * pixel_size.y};
    vect2_mm offset{-bitmap_size.x/2 + pixel_size.x/2,bitmap_size.y/2 -pixel_size.y/2};
    for ( int32_t y = 0; y <num_pixels.y; ++y){
        mm py = offset.y  - pixel_size.y * y;
        if ( quan::abs( mouse_down_pos.y - py) < (pixel_size.y /2)) {
            for ( int32_t x = 0; x <num_pixels.x; ++x){
                 mm px = offset.x + pixel_size.x * x;
                 if ( quan::abs( mouse_down_pos.x - px) < (pixel_size.x /2)) {
                     wxMessageBox(wxString::Format(wxT("[ %i,%i]"),x,y));
                     return;
                 }
            }
         }
   }
#endif
}
/*
   get image pixel if any
*/
void view::OnChar(wxKeyEvent & event)
{
  // event.Skip();
 
   vect2_i result_pos;
   if(get_image_pixel(m_cur_mouse_pos,result_pos) == true){
     // wxMessageBox(_T("GotOne"));
      int ch = toupper(event.GetKeyCode());
      int colour = -1;
      switch(ch){
         case 'B':
            colour = document::black;
         break;
         case 'W':
            colour = document::white;
         break;
         case 'T':
             colour = document::transparent;
         break;
         case 'G':
             colour = document::grey;
         break;
         default:
         break;
      }
      if ( colour != -1){
         auto doc = wxGetApp().get_document();
         doc->set_pixel_colour(result_pos, colour);
         this->Refresh();
      }
   }
}

void view::OnMouseLeftUp(wxMouseEvent & event)
{
  //m_aircraft_selected = false;
}

void view::OnMouseMove(wxMouseEvent & event)
{
    m_cur_mouse_pos.x = event.GetX();
    m_cur_mouse_pos.y = event.GetY();
}



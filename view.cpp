#include <cctype>
#include "view.hpp"
#include "panel.hpp"
#include "events.hpp"
#include "window_ids.hpp"
#include "document.hpp"
#include <quan/gx/primitives/simple_line.hpp>
#include "fontmap_dialog.hpp"


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
     ,m_cur_mouse_pos {0,0}
,m_mouse_is_down {false}
,m_current_bitmap_lib_index {-1} {
     window_ids::view = this->GetId();
     this->SetWindowStyle(wxVSCROLL | wxHSCROLL);
     this->SetScrollbar(wxVERTICAL,50,10,110);
     this->SetScrollbar(wxHORIZONTAL,50,10,110);
     this->m_drawing_view.set_scale(1);
     //this->Refresh();
     this->SetFocus();
}

void view::set_scale(double const & v)
{
     m_drawing_view.set_scale(v);
     this->Refresh();
}

void  view::set_current_bitmap_lib_index(int32_t val)
{
   m_current_bitmap_lib_index = val;
   wxGetApp().get_panel()->CurrentBitmapIndex->ChangeValue (wxString::Format (wxT ("%d"),val));
}

void  view::set_current_bitmap_size(osd_image::size_type const & size)
{
   wxGetApp().get_panel()->XsizeText->ChangeValue (wxString::Format (wxT ("%d"),size.x));
   wxGetApp().get_panel()->YsizeText->ChangeValue (wxString::Format (wxT ("%d"),size.y));
}

quan::gx::abc_color::ptr view::get_colour(osd_image::colour colour_id)
{
     switch( colour_id) {
     case osd_image::colour::black :
          return quan::gx::rgb::colors::black;
          break;
     case osd_image::colour::white :
          return quan::gx::rgb::colors::white;
          break;
     case osd_image::colour::grey :
          return quan::gx::rgb::colors::gray;
          break;
     case osd_image::colour::transparent :
     default:
          return quan::gx::rgb::colors::blue;
          break;
     }
}

void view::paint_bitmap_view(wxPaintEvent & event)
{
     wxPaintDC dc(this);
     dc.SetBackground(* wxWHITE_BRUSH); // sets background brush but doesnt clear
     dc.Clear(); //       need to invoke to clear using current background brush
 
     quan::gx::wxwidgets::graphics_context wc {
          &dc,
          &this->m_drawing,
          &this->m_drawing_view,
          &this->m_device_window
     };
 
     auto current_index = this->get_current_bitmap_lib_index();
     // ok checked for -1

     if( current_index >=0 ) {
          auto doc = wxGetApp().get_document();
          osd_image::size_type num_pixels;
          if (!doc->get_bitmap_size(current_index,num_pixels)) {
               return ;
          }
          // should be view
          vect2_mm pixel_size = doc->get_pixel_size_mm();
          // bitmap always centered
          vect2_mm bitmap_size {num_pixels.x * pixel_size.x,num_pixels.y * pixel_size.y};
          quan::two_d::box<mm> frame {
               -bitmap_size.x, bitmap_size.y,bitmap_size.x, -bitmap_size.y
          };
          vect2_mm px_pos {-bitmap_size.x/2,bitmap_size.y/2};
          vect2_mm cur_px_pos = px_pos;
          wc.draw_filled_box( {frame,mm{},quan::gx::rgb::colors::blue});
          for ( size_t y = 0; y <=num_pixels.y; ++y) {
               quan::gx::primitives::simple_line<mm> line {
                    {-bitmap_size.x,px_pos.y - pixel_size.y * y},
                    { bitmap_size.x,px_pos.y - pixel_size.y * y},
                    mm{0},
                    quan::gx::rgb::colors::silver
               };
               wc.draw_line(line);
          }
          for ( size_t x = 0; x <=num_pixels.x; ++x) {
               quan::gx::primitives::simple_line<mm> line {
                    {px_pos.x + pixel_size.x * x,bitmap_size.y},
                    {px_pos.x + pixel_size.x * x,-bitmap_size.y},
                    mm{0},
                    quan::gx::rgb::colors::silver
               };
               wc.draw_line(line);
          }
 
          mm border {1};
          for ( size_t y = 0; y <num_pixels.y; ++y) {
 
               for ( size_t x = 0; x <num_pixels.x; ++x) {
                    // add color
                    osd_image::colour colour;
                    doc->get_pixel_colour(current_index,osd_image::pos_type {x,y},colour);
                    quan::two_d::box<mm> cur_px_box {
                         cur_px_pos.x + border,
                         cur_px_pos.y - border,
                         cur_px_pos.x + pixel_size.x -border,
                         cur_px_pos.y - pixel_size.y +border
                    };
 
                    quan::gx::primitives::box<mm> cur_px {cur_px_box,mm{0},get_colour(colour)};
                    wc.draw_filled_box( {cur_px});
                    cur_px_pos.x += pixel_size.x;
               }
               cur_px_pos.x = px_pos.x;
               cur_px_pos.y -= pixel_size.y;
          }
     }

}
 
 
void view::OnPaint(wxPaintEvent & event)
{
     paint_bitmap_view(event);
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
     vect2_mm display_size_in_mm {mm{detail_display_size_mm.x},mm{detail_display_size_mm.y}};
 
     this->m_device_window.m_size_mm.x
     = (this->m_device_window.m_size_px.x * display_size_in_mm.x) / display_size_in_px.x;
     this->m_device_window.m_size_mm.y
     = (this->m_device_window.m_size_px.y * display_size_in_mm.y) / display_size_in_px.y;
 
     this->Refresh();
 
}
 
void view::OnScroll(wxScrollWinEvent & event)
{
     if (event.GetOrientation() == wxHORIZONTAL) {
          this->OnHScroll(event);
     } else {
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
bool view::get_image_pixel(vect2_d const & event_pos, osd_image::pos_type & result_pos)
{

     auto current_index = this->get_current_bitmap_lib_index();
     if (current_index == -1) {
          return false;
     }
     quan::gx::wxwidgets::graphics_info_context ic {
          &this->m_drawing,
          &this->m_drawing_view,
          &this->m_device_window
     };
 
     // in drawing
     auto drawing_pos = ic.device_to_drawing(event_pos);
     auto doc = wxGetApp().get_document();
 
     osd_image::size_type num_pixels;
     doc->get_bitmap_size(current_index,num_pixels);
     vect2_mm pixel_size = doc->get_pixel_size_mm();
     // bitmap always centered
     vect2_mm bitmap_size {num_pixels.x * pixel_size.x,num_pixels.y * pixel_size.y};
     vect2_mm offset {-bitmap_size.x/2 + pixel_size.x/2,bitmap_size.y/2 -pixel_size.y/2};
     for ( size_t y = 0; y <num_pixels.y; ++y) {
          mm py = offset.y  - pixel_size.y * y;
          if ( quan::abs( drawing_pos.y - py) < (pixel_size.y /2)) {
               for ( size_t x = 0; x <num_pixels.x; ++x) {
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
     m_mouse_is_down=true;
     // auto doc = wxGetApp().get_document();
     //  auto p = new fontmap_dialog(this);
     //  p->Show();
}
/*
   get image pixel if any
*/
void view::OnChar(wxKeyEvent & event)
{
     event.Skip();
     auto index = this->get_current_bitmap_lib_index();
     if ( index == -1) {
          return ;
     }
     osd_image::pos_type result_pos;
     if ( m_mouse_is_down &&
               (get_image_pixel(m_cur_mouse_pos,result_pos) == true)
        ) {
          int ch = toupper(event.GetKeyCode());
          osd_image::colour colour = osd_image::colour::invalid;
          switch(ch) {
          case 'B':
               colour = osd_image::colour::black;
               break;
          case 'W':
               colour = osd_image::colour::white;
               break;
          case 'T':
               colour = osd_image::colour::transparent;
               break;
          case 'G':
               colour = osd_image::colour::grey;
               break;
          default:
               break;
          }
          if ( colour != osd_image::colour::invalid) {
               auto doc = wxGetApp().get_document();
               doc->set_pixel_colour(index,result_pos, colour);
               this->Refresh();
          }
     }
}
 
void view::OnMouseLeftUp(wxMouseEvent & event)
{
     m_mouse_is_down=false;
}
 
void view::OnMouseMove(wxMouseEvent & event)
{
     m_cur_mouse_pos.x = event.GetX();
     m_cur_mouse_pos.y = event.GetY();
}
 
 
 
 
 
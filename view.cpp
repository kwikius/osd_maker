
#include <cctype>

#include <quan/gx/primitives/simple_line.hpp>

#include "main_frame.h"
#include "view.hpp"
#include "panel.hpp"
#include "events.hpp"
//#include "window_ids.hpp"
#include "document.hpp"
#include "fontmap_dialog.hpp"
#include "bitmap_preview.hpp"
#include "dialogs/new_bitmap_dialog.hpp"

BEGIN_EVENT_TABLE(view,wxWindow)

     EVT_PAINT(view::OnPaint)
     EVT_SIZE(view::OnSize)
     EVT_SCROLLWIN(view::OnScroll)
     EVT_LEFT_DOWN(view::OnMouseLeftDown)
     EVT_RIGHT_DOWN(view::OnMouseRightDown)
     EVT_LEFT_UP(view::OnMouseLeftUp)
     EVT_MOTION(view::OnMouseMove)
     EVT_CHAR(view::OnChar)

END_EVENT_TABLE()

view::view(wxWindow* parent)
     : wxWindow(parent, wxID_ANY)
     ,m_cur_mouse_pos {0,0}
,m_mouse_is_down {false}
,m_current_image{nullptr}
,m_document_image_handle{-1}
,m_current_image_modified{false}
,m_view_mode{view_mode::inBitmaps}
,m_bearing{0}
,m_home_bearing{0} {
   //  window_ids::view = this->GetId();
     this->SetWindowStyle(wxVSCROLL | wxHSCROLL);
     this->SetScrollbar(wxVERTICAL,50,10,110);
     this->SetScrollbar(wxHORIZONTAL,50,10,110);
     this->m_drawing_view.set_scale(1);
     this->SetFocus();
}
font* view::get_current_font()const
{
    int font_handle = wxGetApp().get_bitmap_preview()->get_font_handle();
    if ( font_handle == -1){
      return nullptr;
    }
    return wxGetApp().get_document()->get_font(font_handle);
}

//TODO disable scrollbars or some other stuff with them
void view::set_view_mode(view_mode mode) 
{m_view_mode = mode;}

bool view::resize_image( quan::two_d::box<int> new_size)
{
   if ( m_view_mode != view_mode::inBitmaps){
     wxMessageBox(wxT("1"));
      return false;
   }
   // check its a bmp
   if (  m_current_image == nullptr){
       wxMessageBox(wxT("2"));
      return false;
   }
   if ( m_current_image->get_image_type() != osd_image::image_type::Bitmap){
       wxMessageBox(wxT("3"));
      return false;
   }
   osd_bitmap* bmp = dynamic_cast<osd_bitmap*>(m_current_image);
   if ( ! bmp){
       wxMessageBox(wxT("4"));
      return false;
   }
   if (! bmp->resize(new_size)){
       wxMessageBox(wxT("5"));
      return false;
   }
   this->set_modified (true);
   wxGetApp().get_main_frame()->enable_save_project(true);
   this->Refresh();
   return true;
}

void view::reset()
{
  if ( m_current_image!= nullptr){
      m_current_image->destroy();
      m_current_image = nullptr;
  }
  m_document_image_handle = -1;
  m_current_image_modified = false;
}

void view::sync_to_document()
{
   auto doc = wxGetApp().get_document();
   osd_image* image = this->clone_current_image();
   int image_handle = this->get_doc_image_handle();
//## need to 
//   osd_bitmap* bmp = dynamic_cast<osd_bitmap*> (image);
//   assert( bmp);
   doc->set_image( image_handle, image );
   doc->set_modified (true);
   this->set_modified (false);
}

int view::sync_hmi_view()
{
   if (this->is_modified()) {
      int result = wxMessageBox (wxT ("View modified. Commit to live tree?"),
      wxT ("Confirm View Commit"),
      wxICON_QUESTION | wxYES_NO | wxCANCEL);
      if (result ==  wxYES ) {
         this->sync_to_document();
      }
      return result;
   }else{
      return wxYES;
   }
}

bool view::sync_with_image_handle(int event_handle)
{
   bool const need_mode_change = this->get_view_mode() == view_mode::inLayouts;
   if (need_mode_change){
     this->set_view_mode(view_mode::inBitmaps);
     this->Refresh();
   }
   int view_handle = this->get_doc_image_handle();
   
   if ( view_handle == event_handle){
      if ( !this->is_modified()){
         return false;
      }else{
         if( wxMessageBox(
           wxT("[OK] to Revert view to live-tree?"),
           wxT("Confirm View Revert"),
           wxICON_QUESTION |wxOK | wxCANCEL ) != wxOK){
           return false;
         }
      }
   }else{ // not same image in view
     if ( this->is_modified()){
        if ( this->sync_hmi_view() == wxCANCEL){
            return false;
        }
     }
   }
   this->copy_to_current_image(event_handle);
   return true;
}

void view::set_modified(bool val)
{
   m_current_image_modified = val;
   wxGetApp().get_main_frame()->enable_commit_view_to_tree(val);
}

void view::copy_to_current_image( int handle)
{
   auto image = wxGetApp().get_document()->get_image(handle);
   assert ( (image != nullptr) && __LINE__);
   if ( m_current_image != nullptr){
      m_current_image->destroy();
   }
   m_current_image = image->clone();
   m_document_image_handle = handle;
   this->set_modified(false);
   this->Refresh();
}

void view::set_scale(double const & v)
{
     m_drawing_view.set_scale(v);
     this->Refresh();
}
double view::get_scale()
{
   return m_drawing_view.get_scale();
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
 
     if( m_current_image != nullptr) {
          auto doc = wxGetApp().get_document();
          osd_image::size_type num_pixels = m_current_image->get_size();

          // should be view or config
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
                   // doc->get_pixel_colour(current_index,osd_image::pos_type {x,y},colour);
                    m_current_image->get_pixel_colour(osd_image::pos_type {x,y},colour);
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
#if 0
void view::paint_layout_view(wxPaintEvent & event)
{
     m_display_layout.clear();
//     if( this->have_image()){
//         m_display_layout.bitmap_out({100,100},m_current_image);
//     }
     auto f = this->get_current_font();
     if( f != nullptr){
         m_display_layout.text_out({100,100},"HELLO WORLD",f);
     }
     m_display_layout.set_origin( m_display_layout.get_display_size()/2);
     m_display_layout.line_out({-100,-100},{-50,-50},display_layout::colour::white);
     m_display_layout.line_out({100,100},{50,50},display_layout::colour::white);
     m_display_layout.line_out({-50,50},{-100,100},display_layout::colour::black);
     m_display_layout.line_out({50,-50},{100,-100},display_layout::colour::black);

     m_display_layout.arc_out(100,quan::angle::deg{150},quan::angle::deg{30},20,display_layout::colour::black);

     auto image_size = m_display_layout.get_display_size();
     m_display_layout.rescale(image_size*2);

     wxBitmap bitmap{m_display_layout.get_image()};
     wxPaintDC dc(this);
     dc.SetBackground(* wxBLACK_BRUSH); 
     dc.Clear();
     dc.DrawBitmap(bitmap,0,0);
}
 #endif
 
void view::OnPaint(wxPaintEvent & event)
{
     switch ( this->get_view_mode()){
         case view_mode::inBitmaps:
            paint_bitmap_view(event);
         break;
         case view_mode::inLayouts:
           paint_layout_view(event);
         break;   
         default:
         break;
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
      if ( ! m_current_image){
         return false;
      }

//     auto current_index = this->get_current_bitmap_lib_index();
//     if (current_index == -1) {
//          return false;
//     }
     quan::gx::wxwidgets::graphics_info_context ic {
          &this->m_drawing,
          &this->m_drawing_view,
          &this->m_device_window
     };
 
     // in drawing
     auto drawing_pos = ic.device_to_drawing(event_pos);
     auto doc = wxGetApp().get_document();
 
     osd_image::size_type num_pixels = m_current_image->get_size();
  
    // doc->get_bitmap_size(current_index,num_pixels);
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
}
/*
   get image pixel if any
*/

void view::OnMouseRightDown( wxMouseEvent & event)
{

  new_bitmap_dialog dlg(this);
  dlg.ShowModal();
}

void view::OnChar(wxKeyEvent & event)
{
    event.Skip();
    if ( get_view_mode() == view_mode::inBitmaps ){
      on_bitmaps_char(event);
    }
}

void view::on_bitmaps_char(wxKeyEvent & event)
{

     if (m_current_image == nullptr){
         return;
     }
     osd_image::pos_type result_pos;
     if ( m_mouse_is_down &&
               (get_image_pixel(m_cur_mouse_pos,result_pos) == true)
        ) {
          int ch = toupper(event.GetKeyCode());
          osd_image::colour new_colour = osd_image::colour::invalid;
          switch(ch) {
          case 'B':
               new_colour = osd_image::colour::black;
               break;
          case 'W':
               new_colour = osd_image::colour::white;
               break;
          case 'T':
               new_colour = osd_image::colour::transparent;
               break;
          case 'G':
               new_colour = osd_image::colour::grey;
               break;
          default:
               break;
          }
          if ( new_colour != osd_image::colour::invalid) {
               osd_image::colour cur_colour = osd_image::colour::invalid;
               m_current_image->get_pixel_colour(result_pos, cur_colour);
               if ( new_colour != cur_colour){
                  m_current_image->set_pixel_colour(result_pos, new_colour);
                  this->set_modified(true);
                  wxGetApp().get_main_frame()->enable_save_project(true);
                  wxGetApp().get_main_frame()->enable_save_project_as(true);
                  this->Refresh();
               }
          }
     }else {
          int ch = event.GetKeyCode();
          switch(ch) {
          case '+':{
               double scale = this->get_scale();
               scale += 0.1;
               if (scale >1.0){
                  scale = 1.0;
               }  
               this->set_scale(scale);
          }
          break;
          case '-':{
               double scale = this->get_scale();
               scale -= 0.1;
               if (scale < 0.1){
                  scale = 0.1;
               }  
               this->set_scale(scale);
           }
            break;
            default:
            break;

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

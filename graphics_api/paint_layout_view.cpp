
#include <quan/two_d/rotation.hpp>
#include "../view.hpp"
#include "../app.h"

namespace {
void draw_compass(display_layout & d, quan::angle::deg bearing) // add ofset and dir
{
 
   quan::two_d::rotation const rotate{bearing};
   //auto const display_size = d.get_display_size();
  // display_layout::pxp constexpr pos = {-70,158};
   display_layout::pxp constexpr pos = {0,-188};
  // auto f = wxGetApp().get_view()->get_current_font();
   auto f =  wxGetApp().get_document()->get_font("MWOSD");
   if (f){
      display_layout::size_type const char_size{f->get_char_width(),f->get_char_height()};
      display_layout::pos_type const  char_offset = - char_size /2;
      constexpr auto font_radius = 32;
      constexpr char compass_char[]{'N','S','E','W'};
      constexpr display_layout::pos_type compass_vector[] {
            {0,font_radius}
            ,{0, - font_radius}
            ,{font_radius,0}
            ,{-font_radius,0}
      };
      for ( size_t i = 0; i < 4; ++i){
         int char_handle =-1;
         f->get_handle_at(compass_char[i],char_handle);
         assert( (char_handle != -1) && __LINE__);
         auto const char_bmp = wxGetApp().get_document()->get_image(char_handle);
         if (char_bmp) {
            auto char_pos = rotate(compass_vector[i]) + char_offset;
            d.bitmap_out(pos + char_pos,char_bmp);
         }
      }
   }

   int const radius = 40;
   d.circle_out(pos,radius+1,48,display_layout::colour::black);
   for ( int i = 0; i < 16; ++i){
      constexpr auto offset = quan::angle::deg{360/32};
      auto start_angle = quan::angle::deg{360/16} * i + offset;
      auto end_angle = quan::angle::deg{360/16} * (i+1) + offset;
      display_layout::colour col =( i & 1)? display_layout::colour::black : display_layout::colour::white;
      d.arc_out(pos,radius,start_angle,end_angle,3,col);
   }
   d.circle_out(pos,radius-1,48,display_layout::colour::black);

   auto arrow = wxGetApp().get_document()->get_bitmap("arrow");
   if (arrow){
      display_layout::size_type vect = arrow->get_size()/2;
      d.rotated_bitmap_out(pos,arrow,vect,bearing);
   }
}

}//namespace

void view::paint_layout_view(wxPaintEvent & event)
{
     m_display_layout.clear();
     auto f = this->get_current_font();
     if( f != nullptr){
        // wxMessageBox(wxT("GotFont"));
         m_display_layout.text_out({0,0},"HELLO WORLD",f);
     }

     m_display_layout.line_out({-100,0},{0,0},display_layout::colour::white);

     m_display_layout.line_out({0,0},{0,-100},display_layout::colour::black);

      draw_compass(m_display_layout,get_bearing());
      
//     auto image_size = m_display_layout.get_display_size();
//     m_display_layout.rescale(image_size*2);

     wxBitmap bitmap{m_display_layout.get_image()};
     wxPaintDC dc(this);
     dc.SetBackground(* wxBLACK_BRUSH); 
     dc.Clear();
     dc.DrawBitmap(bitmap,0,0);
}

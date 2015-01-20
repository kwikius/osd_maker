
#include <quan/two_d/rotation.hpp>
#include "../view.hpp"
#include "../app.h"

namespace {
void draw_compass(display_layout & d, quan::angle::deg bearing) // add ofset and dir
{
 
   quan::two_d::rotation const rotate{bearing};
   //auto const display_size = d.get_display_size();
   display_layout::pxp constexpr pos = {263,-188};
   auto f = wxGetApp().get_view()->get_current_font();
   
   if (f){
      display_layout::size_type const char_size{f->get_char_width(),f->get_char_height()};
      display_layout::pos_type const  char_offset = - char_size /2;
      constexpr auto font_radius = 36;
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
#if 0
   constexpr int arrow_len = 26;
   constexpr auto arrow_point = display_layout::pxp{ 0, arrow_len/2};
   auto const rot_arrow_point = rotate(arrow_point);
   auto const rot_arrow_tail =rotate(display_layout::pxp{0,-arrow_len/2});
   auto const rot_arrow_head_left = rotate(arrow_point + display_layout::size_type{-3,-5});
   auto const rot_arrow_head_right = rotate(arrow_point + display_layout::size_type{3,-5});
   d.line_out(rot_arrow_point, rot_arrow_tail,display_layout::colour::black);
   d.line_out(rot_arrow_point, rot_arrow_head_left,display_layout::colour::black);
   d.line_out(rot_arrow_point, rot_arrow_head_right,display_layout::colour::black);
#else
   int const radius = 50;
   d.circle_out(pos,radius+1,50,display_layout::colour::black);
   d.circle_out(pos,radius,50,display_layout::colour::white);
   d.circle_out(pos,radius-1,50,display_layout::colour::black);

   auto arrow = wxGetApp().get_document()->get_bitmap("arrow");
   if (arrow){
      display_layout::size_type vect = arrow->get_size()/2;
      d.rotated_bitmap_out(pos,arrow,vect,bearing);
   }
#endif
   
}

}//namespace

void view::paint_layout_view(wxPaintEvent & event)
{
     m_display_layout.clear();
//     if( this->have_image()){
//         m_display_layout.bitmap_out({100,100},m_current_image);
//     }
#if 0
     auto f = this->get_current_font();
     if( f != nullptr){
        // wxMessageBox(wxT("GotFont"));
         m_display_layout.text_out({0,0},"HELLO WORLD",f);
     }else{
        //  wxMessageBox(wxT("Not GotFont"));
     }

    // m_display_layout.set_origin( m_display_layout.get_display_size()/2);
     m_display_layout.line_out({-100,0},{0,0},display_layout::colour::white);
    // m_display_layout.line_out({100,100},{50,50},display_layout::colour::white);
     m_display_layout.line_out({0,0},{0,-100},display_layout::colour::black);
     //m_display_layout.line_out({50,-50},{100,-100},display_layout::colour::black);

    // m_display_layout.arc_out({0,0},100,quan::angle::deg{150},quan::angle::deg{30},20,display_layout::colour::black);
    // m_display_layout.arc_out({0,0},99,quan::angle::deg{150},quan::angle::deg{30},20,display_layout::colour::white);
    // m_display_layout.arc_out(98,quan::angle::deg{150},quan::angle::deg{30},20,display_layout::colour::white);
    // m_display_layout.arc_out(97,quan::angle::deg{150},quan::angle::deg{30},20,display_layout::colour::black);
#endif
      draw_compass(m_display_layout,get_bearing());
      
    // auto image_size = m_display_layout.get_display_size();
    // m_display_layout.rescale(image_size*2);

     wxBitmap bitmap{m_display_layout.get_image()};
     wxPaintDC dc(this);
     dc.SetBackground(* wxBLACK_BRUSH); 
     dc.Clear();
     dc.DrawBitmap(bitmap,0,0);
}

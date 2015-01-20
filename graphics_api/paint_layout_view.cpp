
#include <quan/two_d/rotation.hpp>
#include "../view.hpp"
#include "../app.h"

namespace {
void draw_compass(display_layout & d, 
         quan::angle::deg const & bearing,
         quan::angle::deg const & home_bearing) // add ofset and dir
{
 
   quan::two_d::rotation const rotate{bearing};
   
   // bottom of display
   //display_layout::pxp constexpr pos = {0,-188};
   display_layout::pxp constexpr pos = {-240,188};
   int const radius = 36;
   d.circle_out(pos,radius+1,48,display_layout::colour::black);
   for ( int i = 0; i < 16; ++i){
      constexpr auto offset = quan::angle::deg{360/32};
      auto start_angle = quan::angle::deg{360/16} * i + offset;
      auto end_angle = quan::angle::deg{360/16} * (i+1) + offset;
      display_layout::colour col =( i & 1)? display_layout::colour::black : display_layout::colour::white;
      d.arc_out(pos,radius,start_angle,end_angle,3,col);
   }
   d.circle_out(pos,radius-1,48,display_layout::colour::black);
#if 0
   d.circle_out(pos,radius-2,48,display_layout::colour::black);
   //d.circle_out(pos,radius-3,48,display_layout::colour::grey);
   d.flood_fill(pos,display_layout::colour::black);
#endif
   auto f =  wxGetApp().get_document()->get_font("MWOSD");
   if (f){
      display_layout::size_type const char_size{f->get_char_width(),f->get_char_height()};
      display_layout::pos_type const  char_offset = - char_size /2;
      constexpr auto font_radius = 28;
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
   
   auto home_arrow = wxGetApp().get_document()->get_bitmap("copy_1_arrow");
   if (home_arrow){
      display_layout::size_type vect = home_arrow->get_size()/2;
      d.rotated_bitmap_out(pos,home_arrow,vect,home_bearing);
   }

   auto arrow = wxGetApp().get_document()->get_bitmap("arrow");
   if (arrow){
      display_layout::size_type vect = arrow->get_size()/2;
      d.rotated_bitmap_out(pos,arrow,vect,bearing);
   }
}

  
void test_fill(display_layout & d)
{
   typedef display_layout::pxp  pxp;
   display_layout::colour col = display_layout::colour::black;
   int len = 20;
   pxp p0{ -len, len};
   pxp p1{len,len};
   pxp p2{len,-len};
   pxp p3{-len,-len};

   d.line_out(p0,p1,col);
   d.line_out(p1,p2,col);
   d.line_out(p2,p3,col);
   d.line_out(p3,p0,col);

   pxp centre {0,0};
   d.flood_fill(centre,col);
   
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

    // m_display_layout.line_out({-100,0},{0,0},display_layout::colour::white);

    // m_display_layout.line_out({0,0},{0,-100},display_layout::colour::black);

      draw_compass(m_display_layout,get_bearing(),get_home_bearing());

    // test_fill(m_display_layout);
      
//    auto image_size = m_display_layout.get_display_size();
//    m_display_layout.rescale(image_size*4);

     wxBitmap bitmap{m_display_layout.get_image()};
     wxPaintDC dc(this);
     dc.SetBackground(* wxBLACK_BRUSH); 
     dc.Clear();
     dc.DrawBitmap(bitmap,0,0);
}

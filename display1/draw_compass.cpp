#include <quan/two_d/rotation.hpp>
#include "display.hpp"
#include "../view.hpp"
#include "../app.h"

void draw_compass (display_layout & d,
                   quan::angle::deg const & bearing,
                   quan::angle::deg const & home_bearing) // add ofset and dir
{

   quan::two_d::rotation const rotate {bearing};
   
   display_layout::pxp constexpr pos = {0,-188};
   int constexpr radius = 36;
   d.circle1_out (pos, radius + 2,display_layout::colour::black);
   for (int i = 0; i < 16; ++i) {
      constexpr auto offset = quan::angle::deg {360 / 32};
      auto basic_angle = quan::angle::deg { (360 * i) / 16};
      auto start_angle = basic_angle - offset;
      auto end_angle = basic_angle + offset;
      display_layout::colour col 
         = (i & 1) 
         ? display_layout::colour::white 
         : display_layout::colour::black;

      d.arc1_out (pos, radius + 1, start_angle, end_angle,  col);
      d.arc1_out (pos, radius, start_angle, end_angle,  col);
   }
   d.circle1_out (pos, radius - 1,  display_layout::colour::black);

   auto home_arrow = wxGetApp().get_document()->get_bitmap ("home_arrow");
   if (home_arrow) {
      display_layout::size_type vect = home_arrow->get_size() / 2;
      d.rotated_bitmap_out (pos, home_arrow, vect, home_bearing);
   }
      
   auto arrow = wxGetApp().get_document()->get_bitmap ("compass_arrow");
   if (arrow) {
      display_layout::size_type vect = arrow->get_size() / 2;
      d.rotated_bitmap_out (pos, arrow, vect, bearing);
   }
      
   auto f =  wxGetApp().get_document()->get_font ("MWOSD");
   if (f) {
      display_layout::size_type const char_size {f->get_char_width(), f->get_char_height() };
      display_layout::pos_type const  char_offset = - char_size / 2;
      constexpr auto font_radius = 28;
      constexpr char compass_char[] {'N', 'S', 'E', 'W'};
      constexpr display_layout::pos_type compass_vector[] {
         {0, font_radius}
         , {0, - font_radius}
         , {font_radius, 0}
         , { -font_radius, 0}
      };
      for (size_t i = 0; i < 4; ++i) {
         int char_handle = -1;
         f->get_handle_at (compass_char[i], char_handle);
         assert ( (char_handle != -1) && __LINE__);
         auto const char_bmp = wxGetApp().get_document()->get_image (char_handle);
         if (char_bmp) {
            auto char_pos = rotate (compass_vector[i]);
            display_layout::pxp char_pos_i = {
               static_cast<int> (char_pos.x + static_cast<QUAN_FLOAT_TYPE> (0.5)) + char_offset.x + pos.x,
               static_cast<int> (char_pos.y + static_cast<QUAN_FLOAT_TYPE> (0.5)) + char_offset.y + pos.y
            };
            d.bitmap_out (char_pos_i, char_bmp);
         }
      }
   }
}

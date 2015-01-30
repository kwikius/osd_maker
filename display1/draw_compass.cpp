#include <quan/two_d/rotation.hpp>

#include "../osd_bmp_app.hpp"
#include "../gui/view.hpp"

#include "display.hpp"

/*
draw_element( osd_device d, aircraft_params params, resources r)
*/
void draw_compass (osd_device & d,
                   quan::angle::deg const & bearing,
                   quan::angle::deg const & home_bearing) // add ofset and dir
{

   quan::two_d::rotation const rotate {bearing};
   
   osd_device::pxp constexpr pos = {0,-100};
   int constexpr radius = 36;
   d.circle (pos, radius + 2,osd_device::colour::black);
   for (int i = 0; i < 16; ++i) {
      constexpr auto offset = quan::angle::deg {360 / 32};
      auto basic_angle = quan::angle::deg { (360 * i) / 16};
      auto start_angle = basic_angle - offset;
      auto end_angle = basic_angle + offset;
      osd_device::colour col 
         = (i & 1) 
         ? osd_device::colour::white 
         : osd_device::colour::black;

      d.arc (pos, radius + 1, start_angle, end_angle,  col);
      d.arc (pos, radius, start_angle, end_angle,  col);
   }
   d.circle (pos, radius - 1,  osd_device::colour::black);

   auto home_arrow = wxGetApp().get_document()->get_bitmap ("home_arrow");
   if (home_arrow) {
      osd_device::size_type vect = home_arrow->get_size() / 2;
      d.rotated_bitmap_out (pos, home_arrow, vect, home_bearing);
   }
      
   auto arrow = wxGetApp().get_document()->get_bitmap ("compass_arrow");
   if (arrow) {
      osd_device::size_type vect = arrow->get_size() / 2;
      d.rotated_bitmap_out (pos, arrow, vect, bearing);
   }
      
   auto f =  wxGetApp().get_document()->get_font ("MWOSD");
   if (f) {
      osd_device::size_type const char_size {f->get_char_width(), f->get_char_height() };
      osd_device::pos_type const  char_offset = - char_size / 2;
      constexpr auto font_radius = 28;
      constexpr char compass_char[] {'N', 'S', 'E', 'W'};
      constexpr osd_device::pos_type compass_vector[] {
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
            osd_device::pxp char_pos_i = {
               static_cast<int> (char_pos.x + static_cast<QUAN_FLOAT_TYPE> (0.5)) + char_offset.x + pos.x,
               static_cast<int> (char_pos.y + static_cast<QUAN_FLOAT_TYPE> (0.5)) + char_offset.y + pos.y
            };
            d.bitmap (char_pos_i, char_bmp);
         }
      }
   }
}

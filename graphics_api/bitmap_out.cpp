
#include "../display_layout.hpp"

void display_layout::bitmap_out(pxp const & pos_in, osd_image* image)
{
   if ( !image){
      return ;
   }
   auto size_px = image->get_size();
   pxp raw_pos_out = transform_to_raw({pos_in.x,pos_in.y + size_px.y});
   int const raw_x_start = raw_pos_out.x;
   for ( uint32_t y = 0U; y < size_px.y; ++y, ++raw_pos_out.y){
      raw_pos_out.x = raw_x_start;
      for ( uint32_t x = 0U; x < size_px.x; ++x, ++ raw_pos_out.x){
          colour c = osd_image::colour::transparent;
          image->get_pixel_colour({x,y},c);
       // wont overwrite stuff
         // may need an option
          if ( c != osd_image::colour::transparent){
            set_pixel(raw_pos_out,c); 
          }
      }
      
   }
}
#include <quan/two_d/rotation.hpp>
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

void display_layout::rotated_bitmap_out(pxp const & pos, osd_image* image, pxp const & rotation_centre,  quan::angle::deg const & angle)
{
    if ( !image){
      return ;
   }
   auto size_px = image->get_size();
   quan::two_d::rotation rotate(angle);
   for ( uint32_t y = 0U; y < size_px.y; ++y){
      for ( uint32_t x = 0U; x < size_px.x; ++x ){
          colour c = osd_image::colour::transparent;
          pxp raw_px = {x, (size_px.y - 1)- y};
          image->get_pixel_colour(raw_px,c);
          if ( c != osd_image::colour::transparent){
             pxp rvect = pxp{x,y} - rotation_centre;
             pxp raw_out_pos = transform_to_raw(rotate(rvect) + pos);
             set_pixel(raw_out_pos,c); 
          }
      }
   }
  
}
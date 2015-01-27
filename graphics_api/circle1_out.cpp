#include "../display_layout.hpp"
//from web.engr.oregonstate.edu/~sllu/bcircle.pdf

namespace {

   void plot8pts(display_layout& d, display_layout::pxp const & centre, int x, int y,display_layout::colour c)
   {
      typedef display_layout::pxp pxp;
      d.set_pixel(centre + pxp{x,y},c);
      d.set_pixel(centre + pxp{-x,y},c);
      d.set_pixel(centre + pxp{-x,-y},c);
      d.set_pixel(centre + pxp{x,-y},c);
      d.set_pixel(centre + pxp{y,x},c);
      d.set_pixel(centre + pxp{-y,x},c);
      d.set_pixel(centre + pxp{-y,-x},c);
      d.set_pixel(centre + pxp{y,-x},c);
   }
}
void display_layout::circle1_out(pxp const & centre, int radius, colour c)
{
  int x = radius;
  int y =0;
  int re = 1-x;
  while ( x >= y){
     plot8pts(*this,centre,x,y,c);
     ++y;
     if ( re < 0){
        re += 2 * (y + 1);
     }else{
        --x;
        re += 2 * (y - x + 1);
     }
  }
}
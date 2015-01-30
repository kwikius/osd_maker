#include <cstdio>

#include "../osd_bmp_app.hpp"
#include "../aircraft/aircraft.hpp"
#include "../gui/view.hpp"

#include "display.hpp"
#include "constants.hpp"

void draw_altitude(osd_device & d)
{
   auto const & aircraft_position = the_aircraft.get_position();
   auto const & home_position = the_aircraft.get_home_position();
#if 0
   quan::length::m  alt = aircraft_position.alt - home_position.alt; // sort diffence for angle
#else
   quan::length::m  alt{100};
#endif
   auto image = wxGetApp().get_document()->get_bitmap ("altitude_symbol");
   typedef osd_device::pxp pxp;
   osd_device::size_type vect;
   pxp pos{-160,100}; 
   if (image) {
          vect = image->get_size() / 2;
         d.bitmap (pos,image);
   }
  
   auto alt_m = static_cast<uint32_t> (alt.numeric_value() + 0.5);

   char buf[30];
   sprintf( buf,"%5um",alt_m);
   auto f = wxGetApp().get_view()->get_current_font();
   if ( f){
      d.text(pos + pxp{vect.x,0},buf,f);
   }

}
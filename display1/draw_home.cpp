#include <cstdio>

#include "../osd_bmp_app.hpp"
#include "../gui/view.hpp"
#include "../aircraft/aircraft.hpp"

#include "display.hpp"
#include "constants.hpp"

void draw_home(osd_device & d)
{
   auto const & aircraft_position = the_aircraft.get_position();
   auto const & home_position = the_aircraft.get_home_position();
   quan::angle_<double>::deg  dlat = aircraft_position.lat - home_position.lat; 
   quan::angle_<double>::deg  dlon = aircraft_position.lon - home_position.lon;
   quan::length_<double>::m rlat = cos(quan::angle_<double>::rad{abs(home_position.lat)}) * radius_of_world();
   quan::length_<double>::m dx = (dlon * rlat) / quan::angle_<double>::rad{1};
   quan::length_<double>::m dy =  (dlat * radius_of_world() ) / quan::angle_<double>::rad{1};
 #if 0
   quan::length_<double>::m distance = quan::sqrt(dx * dx + dy * dy);
#else
  quan::length_<double>::m distance{235};
#endif
   auto d_m = static_cast<uint32_t> (distance.numeric_value() + 0.5);
   auto image = wxGetApp().get_document()->get_bitmap ("home_image");
   typedef osd_device::pxp pxp;
   osd_device::size_type vect;
 //  pxp pos{-263,200}; 
    pxp pos{-160,70};
   if (image) {
          vect = image->get_size() / 2;
         d.bitmap (pos,image);
   }
   char buf[30];
   sprintf( buf,"%5u M",d_m);
   auto f = wxGetApp().get_view()->get_current_font();
   if ( f){
      d.text(pos + pxp{vect.x,0},buf,f);
   }
}
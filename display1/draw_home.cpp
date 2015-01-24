#include "display.hpp"
#include "../aircraft/aircraft.hpp"
#include "../constants.hpp"
#include <cstdio>
#include "../app.h"
#include "../view.hpp"

void draw_home(display_layout & d)
{
   auto const & aircraft_position = the_aircraft.get_position();
   auto const & home_position = the_aircraft.get_home_position();
   quan::angle_<double>::deg  dlat = aircraft_position.lat - home_position.lat; // sort diffence for angle
   quan::angle_<double>::deg  dlon = aircraft_position.lon - home_position.lon;

  // static_assert(std::is_same<decltype(dlat),quan::angle::deg>::value , "odd");
   quan::length_<double>::m  rlat = cos(quan::angle_<double>::rad{abs(home_position.lat)}) * radius_of_world();
   quan::length_<double>::m dx = (dlon * rlat) / quan::angle_<double>::rad{1};
   quan::length_<double>::m dy =  (dlat * radius_of_world() ) / quan::angle_<double>::rad{1};
   quan::length_<double>::m distance = quan::sqrt(dx * dx + dy * dy);
   auto d_m = static_cast<uint32_t> (distance.numeric_value() + 0.5);
   // draw a bitmap representing home
   // and put the distance to home
   // retrieve wanted units
   char buf[30];
   int ret = sprintf( buf,"home:%5u m",d_m);
   auto f = wxGetApp().get_view()->get_current_font();
   if ( f){
      d.text_out({-290,210},buf,f);
   }
}
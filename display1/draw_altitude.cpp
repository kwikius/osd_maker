#include "display.hpp"
#include "../aircraft/aircraft.hpp"
#include "../constants.hpp"
#include <cstdio>
#include "../app.h"
#include "../view.hpp"

void draw_altitude(display_layout & d)
{
   
   auto const & aircraft_position = the_aircraft.get_position();
   auto const & home_position = the_aircraft.get_home_position();
   quan::length::m  alt = aircraft_position.alt - home_position.alt; // sort diffence for angle
  
   auto alt_m = static_cast<uint32_t> (alt.numeric_value() + 0.5);
   // draw a bitmap representing home
   // and put the distance to home
   // retrieve wanted units
   char buf[30];
   int ret = sprintf( buf," ALT:%5u M",alt_m);
   auto f = wxGetApp().get_view()->get_current_font();
   if ( f){
      d.text_out({-290,190},buf,f);
   }

}
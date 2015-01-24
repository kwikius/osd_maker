#ifndef OSD_BITMAP_MAKER_DISPLAY1_DISPLAY_HPP_INCLUDED
#define OSD_BITMAP_MAKER_DISPLAY1_DISPLAY_HPP_INCLUDED

#include "../display_layout.hpp"
#include <quan/angle.hpp>
#include "../aircraft/aircraft.hpp"

void draw_compass(display_layout & d, 
         quan::angle::deg const & bearing,
         quan::angle::deg const & home_bearing);

void draw_artificial_horizon( display_layout & d, attitude_t const & attitude);
void draw_home(display_layout & d);
void draw_altitude(display_layout & d);

#endif

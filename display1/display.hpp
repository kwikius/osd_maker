#ifndef OSD_BITMAP_MAKER_DISPLAY1_DISPLAY_HPP_INCLUDED
#define OSD_BITMAP_MAKER_DISPLAY1_DISPLAY_HPP_INCLUDED

#include <quan/angle.hpp>

#include "../graphics_api/osd_device.hpp"
#include "../aircraft/aircraft.hpp"

void draw_compass(osd_device & d, 
         quan::angle::deg const & bearing,
         quan::angle::deg const & home_bearing);

void draw_artificial_horizon( osd_device & d, attitude_t const & attitude);
void draw_home(osd_device & d);
void draw_altitude(osd_device & d);

#endif

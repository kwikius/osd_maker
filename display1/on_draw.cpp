
#include "display.hpp"

void draw_compass(osd_device & d, 
         quan::angle::deg const & bearing,
         quan::angle::deg const & home_bearing);

void draw_artificial_horizon( osd_device & d, attitude_t const & attitude);
void draw_home(osd_device & d);
void draw_altitude(osd_device & d);

void display1_on_draw( osd_device & d,aircraft const & a, osd_object_database const &db)
{
     draw_compass(d,quan::angle::deg{20},quan::angle::deg{300});
     draw_artificial_horizon(d,the_aircraft.get_attitude());
     draw_home(d);
     draw_altitude(d);
}
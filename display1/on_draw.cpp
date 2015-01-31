
#include "display.hpp"

void draw_compass(osd_device & d);
void draw_artificial_horizon( osd_device & d);
void draw_home(osd_device & d);
void draw_altitude(osd_device & d);

void display1_on_draw( osd_device & d,aircraft const & a, osd_object_database const &db)
{
     draw_compass(d);
     draw_artificial_horizon(d);
     draw_home(d);
     draw_altitude(d);
}
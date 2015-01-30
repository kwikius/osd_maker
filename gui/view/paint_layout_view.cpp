
#include "../view.hpp"
#include "../../display1/display.hpp"
#include "../../aircraft/aircraft.hpp"

void view::paint_layout_view(wxPaintEvent & event)
{
     m_osd_device.clear();

     draw_compass(m_osd_device,get_bearing(),get_home_bearing());
     draw_artificial_horizon(m_osd_device,the_aircraft.get_attitude());
     draw_home(m_osd_device);
     draw_altitude( m_osd_device);
#if 0
    m_osd_device.arc({0,0},100,quan::angle::deg{270}, 
         quan::angle::deg{20},osd_device::colour::black);
#endif
     wxBitmap bitmap{m_osd_device.get_image()};
     wxPaintDC dc(this);
     dc.SetBackground(* wxBLACK_BRUSH); 
     dc.Clear();
     dc.DrawBitmap(bitmap,0,0);
}


#include "../view.hpp"
#include "../display1/display.hpp"
#include "../aircraft/aircraft.hpp"

void view::paint_layout_view(wxPaintEvent & event)
{
     m_display_layout.clear();

     draw_compass(m_display_layout,get_bearing(),get_home_bearing());
     draw_artificial_horizon(m_display_layout,the_aircraft.get_attitude());
     draw_home(m_display_layout);
     draw_altitude( m_display_layout);
#if 0
    m_display_layout.arc1_out({0,0},100,quan::angle::deg{270}, 
         quan::angle::deg{20},display_layout::colour::black);
#endif
     wxBitmap bitmap{m_display_layout.get_image()};
     wxPaintDC dc(this);
     dc.SetBackground(* wxBLACK_BRUSH); 
     dc.Clear();
     dc.DrawBitmap(bitmap,0,0);
}

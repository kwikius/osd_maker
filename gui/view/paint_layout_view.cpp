
#include "../../document.hpp"
#include "../view.hpp"
#include "../../display1/display.hpp"
#include "../../aircraft/aircraft.hpp"

void view::paint_layout_view(wxPaintEvent & event)
{
#if 1
    m_osd_device.clear();
     // call pf(m_osd_device,the_aircraft_params, resources);
    display1_on_draw(
         m_osd_device,
         the_aircraft, wxGetApp().get_document()->get_database());
#else
     


#endif     

     wxBitmap bitmap{m_osd_device.get_image()};
     wxPaintDC dc(this);
     dc.SetBackground(* wxBLACK_BRUSH); 
     dc.Clear();
     dc.DrawBitmap(bitmap,0,0);

}


#include "../../document.hpp"
#include "../view.hpp"
#include "../../display1/display.hpp"
#include "../../aircraft/aircraft.hpp"

void view::paint_layout_view(wxPaintEvent & event)
{

    m_osd_device.clear();

    if ( (this->m_pfn_set_osd_on_draw_params != nullptr)   
        && ( this->m_pfn_osd_on_draw != nullptr)){

         m_pfn_set_osd_on_draw_params(
            &wxGetApp().get_document()->get_database(),&the_aircraft
         );
         m_pfn_osd_on_draw(m_osd_device);
    }
    
     wxBitmap bitmap{m_osd_device.get_image()};
     wxPaintDC dc(this);
     dc.SetBackground(* wxBLACK_BRUSH); 
     dc.Clear();
     dc.DrawBitmap(bitmap,0,0);

}

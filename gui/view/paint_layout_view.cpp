
#include "../../document.hpp"
#include "../view.hpp"
#include "../../display1/display.hpp"

#include <quan/uav/osd/dynamic/object_database.hpp>

/*
operating systems
   __LINUX__
  __WINDOWS__
*/


namespace {
//wxString dll_path = wxT("C:/cpp/projects/my_dll/bin/Debug/my_dll");
wxString dll_path = wxT("C:/cpp/lib/quantracker_lib/examples/osd_example1/pc_sim/osd_draw");
}
void view::paint_layout_view(wxPaintEvent & event)
{


    m_osd_device.clear();


#if 0
    if ( (this->m_pfn_set_osd_on_draw_params != nullptr)
        && ( this->m_pfn_osd_on_draw != nullptr)){

         m_pfn_set_osd_on_draw_params(
            &wxGetApp().get_document()->get_database()
         );
         m_pfn_osd_on_draw(m_osd_device);

    }

    #else
//static int count = 0;
   // if ( count ==0){
              //  ++count;
     wxDynamicLibrary dll;
     if ( dll.Load(dll_path) ){
        bool dll_good = dll.HasSymbol(wxT("osd_on_draw")) && dll.HasSymbol(wxT("set_osd_on_draw_params"));
        if ( dll_good){
             void(*pfn_par)(quan::uav::osd::dynamic::object_database const * db);
             void(*pfn)( quan::uav::osd::dynamic::display_device * d) = nullptr;
             *(void**) (&pfn_par) = dll.GetSymbol(wxT("set_osd_on_draw_params"));
             *(void**) (&pfn) = dll.GetSymbol(wxT("osd_on_draw"));
            //auto const & db = wxGetApp().get_view()-;
            if ( pfn_par && pfn){
                auto const & db = wxGetApp().get_document()->get_database();
                pfn_par(&db);
                pfn(&m_osd_device);
            }
        }
     }
    #endif

     wxBitmap bitmap{m_osd_device.get_bitmap()};
     wxPaintDC dc(this);
     dc.SetBackground(* wxBLACK_BRUSH);
     dc.Clear();
     dc.DrawBitmap(bitmap,0,0);
    //}

}

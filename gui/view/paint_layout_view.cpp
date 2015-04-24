
#include "../../document.hpp"
#include "../view.hpp"
#include "../../display1/display.hpp"

#include <quan/uav/osd/dynamic/object_database.hpp>
#include <wx/filefn.h>
#include <wx/stdpaths.h>

void view::paint_layout_view(wxPaintEvent & event)
{
    m_osd_device.clear();

    static bool failed = false;
    if ( m_layout_filename != wxT("") && ! failed){
      wxString dll_path =  wxGetApp().get_app_dir() 
         + wxT("/resources/layouts_checkout/linux/") + m_layout_filename;

      wxDynamicLibrary dll;
      if ( dll.Load(dll_path) ){
         bool dll_good = dll.HasSymbol(wxT("osd_on_draw")) && dll.HasSymbol(wxT("set_osd_on_draw_params"));
         if ( dll_good){
            void(*pfn_par)(quan::uav::osd::dynamic::object_database const * db);
            void(*pfn)( quan::uav::osd::dynamic::display_device * d) = nullptr;
            *(void**) (&pfn_par) = dll.GetSymbol(wxT("set_osd_on_draw_params"));
            *(void**) (&pfn) = dll.GetSymbol(wxT("osd_on_draw"));

            if ( pfn_par && pfn){
               auto const & db = wxGetApp().get_document()->get_database();
               pfn_par(&db);
               pfn(&m_osd_device);
            }else {
               failed = true;
            }
         }else{
            failed = true;
         }
      }else{
         failed = true;
      }
   }
   wxBitmap bitmap{m_osd_device.get_bitmap()};
   wxPaintDC dc(this);
   dc.SetBackground(* wxBLACK_BRUSH);
   dc.Clear();
   dc.DrawBitmap(bitmap,0,0);
}

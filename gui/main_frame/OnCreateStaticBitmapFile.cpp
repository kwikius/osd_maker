


#include <quan/gx/wxwidgets/from_wxString.hpp>
#include <quan/gx/wxwidgets/to_wxString.hpp>
#include <quan/uav/osd/detail/writable_bitmap.hpp>
#include <fstream>
#include "../../document.hpp"
#include "../../osd_bmp_app.hpp"
#include "../panel.hpp"
#include "../main_frame.hpp"
#include <wx/choicdlg.h>

using quan::gx::wxwidgets::from_wxString;
using quan::gx::wxwidgets::to_wxString;

void main_frame::OnCreateStaticBitmapFile(wxCommandEvent & event)
{
   wxArrayString choices = wxGetApp().get_panel()->get_bitmap_names();
   
   wxMultiChoiceDialog dlg{
      this
      ,wxT("Choose bitmaps")
      ,wxT("Bitmaps to headers")
      ,choices
   };
   if (dlg.ShowModal() == wxID_OK){

     // do FileDialog  put all in one file

      std::string dir = "/home/andy/cpp/projects/test/dummy/";

      /// Open one file here and put all in it
      /*
         a cpp file
         #include <quan/uav/osd/api.hpp>
         these definitions
            quan::uav::osd::bitmap_ptr quan::uav::osd::get_bitmap(uint32_t id);
            quan::uav::osd::pxp_type quan::uav::osd::get_size( bitmap_ptr p);
      */
      wxArrayInt result = dlg.GetSelections();
      for ( size_t i = 0; i < result.GetCount();++i){
         wxString bmp_name = choices[result[i]];
         // get the bitmap of that name
         typedef quan::uav::osd::dynamic::bitmap dynamic_bitmap;
         dynamic_bitmap* bmp = wxGetApp().get_document()->get_bitmap(
            from_wxString<char>(bmp_name)
         );
         if ( bmp ){
            std::string bmp_name1 = from_wxString<char>(bmp_name);
            std::ofstream out (dir + bmp_name1 + ".hpp");
            bmp->output_header( bmp_name1 + "_type", bmp_name1,out);
         }
      }  
   }
}

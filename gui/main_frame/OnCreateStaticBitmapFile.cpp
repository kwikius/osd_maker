


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
     std::string dirname = "/home/andy/cpp/projects/quantracker/examples/osd_example1/";

     // todo : FileDialog  put all in one file
      std::string src_filename = dirname + "board/bitmaps.cpp";

      wxArrayInt result = dlg.GetSelections();
      // check result ok
      std::ofstream out (src_filename);

      out << "//Generated by OSDMaker\n\n";
      out << "#include <quan/uav/osd/api.hpp>\n";
      out << "#include <quan/uav/osd/basic_bitmap.hpp>\n\n";
      out << "namespace {\n\n";

      // check out ok
      for ( size_t i = 0; i < result.GetCount();++i){
         wxString bmp_name = choices[result[i]];
         // get the bitmap of that name
         typedef quan::uav::osd::dynamic::bitmap dynamic_bitmap;
         dynamic_bitmap* bmp = wxGetApp().get_document()->get_bitmap(
            from_wxString<char>(bmp_name)
         );
         if ( bmp ){
            std::string bmp_name1 = from_wxString<char>(bmp_name);
            bmp->output_header( bmp_name1 + "_type", bmp_name1,out);
         }
      } 

      out << " constexpr quan::uav::osd::bitmap_ptr bitmap_array[] =\n";
      out << "   {\n";
      for ( size_t i = 0; i < result.GetCount();++i){
         std::string bmp_name = from_wxString<char>(choices[result[i]]);
         out << "    ";
         if ( i != 0){
            out << ",";
         }
         out << "&" << bmp_name  <<'\n';
      }
      out << "   };\n} // namespace\n\n";
      out << "quan::uav::osd::bitmap_ptr\n";
      out << "quan::uav::osd::get_bitmap(uint32_t id)\n";
      out << "{\n";
      out << "constexpr uint32_t size = sizeof(bitmap_array)/sizeof(quan::uav::osd::bitmap_ptr);\n";
      out << "  if ( id < size){\n";
      out << "     return bitmap_array[id];\n";
      out << "  }else{\n";
      out << "     return nullptr;\n";
      out << "  }\n";
      out << "}\n";

      out.close();
      // source done

      std::string hdr_filename = dirname + "board/bitmap.hpp";
      std::ofstream out1 (hdr_filename);
      // check ok
      
      out1 << "#ifndef USER_BITMAP_HPP_INCLUDED\n";
      out1 << "#define USER_BITMAP_HPP_INCLUDED\n\n";
      out1 << "//Generated by OSDMaker\n\n";
      out1 << "#include <quan/uav/osd/basic_bitmap.hpp>\n\n";
      out1 <<  " struct BitmapID{\n";

      for ( size_t i = 0; i < result.GetCount();++i){
         std::string bmp_name = from_wxString<char>(choices[result[i]]);
         out1 << "static constexpr uint32_t " << bmp_name << " = " << i << ";\n";
      }
      out1 << "};\n";
      out1  << "\n";
      out1 << "#endif\n";
   }
}



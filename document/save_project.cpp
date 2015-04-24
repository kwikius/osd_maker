#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <wx/zipstrm.h>
#include <wx/wfstream.h>
#include <wx/filedlg.h>
#include <wx/textfile.h>
#include <fstream>

#include <quan/fs/get_basename.hpp>
#include <quan/fs/strip_file_extension.hpp>

#include <quan/gx/wxwidgets/from_wxString.hpp>
#include <quan/gx/wxwidgets/to_wxString.hpp>

#include "../osd_bmp_app.hpp"
#include "../document.hpp"
#include "../document/osd_object_database.hpp"
#include "../gui/main_frame.hpp"
#include "../gui/view.hpp"
#include "../gui/panel.hpp"

#include "../graphics_api/objects/db_bitmap.hpp"

using quan::gx::wxwidgets::from_wxString;
using quan::gx::wxwidgets::to_wxString;

bool document::save_project()
{
   wxGetApp().get_view()->sync_hmi_view();
   
   wxString save_path = get_project_file_path() ;
   if (save_path == wxT ("")) {
         // get current path and name
         
         wxString dirname =  wxGetApp().get_config()->Read (wxT ("/CurrentProject/FileDir"), wxT (""));
         wxString filename =  wxGetApp().get_config()->Read (wxT ("/CurrentProject/FileName"), wxT (""));
         
         wxFileDialog fd {wxGetApp().get_main_frame(),
                          wxT ("Save Project"),
                          dirname,                    // default dir
                          filename,                    // default file
                          wxT ("osd lib files(*.zip)|*.zip"), // wildcard
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT
                         };
         if (fd.ShowModal() == wxID_CANCEL) {
               return false;
            }
         save_path = fd.GetPath();
      }
   return ll_save_project (save_path);
}

bool document::save_project_as (wxString const & path)
{

   if (ll_save_project (path)) {
         // TODO set project name, tree name etc
         this->m_project_file_path = path;
         wxGetApp().get_main_frame()->SetTitle (path);
         
         std::string str_path = from_wxString<char> (path);
         std::string basename =  quan::fs::get_basename (str_path);
         std::string dirname =  str_path.substr (0, str_path.rfind ('/'));
         
         wxGetApp().get_config()->Write (wxT ("/CurrentProject/FileDir"), to_wxString (dirname));
         wxGetApp().get_config()->Write (wxT ("/CurrentProject/FileName"), to_wxString (basename));
         
         std::string std_path
         = quan::fs::strip_file_extension (
              quan::fs::get_basename (from_wxString<char> (path))
           );
         wxGetApp().get_panel()->set_project_name (to_wxString (std_path));
         return true;
      }
   else {
         return false;
      }
}

bool document::ll_save_project (wxString const & path)
{
   wxFileOutputStream out {path};
   if (! out.IsOk()) {
         wxMessageBox (wxT ("Output file failed\n"));
         return false;
      }
   else {
         wxZipOutputStream zipout {out};
         zipout.PutNextDirEntry (wxT ("fonts"));
         for (size_t i = 0; i < m_database->get_num_fonts(); ++i) {
               int font_handle = -1;
               assert (m_database->get_font_handle_at (i, font_handle) && __LINE__) ;
               db_font * cur_font = get_font (font_handle);
               assert (cur_font != nullptr && __LINE__);
               wxString dirname = wxT ("fonts/");
               dirname += to_wxString (cur_font->get_name());
               zipout.PutNextDirEntry (dirname);
               for (int32_t j = cur_font->get_begin();
                     j < (cur_font->get_num_elements() + cur_font->get_begin());
                     ++j) {
                     int image_handle = -1;
                     assert (cur_font->get_handle_at (j, image_handle) && __LINE__);
                     assert ( (image_handle != -1) && __LINE__);
                     dynamic_bitmap * bmp = get_bitmap (image_handle);
                     assert (bmp && __LINE__);
                     wxString filename = dirname + wxT ("/");
                     filename += wxString::Format (wxT ("char%d.png"), j);
                     zipout.PutNextEntry (filename);
                     wxImage* wx_image = ConvertTo_wxImage (*bmp);
                     wx_image->SaveFile (zipout, wxBITMAP_TYPE_PNG);
                  }
            }
         zipout.PutNextDirEntry (wxT ("bitmaps"));
         for (size_t i = 0; i < m_database->get_num_bitmaps(); ++i) {
               int handle = -1;
               assert (m_database->get_bitmap_handle_at (i, handle) && __LINE__);
               dynamic_bitmap * bmp = m_database->find_bitmap_by_handle (handle);
               assert (bmp && __LINE__);
               wxString name = wxT ("bitmaps/");
               name += to_wxString (bmp->get_name());
               name += wxT (".png");
               zipout.PutNextEntry (name);
               wxImage* wx_image = ConvertTo_wxImage (*bmp);
               wx_image->SaveFile (zipout, wxBITMAP_TYPE_PNG);
            }
         zipout.PutNextDirEntry (wxT ("layouts"));
         wxArrayString layout_names = wxGetApp().get_panel()->get_layout_names();
         
         wxString dll_dir = wxGetApp().get_app_dir() + wxT ("/resources/layouts_checkout/");
      for (wxString name : layout_names) {
               // wxMessageBox(name);
               auto pos = name.rfind (wxT (".so"));
               if (pos == name.length() - 3) {
// if on LinuxOS
                     wxString dll_path = dll_dir + wxT ("linux/") + name;
//else on windows
//endif
                     wxFileInputStream dll_in (dll_path);
                     zipout.PutNextEntry (wxT ("layouts/") + name);
                     zipout.Write (dll_in);
                  }
            }
      }
   this->set_modified (false);
   return true;
}
 
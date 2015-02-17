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
#include "osd_object_database.hpp"
#include "../gui/main_frame.hpp"
#include "../gui/view.hpp"
#include "../gui/panel.hpp"

#include "../graphics_api/objects/db_bitmap.hpp"


using quan::gx::wxwidgets::from_wxString;
using quan::gx::wxwidgets::to_wxString;

bool document::open_project (wxString const & path)
{
   if ( this->is_modified()){
      if ( wxMessageBox(wxT("Do you want to save current Project?"),
                            wxT("Confirm Current Project Save"),
                            wxICON_QUESTION | wxYES_NO ) == wxYES ) {
          this->save_project();
       }
   }
  // TODO if have project open
  // ask to save
   if (! wxImage::FindHandler (wxBITMAP_TYPE_PNG)) {
         wxImage::AddHandler (new wxPNGHandler);
   }
   wxFileInputStream in(path);
   if (!in.IsOk()){
         wxMessageBox (wxT ("Input file failed\n"));
         return false;
   }
   wxZipInputStream zipin(in);
   if (!zipin.IsOk()){
         wxMessageBox (wxT ("Create zip input stream failed\n"));
         return false;
   }
   int num_entries = zipin.GetTotalEntries();

   auto temp_resources = new osd_object_database;
   for ( int i = 0; i < num_entries; ++i){
      wxZipEntry* entry = zipin.GetNextEntry();
      assert(entry && __LINE__);
      if ( !entry->IsDir()){
         std::string full_name = from_wxString<char>(entry->GetName());
         if ( ( full_name.find("bitmaps\\") != std::string::npos) &&
            (full_name.find(".png",full_name.length()-4,4) != std::string::npos)
         ){

             // rotating_compass\\bitmaps\\bitmap_name.png
          size_t const bitmaps_pos = full_name.find("bitmaps\\");
          std::string const name_png = full_name.substr(bitmaps_pos + 8,std::string::npos);
          std::string const name = name_png.substr(0,name_png.length()-4);
          //############
          if (temp_resources->find_bitmap_by_name(name) != nullptr){
            wxMessageBox(wxT("Invalid file, multiple bitmaps of same name"));
            delete temp_resources;
            delete entry;
            return false;
          }
          wxImage image(zipin,wxBITMAP_TYPE_PNG);
          dynamic_bitmap * bmp = ConvertTo_osd_bitmap(name,image);
          temp_resources->add_bitmap(bmp);
         } else{

             if ( ( full_name.find("fonts\\",0,6) != std::string::npos) &&
            (full_name.find(".png",full_name.length()-4,4) != std::string::npos)
             ){
               size_t dirsep1 = full_name.find("fonts\\");
               if (dirsep1 == std::string::npos){
                  wxMessageBox(wxT("Invalid file"));
                  //TODO clean up
                  return false;
               }
               full_name = full_name.substr(dirsep1,std::string::npos);
               size_t dirsep = full_name.find_first_of('\\',6);
               if (dirsep == std::string::npos){
                  wxMessageBox(wxT("Invalid file"));
                  //TODO clean up
                  return false;
               }
               std::string font_name = full_name.substr(6,dirsep-6);
               db_font * home_font = temp_resources->find_font_by_name(font_name);
               bool new_font = false;
               if ( home_font == nullptr){
                 // need to sort size
                 home_font = new db_font{font_name,dynamic_bitmap::size_type{0,0},0};
                 new_font = true;
                 temp_resources->add_font(home_font);
               }
               std::string font_element_index_str = full_name.substr(dirsep +5,full_name.length() - (dirsep + 9));

               for ( auto c : font_element_index_str){
                  if ( !::isdigit(c)){
                         char buf[2];
                        buf[0]= c;
                        buf[1] = '\0';
                        std::string std_dig = buf;
                        wxString dig = to_wxString(std_dig);
                        wxString wxfull_name = to_wxString(full_name);
                        wxMessageBox(wxT("Invalid font char file. string = ") + wxfull_name + wxT(" char = \'") + dig + wxT("\'"));
                        //TODO clean up
                        return false;
                  }
               }
               int font_element_pos = ::atoi(font_element_index_str.c_str());
               // TODO check not too big
               char font_element_name[] = {'\'', static_cast<char>(font_element_pos),'\'','\0'};
               wxImage font_elem_image(zipin,wxBITMAP_TYPE_PNG);


               dynamic_bitmap * elem_bmp = ConvertTo_osd_bitmap(font_element_name,font_elem_image);
               int font_elem_handle = temp_resources->add_font_element(elem_bmp);
               home_font->set_handle_at(font_element_pos, font_elem_handle);
               if ( new_font){
                  home_font->set_char_size(elem_bmp->get_size());
                  new_font = false;
               }
               //TODO else check that all elements are same size
             }

         }
      }
      delete entry;
   }


   if ( temp_resources->get_num_bitmaps() == 0){

    wxMessageBox(wxT("no bitmaps in zip"));
     return false;
   }

    if ( temp_resources->get_num_fonts() == 0){

    wxMessageBox(wxT("no fonts in zip"));
     return false;
   }

   wxGetApp().get_main_frame()->clear();
   for ( size_t i =0, end = temp_resources->get_num_bitmaps(); i < end; ++i){
      int handle = -1;
      assert(temp_resources->get_bitmap_handle_at(i,handle) && __LINE__);
      auto bmp = temp_resources->move_bitmap_by_handle(handle);
      assert(bmp);
      this->add_bitmap(bmp);
   }
   for ( size_t i = 0; i < temp_resources->get_num_fonts(); ++i){
      int temp_font_handle = -1;
      assert ( temp_resources->get_font_handle_at(i,temp_font_handle) && __LINE__);
      assert (( temp_font_handle != -1) && __LINE__);
      db_font* new_font = temp_resources->move_font_by_handle(temp_font_handle);
      assert ((new_font != nullptr) && __LINE__);
      for ( size_t j = new_font->get_begin();
            j < (new_font->get_num_elements() + static_cast<size_t>(new_font->get_begin()));
            ++j){
         int temp_image_handle = -1;
         assert(new_font->get_handle_at( j, temp_image_handle) && __LINE__);
         dynamic_bitmap* font_elem
            = temp_resources->move_font_element_by_handle(temp_image_handle);
         assert (( font_elem != nullptr && __LINE__));
         int elem_handle = m_database->add_font_element(font_elem);
         new_font->set_handle_at(j, elem_handle);
      }
      int new_font_handle = this->m_database->add_font(new_font);
      wxGetApp().get_panel()->add_font_handle(new_font->get_name(),new_font_handle);

   }
   // TODO delete temp_resources;

   this->m_project_file_path = path;
   wxGetApp().get_main_frame()->SetTitle(path);
   std::string std_path
      = quan::fs::strip_file_extension(
            quan::fs::get_basename(from_wxString<char>(path))
      );

   wxGetApp().get_panel()->set_project_name(to_wxString(std_path));
   return true;
}

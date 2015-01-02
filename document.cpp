#include <cstdio>
#include <wx/zipstrm.h>
#include <wx/wfstream.h>
#include <wx/filedlg.h>

#include <quan/fs/get_basename.hpp>
#include <quan/fs/strip_file_extension.hpp>
#include <quan/gx/wxwidgets/from_wxString.hpp>
#include <quan/gx/wxwidgets/to_wxString.hpp>

#include "app.h"
#include "document.hpp"
#include "main_frame.h"
#include "view.hpp"
#include "panel.hpp"

using quan::gx::wxwidgets::from_wxString;
using quan::gx::wxwidgets::to_wxString;

font * bitmap_resource_t::get_font_at(size_t i)
{
   assert( (i < get_num_fonts()) && __LINE__);
   return m_fonts.at(i);
}
osd_bitmap* bitmap_resource_t::get_bitmap_at(size_t i)
{
   assert( (i < get_num_bitmaps()) && __LINE__);
   return m_bitmaps.at(i);
}

std::string bitmap_resource_t::make_unique_image_name(std::string const & name_in)const
{
   std::string name_out = name_in;
   int val = 1;
   for(;;){
      bool name_unique = true;
      // search through bitmaps looking for name
      for( auto v : m_osd_image_map){
         osd_image* image =  v.second;
         auto bmp = dynamic_cast<osd_bitmap*>( image);
         if(bmp && (name_out == bmp->get_name())){
            name_unique = false;
            // modify the output name by prepending "copy_n_name
            char* const buf = static_cast<char* const>(malloc( name_in.length() + 2 + 30));
            sprintf(buf,"copy_%d_%s",val,name_in.c_str());
            name_out = buf;
            free(buf);
            ++val;
            break;
         }
      }
      if ( name_unique == true){
         return name_out;
      }else{
         continue;
      }
   }
   // shouldnt get here
   assert(false && __LINE__);
   return "";
}

// image was created on heap or by clone
// resource container takes ownership
void bitmap_resource_t::set_image_handle(int handle, osd_image* image)
{
      osd_image* old_image = find_osd_image(handle);
      assert((old_image != nullptr) && __LINE__);
      old_image->destroy();
      m_osd_image_map.at(handle) = image;
}
 
int bitmap_resource_t::get_new_handle()
{
   for (int i = m_min_handle; i < m_max_handle; ++i) {
         auto iter = m_handles_in_use.find (i);
         if (iter == m_handles_in_use.end()) {
               m_handles_in_use.insert (i);
               return i;
            }
   }
   m_handles_in_use.insert(m_max_handle);
   int result = m_max_handle;
   ++m_max_handle;
   return result;
}
 
// can only free bitmaps. to remove fonts do all at one go
bool bitmap_resource_t::free_handle (int handle)
{
   auto iter = m_handles_in_use.find (handle);
   if (iter != m_handles_in_use.end()) {
         m_handles_in_use.erase (iter);
         return true;
      }
   else {
         return false;
      }
}

// can find bitmaps and font elements
osd_image* bitmap_resource_t::find_osd_image(int handle)const
{
   auto iter = m_osd_image_map.find(handle);
   if(iter != m_osd_image_map.end()){
      return iter->second;
   }else{
      return nullptr;
   }
}
// can only add bitmaps not font elements
int bitmap_resource_t::add_bitmap( osd_bitmap* bmp)
{
   int new_handle = get_new_handle();
   m_osd_image_map.insert({new_handle,bmp});
   m_bitmaps.push_back(bmp);
   return new_handle;
}

void document::add_bitmap(osd_bitmap* bmp)
{
   //TODO  add check name is unique...
   int handle = m_resources->add_bitmap(bmp);
   wxGetApp().get_panel()->add_bitmap_handle(bmp->get_name(), handle);
   auto view = wxGetApp().get_view();
   auto frame = wxGetApp().get_main_frame();
   if (! view->have_image()) {
      view->copy_to_current_image (handle);
   }
   frame->enable_save_project (true);
   frame->enable_save_project_as (true);
}

void document::set_image(int handle, osd_image* image)
{
  m_resources->set_image_handle(handle,image);
}
 
bool document::is_modified() const
{
   return this->m_is_modified;
}
void document::set_modified (bool val)
{
   this->m_is_modified = val;
}
 
bool document::open_project (wxString const & path)
{
   // save current project if its modified etc
   // check the path
   // create a new resources

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
   
   for ( int i = 0; i < num_entries; ++i){
      wxZipEntry* entry = zipin.GetNextEntry();
      assert(entry && __LINE__);
      if ( !entry->IsDir()){
         std::string full_name = from_wxString<char>(entry->GetName()); 
         if ( ( full_name.find("bitmaps/",0,8) != std::string::npos) &&
            (full_name.find(".png",full_name.length()-4,4) != std::string::npos)
         ){
          std::string name = full_name.substr(8,full_name.length()-12);
          wxImage image(zipin,wxBITMAP_TYPE_PNG);
          osd_bitmap * bmp = ConvertTo_osd_bitmap(name,image);
          this->add_bitmap(bmp);
         }
      }
      delete entry;
   }
   return false;
}

// ret false if doc was not saved
// save zip of the project files
bool document::save_project()
{
   wxGetApp().get_view()->sync_hmi_view();
   wxString save_path = get_project_file_path() ;
   if (save_path == wxT ("")) {
         wxFileDialog fd {wxGetApp().get_main_frame(),
                          wxT ("Save Project"),
                          wxT (""),                    // default dir
                          wxT (""),                    // default file
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
 
bool document::ll_save_project (wxString const & path)
{
   wxFileOutputStream out {path};
   if (! out.IsOk()) {
         wxMessageBox (wxT ("Output file failed\n"));
         return false;
   }else{
         wxZipOutputStream zipout {out};
         zipout.PutNextDirEntry(wxT("fonts"));
         //make fonts and bitmaps dirs
/*
         for (size_t i = 0; i < m_resources->get_num_fonts(); ++i) {
               wxString name = wxString::Format (wxT ("font_%d.png"), i);
               zipout.PutNextEntry (name);
               osd_image * osd_image = m_resources->get_font_at(i);
               wxImage* wx_image = ConvertTo_wxImage (*inosd_image);
               wx_image->SaveFile (zipout, wxBITMAP_TYPE_PNG);
               zipout.CloseEntry();
            }
*/

         zipout.PutNextDirEntry(wxT("bitmaps"));
         for (size_t i = 0; i < m_resources->get_num_bitmaps(); ++i) {
               osd_image * osd_image = m_resources->get_bitmap_at(i);
               auto bmp = dynamic_cast<osd_bitmap*>(osd_image);
               assert(bmp && __LINE__);
               wxString name = wxT("bitmaps/");
               name += to_wxString(bmp->get_name());
               name += wxT(".png");
               zipout.PutNextEntry (name);
               wxImage* wx_image = ConvertTo_wxImage(*bmp);
               wx_image->SaveFile (zipout, wxBITMAP_TYPE_PNG);
          }
      }

   this->set_modified (false);
   wxGetApp().get_main_frame()->enable_save_project (false);
   return true;

}
 
document::document()
   : m_page_size {quan::length::mm{500}, quan::length::mm{500}}
, m_pixel_size {quan::length::mm{10}, quan::length::mm{10}}
, m_resources {new bitmap_resource_t}, m_is_modified {false}
{}

bool
document::load_png_file (wxString const & path)
{

   if (! wxImage::FindHandler (wxBITMAP_TYPE_PNG)) {
         wxImage::AddHandler (new wxPNGHandler);
      }

   wxImage image;
   if (! image.LoadFile (path)) {
         wxMessageBox (wxT ("image Load failed"));
         return false;
      }
    
   std::string name  =  quan::fs::get_basename(
         from_wxString<char>(path)
      );
   name = quan::fs::strip_file_extension(name);
   name = m_resources->make_unique_image_name(name);
   osd_bitmap * bmp = ConvertTo_osd_bitmap(name,image);
   this->add_bitmap(bmp);
/*
   int handle = m_resources->add_bitmap(bmp);
   wxGetApp().get_panel()->add_bitmap_handle(bmp->get_name(), handle);
   auto view = wxGetApp().get_view();
   auto frame = wxGetApp().get_main_frame();
   if (! view->have_image()) {
      view->copy_to_current_image (handle);
   }
   frame->enable_save_project (true);
   frame->enable_save_project_as (true);
*/
   return true;
}
 
quan::two_d::vect<quan::length::mm> const &
document::get_page_size() const
{
   return m_page_size;
}
 
void
document::set_page_size (quan::two_d::vect<quan::length::mm> const & size)
{
   m_page_size = size;
}
 
quan::two_d::vect<quan::length::mm> const &
document::get_pixel_size_mm() const
{
   return m_pixel_size;
}

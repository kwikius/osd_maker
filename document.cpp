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
#include "bitmap_resource.hpp"
#include "main_frame.h"
#include "view.hpp"
#include "panel.hpp"

using quan::gx::wxwidgets::from_wxString;
using quan::gx::wxwidgets::to_wxString;

osd_image* document::get_image( int handle)const 
{return m_resources->find_osd_image(handle);}

void document::add_bitmap(osd_bitmap* bmp)
{
   assert((bmp != nullptr) && __LINE__);
   assert( ! m_resources->find_bitmap_name(bmp->get_name()));
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
  assert(image && __LINE__);
  assert( (handle != -1) && __LINE__);
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
   // create a vector of bitmaps
   // add each to vector of bitmaps
   // or create a resources
   auto temp_resources = new bitmap_resource_t;
   for ( int i = 0; i < num_entries; ++i){
      wxZipEntry* entry = zipin.GetNextEntry();
      assert(entry && __LINE__);
      if ( !entry->IsDir()){
         std::string full_name = from_wxString<char>(entry->GetName()); 
         if ( ( full_name.find("bitmaps/",0,8) != std::string::npos) &&
            (full_name.find(".png",full_name.length()-4,4) != std::string::npos)
         ){
          std::string name = full_name.substr(8,full_name.length()-12);
          if (temp_resources->find_bitmap_name(name)){
            wxMessageBox(wxT("Invalid file, multiple bitmaps of same name"));
            delete temp_resources;
            delete entry;
            return false;
          }
          wxImage image(zipin,wxBITMAP_TYPE_PNG);
          osd_bitmap * bmp = ConvertTo_osd_bitmap(name,image);
          temp_resources->add_bitmap(bmp);
         }
      }
      delete entry;
   }
   wxGetApp().get_main_frame()->clear();
   for ( size_t i =0, end = temp_resources->get_num_bitmaps(); i < end; ++i){
      int handle = -1;
      assert(temp_resources->get_bitmap_handle_at(i,handle) && __LINE__);
      auto bmp = temp_resources->move_osd_bitmap(handle);
      assert(bmp);
      this->add_bitmap(bmp);
   }
   this->m_project_file_path = path;
   wxGetApp().get_main_frame()->SetTitle(path);
   return true;
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
               int handle = -1;
               assert(m_resources->get_bitmap_handle_at(i,handle) && __LINE__);
               osd_image * osd_image = m_resources->find_osd_image(handle);
               assert( osd_image && __LINE__);
               osd_bitmap* bmp = dynamic_cast<osd_bitmap*>(osd_image);
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

void document::reset()
{
  if ( m_resources){
   delete m_resources;
  }
  m_resources = new bitmap_resource_t;
  m_is_modified = false;
  m_project_name = wxT("");
  m_project_file_path = wxT("");
}

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
    
   std::string name = quan::fs::get_basename(
      from_wxString<char>(path)
   );
   name = quan::fs::strip_file_extension(name);
   name = m_resources->make_unique_bitmap_name(name);
   osd_bitmap * bmp = ConvertTo_osd_bitmap(name,image);
   this->add_bitmap(bmp);
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

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
   return false;
}

void document::set_image(int handle, osd_image* image)
{
  m_resources->set_image_handle(handle,image);
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
      }
   else {
         wxZipOutputStream zipout {out};
#if 0
         for (uint32_t i = 0; i < get_num_bitmap_elements(); ++i) {
               wxString name = wxString::Format (wxT ("filename_%d.png"), i);
               zipout.PutNextEntry (name);
               osd_image * inosd_image = get_osd_image_ptr (i);
               wxImage* wx_image = ConvertTo_wxImage (*inosd_image);
               wx_image->SaveFile (zipout, wxBITMAP_TYPE_PNG);
            }
#endif
      }
#if 0
   m_project_file_path = path;
   assert (m_image_container && __LINE__);
   m_image_container->set_modified (false);
   wxGetApp().get_main_frame()->enable_save_project (false);
   return true;
#endif
   return false;
}
 
 
document::document()
   : m_page_size {quan::length::mm{500}, quan::length::mm{500}}
, m_pixel_size {quan::length::mm{10}, quan::length::mm{10}}
, m_resources {new bitmap_resource_t}, m_is_modified {false}
{}
 
bool
document::load_png_file (wxString const & path)
{
   // init of bitmap_lib here as the type of lib dependent on file type
   // but prob abandon that and just verify it when saving
   
   if (! wxImage::FindHandler (wxBITMAP_TYPE_PNG)) {
         wxImage::AddHandler (new wxPNGHandler);
      }
   // load into image
   wxImage image;
   if (! image.LoadFile (path)) {
         wxMessageBox (wxT ("image Load failed"));
         return false;
      }
    
   // create unique name from filename
   std::string name  =  quan::fs::get_basename(
         quan::gx::wxwidgets::from_wxString<char>(path)
      );
   name = quan::fs::strip_file_extension(name);
   name = m_resources->make_unique_image_name(name);
   
  // wxMessageBox(quan::gx::wxwidgets::to_wxString(name));

   osd_image::size_type bitmap_size {image.GetWidth(), image.GetHeight() };
   osd_bitmap * bmp = new osd_bitmap {name,bitmap_size};
   // create the osd bitmap
   for (uint32_t y = 0; y < bitmap_size.y; ++y) {
         for (uint32_t x = 0; x < bitmap_size.x; ++x) {
               osd_image::colour  colour = osd_image::colour::transparent;
               if (!image.IsTransparent (x, y)) {
                     uint8_t red = image.GetRed (x, y);
                     uint8_t green = image.GetGreen (x, y);
                     uint8_t blue = image.GetBlue (x, y);
                     float mono = (red * 0.2126 + green * 0.7152 + blue * 0.0722) / 255;
                     if (mono < 1. / 4) {
                           colour = osd_image::colour::black;
                        }
                     else {
                           if (mono < 1. / 2) {
                                 colour = osd_image::colour::grey;
                              }
                           else {
                                 colour = osd_image::colour::white;
                              }
                        }
                  }
               bmp->set_pixel_colour ( {x, y}, colour);
            }
      }
   int handle = m_resources->add_bitmap(bmp);
   wxGetApp().get_panel()->add_bitmap_handle(name, handle);
   auto view = wxGetApp().get_view();
   auto frame = wxGetApp().get_main_frame();
   if (! view->have_image()) {
      view->copy_to_current_image (handle);
      
   }
   frame->enable_save_project (true);
   frame->enable_save_project_as (true);
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
 
 
 
 
 
 
 
 
 
 
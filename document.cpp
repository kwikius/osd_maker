#include <wx/zipstrm.h>
#include <wx/wfstream.h>

#include "app.h"
#include "document.hpp"
#include "view.hpp"

bool osd_bitmap::get_pixel_colour( pos_type const & p, osd_image::colour & c) const
{
     if (( p.x >= m_size.x) || ( p.y >= m_size.y)) {
          return false;
     }
     auto idx = p.y * m_size.x + p.x;
     c = m_data.at(idx);
     return true;
}
bool osd_bitmap::set_pixel_colour( pos_type const & p, osd_image::colour c)
{
     if (  c == colour::invalid) {
          return false;
     }
     if (( p.x >= m_size.x) || ( p.y >= m_size.y)) {
          return false;
     }
     auto idx = p.y * m_size.x + p.x;
     m_data.at(idx) = c;
     return true;
}

bool document::save_document()
{
 auto view = wxGetApp().get_view();
 if ( view->is_modified() ){
   if ( wxMessageBox(wxT("View modified. Commit to lib before saving?"),
    wxT("Confirm"),
    wxICON_QUESTION | wxYES_NO  ) == wxYES ) {
      auto view_image = view->clone_current_image();
      assert(view_image && __LINE__);
      int32_t index = view->get_current_bitmap_lib_index();
      assert((index >= 0) && __LINE__);
      assert ( get_num_bitmap_elements() > static_cast<uint32_t>(index));
      // could check same, need to watch if stuff is deleted!
      m_image_container->at(index)->destroy();
      m_image_container->at(index) = view_image;
      view->set_modified(false);
    }
 }
  
 ll_save_document(wxT("/home/andy/cpp/projects/OSDBitmapMaker/zipfile1.zip"));
 return true;
}
bool document::ll_save_document(wxString const & path)
{
    wxFileOutputStream out{path};
    if ( ! out.IsOk()){
         wxMessageBox(wxT("Output file failed\n"));
         return false;
    }else{
         wxZipOutputStream zipout{out};
         for ( uint32_t i = 0; i < get_num_bitmap_elements(); ++i){
            wxString name = wxString::Format(wxT("filename_%d.png"),i);
            zipout.PutNextEntry(name);
            osd_image * inosd_image = get_osd_image_ptr(i);
            wxImage* wx_image = ConvertTo_wxImage(*inosd_image);
            wx_image->SaveFile(zipout,wxBITMAP_TYPE_PNG);
         }
    }
    return true;
}

bool document::init_bitmap_lib(image_container::type t)
{
     if ( have_image_lib() || (t == image_container::type::Undefined)) {
          return false;
     }
     m_image_container = new bitmap_lib;
     m_container_type = t;
     // send event
     return true;
}

document::document()
     : m_page_size {quan::length::mm{500},quan::length::mm{500}}
, m_pixel_size {quan::length::mm{10},quan::length::mm{10}}
, m_image_container {nullptr}
, m_container_type {image_container::type::Undefined} {
     // init the drawing image_container
     // or load a bitmap
     //  create_osd_null_image();
}

bool
document::load_png_file( wxString const & path)
{
     // init of bitmap_lib here as the type of lib dependent on file type
     // but prob abandon that and just verify it when saving
     if (!this->have_image_lib()) {
          if (!this->init_bitmap_lib(image_container::type::ImageLib)) {
               wxMessageBox(wxT("Load bitmap_lib failed"));
               return false;
          }
     }
     if ( this->m_image_container == nullptr) {
          wxMessageBox(wxT("empty container fail"));
          return false;
     }
     if ( ! wxImage::FindHandler(wxBITMAP_TYPE_PNG)) {
          wxImage::AddHandler(new wxPNGHandler);
     }
     // load into image
     wxImage image;
     if ( ! image.LoadFile(path)) {
          wxMessageBox(wxT("image Load failed"));
          return false;
     }
     // create the osd bitmap
     osd_image::size_type bitmap_size {image.GetWidth(),image.GetHeight()};
     osd_bitmap * bmp = new osd_bitmap {bitmap_size};
     for ( uint32_t y = 0; y < bitmap_size.y; ++y) {
          for (uint32_t x = 0; x < bitmap_size.x; ++x) {
               osd_image::colour  colour = osd_image::colour::transparent;
               if ( !image.IsTransparent(x,y)) {
                    uint8_t red = image.GetRed(x,y);
                    uint8_t green = image.GetGreen(x,y);
                    uint8_t blue = image.GetBlue(x,y);
                    float mono = (red * 0.2126 + green * 0.7152 + blue * 0.0722)/ 255;
                    if ( mono < 1./4) {
                         colour = osd_image::colour::black;
                    } else {
                         if (mono < 1./2) {
                              colour = osd_image::colour::grey;
                         } else {
                              colour = osd_image::colour::white;
                         }
                    }
               }
               bmp->set_pixel_colour( {x,y},colour);
          }
     }
     this->m_image_container->push_back(bmp);
     return true;
}

quan::two_d::vect<quan::length::mm> const &
document::get_page_size() const
{
     return m_page_size;
}

void
document::set_page_size( quan::two_d::vect<quan::length::mm> const & size)
{
     m_page_size = size;
}

quan::two_d::vect<quan::length::mm> const &
document::get_pixel_size_mm()const
{
     return m_pixel_size;
}

bool  document::get_bitmap_size(int32_t idx, osd_image::size_type & size)const
{
     if ( this->m_image_container == nullptr) {
          return false;
     }
     if ( ( idx < 0) || (m_image_container->get_num_elements() <= static_cast<size_t>(idx)) ) {
          return false;
     }

     osd_image* const bmp = m_image_container->at(idx);
     size = bmp->get_size();
     return true;
}

bool
document::get_pixel_colour(int32_t idx,osd_image::pos_type const & pos,osd_image::colour& colour)const
{
     if ( this->m_image_container == nullptr) {
          return false;
     }
     if ( ( idx < 0) || (m_image_container->get_num_elements() <= static_cast<size_t>(idx)) ) {
          return false;
     }

     osd_image* const bmp = m_image_container->at(idx);
     osd_image::size_type const size = bmp->get_size();
     if ( (static_cast<uint32_t>(pos.x) < size.x)
               && (static_cast<uint32_t>(pos.y) < size.y)
        ) {
          return bmp->get_pixel_colour(pos,colour);
     } else {
          return false;
     }
}
 
bool
document::set_pixel_colour(int32_t idx,osd_image::pos_type const & pos,osd_image::colour colour)
{
     if ( this->m_image_container == nullptr) {
          return false;
     }
     if ( ( idx < 0) || (m_image_container->get_num_elements() <= static_cast<size_t>(idx)) ) {
          return false;
     }
 
     osd_image* const bmp = m_image_container->at(idx);
     osd_image::size_type const size = bmp->get_size();
     if ( (static_cast<uint32_t>(pos.x) < size.x)
               && (static_cast<uint32_t>(pos.y) < size.y)
        ) {
          return bmp->set_pixel_colour(pos,colour);
     } else {
          return false;
     }
}
 
 
 
 
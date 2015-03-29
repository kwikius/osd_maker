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

#include "osd_bmp_app.hpp"
#include "document.hpp"
#include "document/osd_object_database.hpp"
#include "gui/main_frame.hpp"
#include "gui/view.hpp"
#include "gui/panel.hpp"

#include "graphics_api/objects/db_bitmap.hpp"


using quan::gx::wxwidgets::from_wxString;
using quan::gx::wxwidgets::to_wxString;

document::dynamic_bitmap* document::get_bitmap( int handle)const
{return m_database->find_bitmap_by_handle(handle);}

document::dynamic_bitmap* document::get_bitmap( std::string const & name)const
{return m_database->find_bitmap_by_name(name);}

db_font* document::get_font(int handle) const
{return m_database->find_font_by_handle(handle);}

db_font* document::get_font( std::string const & name)const
{
    return m_database->find_font_by_name(name);
}

bool document::add_new_bitmap(std::string const & name, dynamic_bitmap::size_type size)
{
   if(get_bitmap(name) == nullptr){
       auto bmp = new dynamic_bitmap(name,size);
       add_bitmap(bmp);
       return true;
   }else{
      return false;
   }
}
/*
-p  /home/andy/cpp/projects/OSDBitmapMaker/test/rotating_compass1.zip
*/
void document::add_bitmap(dynamic_bitmap* bmp)
{
   assert((bmp != nullptr) && __LINE__);
   assert( ! m_database->find_bitmap_by_name(bmp->get_name()));
   int handle = m_database->add_bitmap(bmp);
   wxGetApp().get_panel()->add_bitmap_handle(bmp->get_name(), handle);
   auto view = wxGetApp().get_view();
  // auto frame = wxGetApp().get_main_frame();
   if (! view->have_image()) {
      view->copy_to_current_image (handle);
   }
   this->set_modified(true);
}

bool document::delete_font_element( int handle)
{
   osd_object_database::dynamic_bitmap * bmp 
      = this->m_database->move_font_element_by_handle(handle);
   if (bmp){
      delete bmp;
      return true;
   }else{
      return false;
   }
}

void document::set_image(int handle, dynamic_bitmap* image)
{
  assert(image && __LINE__);
  assert( (handle != -1) && __LINE__);
  m_database->set_image_handle(handle,image);
}

bool document::is_modified() const
{
   return this->m_is_modified;
}

void document::set_modified (bool val)
{
   this->m_is_modified = val;
   auto * frame = wxGetApp().get_main_frame();
   frame->enable_save_project(val);
   frame->enable_save_project_as (val);
}

// ret false if doc was not saved
// save zip of the project files
bool document::save_project()
{
   wxGetApp().get_view()->sync_hmi_view();

   wxString save_path = get_project_file_path() ;
   if (save_path == wxT ("")) {
      // get current path and name

     wxString dirname =  wxGetApp().get_config()->Read(wxT("/CurrentProject/FileDir"),wxT(""));
     wxString filename =  wxGetApp().get_config()->Read(wxT("/CurrentProject/FileName"),wxT(""));

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

bool document::save_project_as(wxString const & path)
{
   
   if (ll_save_project(path) ){
      // TODO set project name, tree name etc
     this->m_project_file_path = path;
     wxGetApp().get_main_frame()->SetTitle(path);

     std::string str_path = from_wxString<char>(path);
     std::string basename =  quan::fs::get_basename(str_path);
     std::string dirname =  str_path.substr(0,str_path.rfind('/'));

     wxGetApp().get_config()->Write(wxT("/CurrentProject/FileDir"),to_wxString(dirname));
     wxGetApp().get_config()->Write(wxT("/CurrentProject/FileName"),to_wxString(basename));

     std::string std_path
      = quan::fs::strip_file_extension(
            quan::fs::get_basename(from_wxString<char>(path))
      );
     wxGetApp().get_panel()->set_project_name(to_wxString(std_path));
     return true;
   }else{
      return false;
   }
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
         for (size_t i = 0; i < m_database->get_num_fonts(); ++i) {
            int font_handle =-1;
            assert( m_database->get_font_handle_at(i,font_handle) && __LINE__) ;
            db_font * cur_font = get_font(font_handle);
            assert( cur_font != nullptr && __LINE__);
            wxString dirname = wxT("fonts/");
            dirname += to_wxString(cur_font->get_name());
            zipout.PutNextDirEntry (dirname);
            for ( int32_t j= cur_font->get_begin();
                  j < (cur_font->get_num_elements() + cur_font->get_begin());
                  ++j){
               int image_handle = -1;
               assert(cur_font->get_handle_at(j, image_handle) && __LINE__);
               assert ((image_handle != -1) && __LINE__);
               dynamic_bitmap * bmp = get_bitmap(image_handle);
               assert(bmp && __LINE__);
               wxString filename = dirname + wxT("/");
               filename += wxString::Format(wxT("char%d.png"),j);
               zipout.PutNextEntry (filename);
               wxImage* wx_image = ConvertTo_wxImage(*bmp);
               wx_image->SaveFile (zipout, wxBITMAP_TYPE_PNG);
            }
         }
         zipout.PutNextDirEntry(wxT("bitmaps"));
         for (size_t i = 0; i < m_database->get_num_bitmaps(); ++i) {
               int handle = -1;
               assert(m_database->get_bitmap_handle_at(i,handle) && __LINE__);
               dynamic_bitmap * bmp = m_database->find_bitmap_by_handle(handle);
//               assert( dynamic_bitmap && __LINE__);
//               dynamic_bitmap* bmp = dynamic_cast<dynamic_bitmap*>(dynamic_bitmap);
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
   
   return true;
}

document::document()
   : m_page_size {quan::length::mm{500}, quan::length::mm{500}}
, m_pixel_size {quan::length::mm{10}, quan::length::mm{10}}
, m_database {new osd_object_database}, m_is_modified {false}
{}

void document::reset()
{
  if ( m_database){
   delete m_database;
  }
  m_database = new osd_object_database;
  m_is_modified = false;
  m_project_name = wxT("");
  m_project_file_path = wxT("");
}

bool document::load_png_file (wxString const & path)
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
   name = m_database->make_unique_bitmap_name(name);
   dynamic_bitmap * bmp = ConvertTo_osd_bitmap(name,image);
   this->add_bitmap(bmp);
   this->set_modified(true);
   return true;
}

namespace {

bool line_string_to_val (std::string const & str, uint8_t & out)
{
   if ( str.length() < 8){
      wxMessageBox(wxT("Short"));
      return false;
   }
   out = 0;
   for (uint8_t j = 0; j < 8; ++j) {
      switch (str.at(j)) {
      case '0':
         break;
      case '1':
         out  |= (1 << j);
         break;
      default:
         wxMessageBox(wxString::Format(wxT("%c"),str.at(j)));
         return false;
      }
   }
   return true;
}

void  process_mcm_char (uint8_t inval, document::dynamic_bitmap* out,
         document::dynamic_bitmap::pos_type & pos)
{
   for (uint8_t j = 0; j < 8; j+=2) {
      uint8_t code = (inval >> j) & 0b11;
      typedef quan::uav::osd::colour_type colour_type;
      colour_type colour = colour_type::transparent;
      switch (code) {
      case  0b00: // black
         //out << "#";
         colour = colour_type::black;
         break;
      case  0b01: // white
        // out << " ";
         colour = colour_type::white;
         break;
      case  0b10: // transparent
      case  0b11:
        // out << "*" ;
         colour = colour_type::transparent;
         break;
      }
      //assert(( colour != colour_type::invalid) && __LINE__ );
      out->set_pixel(pos,colour);
      ++pos.x;
   }
}
// to a bitmap
void output_mcm_char( uint8_t ch, std::vector <std::vector<uint8_t> > const & chars_vect,
   document::dynamic_bitmap* bitmap)
{
   std::vector<uint8_t> const & v = chars_vect.at(ch);
   uint32_t iter = 0;
   quan::uav::osd::pxp_type pos{0,0};
   for ( uint32_t line = 0; line < 18 ; ++line){
      pos.x = 0;
      pos.y = line;
      for ( uint32_t row_elem = 0; row_elem < 3; ++row_elem){
         process_mcm_char(v.at(iter),bitmap,pos);
         ++iter;
      }
   }
}

} //namespace

bool document::load_mcm_font_file (wxString const & path)
{
   std::string path1 = from_wxString<char>(path);
   std::ifstream in(path1);

   if (!in){
         wxMessageBox (wxT ("Failed to open Input file\n"));
         return false;
   }

   std::string str;
   std::getline(in,str);

   int count = 0;

   std::vector <std::vector<uint8_t> >  chars_vect;
   bool success = false;
   while (in && ! in.eof()){
      std::vector<uint8_t>  char_vect;
      for (uint32_t i = 0; i < 64; ++i) {
         std::getline (in,str);

         uint8_t out = 0;
         if (line_string_to_val (str,out)) {
            char_vect.push_back (out);
         }else{
            wxMessageBox(wxT("Failed 1"));
            return false;
         }
      }

      chars_vect.push_back (char_vect);
      // std::cout << "char " << count << " done\n";
      if (++count == 256) {
        // std::cout << "all chars done\n";
         success =true;
      }
   }
   if (!success){
       wxMessageBox(wxT("Input file failed"));
      return false;
   }
   std::vector<dynamic_bitmap*> font_chars;
   dynamic_bitmap::size_type size{12,18};
   constexpr int begin = 0 ;
   constexpr int end = 256;

   for ( int ch = begin; ch < end; ++ch){
      char name[] = {'\'', static_cast<char>(ch),'\'','\0'};
      auto fe = new dynamic_bitmap{name,size};
      output_mcm_char(ch,chars_vect,fe);
      font_chars.push_back(fe);
   }
   assert( (font_chars.size() == (end - begin))  && __LINE__);

   std::string name = quan::fs::get_basename(
      from_wxString<char>(path)
   );
   name = quan::fs::strip_file_extension(name);
   name = m_database->make_unique_font_name(name);

   auto new_font = new db_font{name,size,begin};
   int first_handle = -1;
   for ( int ch = begin; ch < end; ++ch){
      try{
         auto elem = font_chars.at(ch-begin);
         int handle = m_database->add_font_element(elem);
         if ( ch == '8'){
            first_handle = handle;
         }
         new_font->set_handle_at(ch, handle);

      }catch( std::exception & e){
         wxMessageBox(wxString::Format(wxT("Exception %d"),ch));
         delete new_font;
         return false;
      }
   }

   int font_handle = m_database->add_font(new_font);

   if (! wxGetApp().get_view()->have_image()) {
      wxGetApp().get_view()->copy_to_current_image (first_handle);
      // TODO set focus to it in panel and preview
   }
   wxGetApp().get_panel()->add_font_handle(new_font->get_name(),font_handle);
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

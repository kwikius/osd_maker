#ifndef QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED
#define QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED

#include <vector>

#include <wx/wx.h>
#include <wx/image.h>

#include <quan/two_d/vect.hpp>
#include <quan/length.hpp>

#include "osd_image.hpp"
#include <map>
#include <set>

  struct bitmap_resource_t{
      bitmap_resource_t(): m_max_handle{m_min_handle}{}
      ~bitmap_resource_t(){

         for ( auto v : m_fonts){
            delete v;
         }

         for ( auto v : m_osd_image_map){
            v.second->destroy();
         }
      }
      osd_image* find_osd_image( int handle);
      int add_osd_image( osd_image*);
      void set_image_handle(int handle, osd_image* image);
      private:
     // bitmap_lib m_bitmap_lib;
     //  fontlib
      std::vector<font*> m_fonts;

      bitmap_resource_t (bitmap_resource_t const&) = delete;
      bitmap_resource_t& operator= (bitmap_resource_t const&) = delete;
  
      std::map<int,osd_image*> m_osd_image_map;
      std::set<int> m_handles_in_use;
      int get_new_handle();
      bool free_handle(int n);
      int m_max_handle;
      static constexpr int m_min_handle = 1;
    
  };

struct document {

     document();
     
     quan::two_d::vect<quan::length::mm> const &
     get_page_size() const ;
     void set_page_size( quan::two_d::vect<quan::length::mm> const & size);

   //  bool  get_bitmap_size(int32_t idx, osd_image::size_type &)const;
     quan::two_d::vect<quan::length::mm> const &
            get_pixel_size_mm()const;

     bool is_modified() const;
     void set_modified( bool );
     void set_project_name(wxString const & str) { m_project_name = str;}
     bool save_project();
     bool open_project(wxString const & path);
    // bool do_save_file_dialog();
     bool load_mcm_font_file (std::istream & in);
     bool load_png_file(wxString const &path);
    /// bool init_bitmap_lib(image_container::type t);
     wxString get_project_file_path(){ return m_project_file_path;}
     osd_image* get_image( int handle)const {return m_resources->find_osd_image(handle);}
private:
     bool ll_save_project(wxString const & path);
     quan::two_d::vect<quan::length::mm> m_page_size;
     quan::two_d::vect<quan::length::mm> m_pixel_size; // {mm{10},mm{10}};

     wxString m_project_file_path;
     wxString m_project_name;

     bitmap_resource_t* m_resources;
     bool m_is_modified;
     
    
};

#endif // QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED

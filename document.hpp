#ifndef QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED
#define QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED

#include <vector>

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/treectrl.h>
#include <quan/two_d/vect.hpp>
#include <quan/length.hpp>

#include "osd_image.hpp"
#include "font.hpp"
#include <map>
#include <set>

struct bitmap_resource_t;

struct document {
     document();
     void reset();
     quan::two_d::vect<quan::length::mm> const &
     get_page_size() const ;
     void set_page_size( quan::two_d::vect<quan::length::mm> const & size);
     quan::two_d::vect<quan::length::mm> const &
     get_pixel_size_mm()const;
     bool is_modified() const;
     void set_modified( bool );
     void set_project_name(wxString const & str) { m_project_name = str;}
     bool save_project();
     bool open_project(wxString const & path);
     bool load_mcm_font_file (wxString const & path);
     bool load_png_file(wxString const &path);
     wxString get_project_file_path(){ return m_project_file_path;}
     // doc doesnt relinquish ownership of image
     osd_image* get_image( int handle)const ;
     osd_bitmap* get_bitmap( std::string const & name)const ;
     font* get_font(int handle) const;
     font* get_font( std::string const & name)const;
     // doc takes ownership of image which was created on heap
     void set_image(int handle, osd_image* image);
     // Take ownership of bitmap which was created on heap
  
private:

     void add_bitmap(osd_bitmap*);
    // void add_font ( font*);

     bool ll_save_project(wxString const & path);
     quan::two_d::vect<quan::length::mm> m_page_size;
     quan::two_d::vect<quan::length::mm> m_pixel_size; // {mm{10},mm{10}};
     wxString m_project_file_path;
     wxString m_project_name;
     bitmap_resource_t* m_resources;
     bool m_is_modified;
};

#endif // QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED

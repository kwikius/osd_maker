#ifndef QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED
#define QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED

#include <vector>
#include <map>
#include <set>

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/treectrl.h>

#include <quan/two_d/vect.hpp>
#include <quan/length.hpp>

#include <quan/uav/osd/dynamic/bitmap.hpp>
#include "graphics_api/font.hpp"

struct osd_object_database;

struct document {

     typedef quan::uav::osd::dynamic::bitmap dynamic_bitmap;

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
     bool save_project_as(wxString const & path);
     bool open_project(wxString const & path);
     bool load_mcm_font_file (wxString const & path);
     bool load_png_file(wxString const &path);
     bool add_new_bitmap(std::string const & name, dynamic_bitmap::size_type size);
     wxString get_project_file_path(){ return m_project_file_path;}
     osd_object_database const & get_database() { return *m_database;}
     // doc doesnt relinquish ownership of image
     dynamic_bitmap* get_bitmap( int handle)const ;
     dynamic_bitmap* get_bitmap( std::string const & name)const ;
     font* get_font(int handle) const;
     font* get_font( std::string const & name)const;
     // doc takes ownership of image which was created on heap
     void set_image(int handle, dynamic_bitmap* image);
     // Take ownership of bitmap which was created on heap
     void add_bitmap(dynamic_bitmap*);
private:
    // void add_font ( font*);
     bool ll_save_project(wxString const & path);
     quan::two_d::vect<quan::length::mm> m_page_size;
     quan::two_d::vect<quan::length::mm> m_pixel_size; // {mm{10},mm{10}};
     wxString m_project_file_path;
     wxString m_project_name;
     osd_object_database* m_database;
     bool m_is_modified;
};

#endif // QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED

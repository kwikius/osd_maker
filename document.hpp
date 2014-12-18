#ifndef QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED
#define QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED

#include <vector>

#include <wx/wx.h>
#include <wx/image.h>

#include <quan/two_d/vect.hpp>
#include <quan/length.hpp>

#include "osd_image.hpp"

struct document {
     document();
     quan::two_d::vect<quan::length::mm> const &
            get_page_size() const ;
     void set_page_size( quan::two_d::vect<quan::length::mm> const & size);

     bool  get_bitmap_size(int32_t idx, osd_image::size_type &)const;
     quan::two_d::vect<quan::length::mm> const &
            get_pixel_size_mm()const;
     bool get_pixel_colour(int32_t idx,osd_image::pos_type const & pos, osd_image::colour& col) const;
     bool set_pixel_colour(int32_t idx,osd_image::pos_type const & pos, osd_image::colour);
     osd_image* get_osd_image_ptr(int32_t idx) 
     {
         if ( ( idx <= 0) && have_image_lib() && (static_cast<size_t>(idx) < m_image_container->get_num_elements()) ){
            return m_image_container->at(idx);
         }else{
            return nullptr;
         }
     }

     bool have_image_lib() const{
          return m_image_container != nullptr;
     }
     size_t get_num_bitmap_elements()const
     {
         if ( have_image_lib()){
            return m_image_container->get_num_elements();
         }else{
            return 0U;
         }
     }
     bool load_mcm_font_file (std::istream & in);
     bool load_png_file(int32_t pos,wxString const &path);
     bool init_bitmap_lib(image_container::type t);
private:
     quan::two_d::vect<quan::length::mm> m_page_size;
     quan::two_d::vect<quan::length::mm> m_pixel_size; // {mm{10},mm{10}};
     image_container * m_image_container;
     image_container::type m_container_type;
};

#endif // QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED

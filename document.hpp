#ifndef QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED
#define QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED

#include <vector>

#include <wx/wx.h>
#include <wx/image.h>

#include <quan/two_d/vect.hpp>
#include <quan/three_d/vect.hpp>
#include <quan/length.hpp>

/*
 Fontlib
 BitmapLib
 FontMode or BitmapMode
*/
  


// either a font element( with fixed image size)
// or a bitmap_lib allowing images of different sizes
// have num_img
// display imag

struct osd_image{
    typedef quan::two_d::vect<size_t> pos_type;
    typedef quan::two_d::vect<size_t> size_type;
    enum class colour{
      grey = 0b00,
      black = 0b01,
      white = 0b10,
      transparent = 0b11
    };
    virtual size_type get_size()const;
    //false if out of range
    virtual bool get_colour( pos_type const & p, colour & c) const;
    // false if out of range
    virtual bool set_colour( pos_type const & p, colour c);

   // convert to wxbitmap or rebuild bitmap from data
   // convert to wximage or rebuild image from

};

// virtual base for fonts and bitmap libs
// in practise either a font or an image_lib
struct abc_image_container{
   virtual size_t get_num_elements()const;
   // at
};
struct document{
   // start with a bitmap
   // load 
   document();
   quan::two_d::vect<quan::length::mm> const & 
      get_map_size() const ;
   void set_map_size( quan::two_d::vect<quan::length::mm> const & size);
   quan::two_d::vect<uint32_t> const & 
      get_bitmap_size()const;
   quan::two_d::vect<quan::length::mm> const & 
      get_pixel_size_mm()const;
   bool get_pixel_colour(quan::two_d::vect<int> const & pos,  uint8_t & colour);
   bool set_pixel_colour(quan::two_d::vect<int> const & pos, uint8_t colour);
   static constexpr uint8_t grey = 0;
   static constexpr uint8_t black = 1;
   static constexpr uint8_t white = 2;
   static constexpr uint8_t transparent = 3;
   
   bool have_image(){ return m_have_image;}
   bool load_mcm_font_file (std::istream & in);
   bool load_png_file(wxString const &path);
   private:

   quan::two_d::vect<quan::length::mm> m_map_size;
  // quan::length::mm  m_click_bucket;
   quan::two_d::vect<uint32_t> m_bitmap_size; //{12,18};
   // size of a single pixel in the view
   quan::two_d::vect<quan::length::mm> m_pixel_size; // {mm{10},mm{10}};
//
   std::vector<uint8_t> pixel_array;


   wxImage image;
   bool m_have_image;
   bool m_have_font;
      
};

#endif // QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED

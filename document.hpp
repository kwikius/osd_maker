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

struct osd_null_bitmap;
struct osd_image{
    typedef quan::two_d::vect<size_t> pos_type;
    typedef quan::two_d::vect<size_t> size_type;
    enum class colour{
      invalid = -1,
      grey = 0b00,
      black = 0b01,
      white = 0b10,
      transparent = 0b11
    };
    virtual size_type get_size()const=0;
    //false if out of range
    virtual bool get_pixel_colour( pos_type const & p, colour & c) const=0;
    // false if out of range
    virtual bool set_pixel_colour( pos_type const & p, colour c)=0;
    static osd_null_bitmap* get_null_bitmap(){return m_null_bitmap;}
    // delete usually 
    virtual void destroy() = 0;
    osd_image(){}
protected:
    virtual ~osd_image(){}
private:
   static osd_null_bitmap* m_null_bitmap;
   friend void create_osd_null_bitmap();
   // convert to wxbitmap or rebuild bitmap from data
   // convert to wximage or rebuild image from
   // save as ..
};

// name ?
struct osd_bitmap : osd_image{
   
   osd_bitmap(size_type const & size_in) 
   : m_size{size_in}, m_data{size_in.x * size_in.y,colour::transparent}
   {}
   size_type get_size() const { return m_size;}
   bool get_pixel_colour( pos_type const & p, colour & c) const;
   bool set_pixel_colour( pos_type const & p, colour c);
   void destroy() { delete this;}
   ~osd_bitmap(){}
   private:
   size_type m_size;
   std::vector<colour> m_data;

};
/*
use to fill lib
*/
//void create_osd_null_bitmap();
struct osd_null_bitmap : osd_image{
   size_type get_size() const { return size_type{0,0};}
   bool get_pixel_colour( pos_type const & p, colour & c) const { c = colour::transparent; return true;}
   bool set_pixel_colour( pos_type const & p, colour c) { return false;}
   void destroy() { }
   private: 
      osd_null_bitmap(){}
   friend void create_osd_null_bitmap();
   
};

// virtual base for fonts and bitmap libs
// in practise either a font or an image_lib
 // use map?
// to look up as a string use index to string
struct image_container{
   enum class type { Undefined, Font, ImageLib};
   virtual size_t get_num_elements()const =0;
   virtual osd_image * at(size_t pos) const=0;
   virtual osd_image *& at(size_t pos) =0;
   virtual ~image_container(){}
};

 struct bitmap_lib : image_container{
   size_t get_num_elements() const { return m_elements.size();}
   osd_image * at(size_t pos)const { return m_elements.at(pos);}
   osd_image *& at(size_t pos) { return m_elements.at(pos);}
   // assume p created on heap and takes ownership
   void push_back(osd_image* p) { m_elements.push_back(p);}
   bitmap_lib(){}
   ~bitmap_lib()
   {
      for (osd_image* p : m_elements){ p->destroy();}
   }
  private:
   std::vector <osd_image *> m_elements;
 };

struct document{
   document();
   quan::two_d::vect<quan::length::mm> const & 
      get_map_size() const ;
   void set_map_size( quan::two_d::vect<quan::length::mm> const & size);
    
    bool  get_bitmap_size(int32_t idx, osd_image::size_type &)const;
    quan::two_d::vect<quan::length::mm> const & 
    get_pixel_size_mm()const;
   bool get_pixel_colour(int32_t idx,osd_image::pos_type const & pos, osd_image::colour& col);
   bool set_pixel_colour(int32_t idx,osd_image::pos_type const & pos, osd_image::colour);

   bool have_image_lib(){ return m_image_container != nullptr;}
   bool load_mcm_font_file (std::istream & in);
   bool load_png_file(int32_t pos,wxString const &path);
   bool init_bitmap_lib(image_container::type t);
   private:

   quan::two_d::vect<quan::length::mm> m_map_size;
   quan::two_d::vect<quan::length::mm> m_pixel_size; // {mm{10},mm{10}};
   image_container * m_image_container;
   image_container::type m_container_type;
      
};

#endif // QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED

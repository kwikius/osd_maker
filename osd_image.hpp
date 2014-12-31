#ifndef OSD_BITMAP_MAKER_OSD_IMAGE_HPP_INCLUDED
#define OSD_BITMAP_MAKER_OSD_IMAGE_HPP_INCLUDED

#include <vector>
#include <wx/image.h>
#include <quan/two_d/vect.hpp>

//struct osd_null_image;
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
    virtual bool get_pixel_colour( pos_type const & p, colour & c) const=0;
    virtual bool set_pixel_colour( pos_type const & p, colour c)=0;
   // static osd_null_image* get_null_image(){return m_null_image;}
    virtual osd_image* clone() const = 0;
    // delete usually 
    virtual void destroy() = 0;
    enum class image_type{ Unknown,FontElement, Bitmap};
    image_type get_image_type()const{return m_image_type;}
protected:
    osd_image(image_type t):m_image_type{t}{}
    virtual ~osd_image(){}
private:
    image_type const m_image_type;

};

wxBitmap* ConvertTo_wxBitmap(osd_image const& in, wxColour const * (&colours)[4]);
wxImage* ConvertTo_wxImage(osd_image const& in);

struct osd_bitmap : osd_image{
   
   osd_bitmap(size_type const & size_in) 
   : osd_image{image_type::Bitmap}
   ,m_size{size_in}, m_data{size_in.x * size_in.y,colour::transparent}
   {}
   
   size_type get_size() const { return m_size;}
   bool get_pixel_colour( pos_type const & p, colour & c) const;
   bool set_pixel_colour( pos_type const & p, colour c);
   void destroy() { delete this;}
   osd_bitmap* clone() const
   {
       return new osd_bitmap(*this);
   }
   ~osd_bitmap(){}
   private:
   osd_bitmap( osd_bitmap const & in) 
   : osd_image{image_type::Bitmap},m_size{in.m_size}, m_data{in.m_data}{}
   osd_bitmap & operator = (osd_bitmap const &) = delete;
   size_type m_size;
   std::vector<colour> m_data;
};
   struct font{

   };
/*
struct osd_null_image : osd_image{
   size_type get_size() const { return size_type{0,0};}
   bool get_pixel_colour( pos_type const & p, colour & c) const { c = colour::transparent; return true;}
   bool set_pixel_colour( pos_type const & p, colour c) { return false;}
   void destroy() { }
   osd_null_image* clone() const { return const_cast<osd_null_image*>(this);}
   private: 
      osd_null_image(){}
   friend void create_osd_null_image();
   
};
*/
/*
// virtual base for fonts and bitmap libs
// in practise either a font or an image_lib
// to look up as a string use index to string
struct image_container{
   enum class type { Undefined, Font, ImageLib};
   virtual size_t get_num_elements()const =0;
   virtual osd_image * at(size_t pos) const=0;
   virtual osd_image *& at(size_t pos) =0;
   virtual void push_back(osd_image* p) =0;
   virtual bool is_modified() const = 0;
   virtual void set_modified ( bool b) = 0;
   virtual ~image_container(){}
};

 struct bitmap_lib : image_container{
   size_t get_num_elements() const { return m_elements.size();}
   osd_image * at(size_t pos)const { return m_elements.at(pos);}
   osd_image *& at(size_t pos) { return m_elements.at(pos);}
   // assume p created on heap and takes ownership
   void push_back(osd_image* p) { m_elements.push_back(p); m_is_modified = true;}
   bool is_modified() const {return m_is_modified;}
   void set_modified ( bool b){m_is_modified = b;}
   bitmap_lib():m_is_modified{false}{}
   ~bitmap_lib()
   {
      for (osd_image* p : m_elements){ p->destroy();}
   }
   osd_image* & back(){ return m_elements.back();}
  private:
   std::vector <osd_image *> m_elements;
   bool m_is_modified;
 };
*/
#endif // OSD_BITMAP_MAKER_OSD_IMAGE_HPP_INCLUDED

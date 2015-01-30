#ifndef OSD_BITMAP_MAKER_OSD_IMAGE_HPP_INCLUDED
#define OSD_BITMAP_MAKER_OSD_IMAGE_HPP_INCLUDED

#include <vector>
#include <wx/image.h>
#include <quan/two_d/vect.hpp>
#include <quan/two_d/box.hpp>

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
    colour get_pixel_colour(pos_type const & p) 
    { colour col = colour::invalid; get_pixel_colour(p,col); return col;}
      
    virtual bool set_pixel_colour( pos_type const & p, colour c)=0;
    virtual osd_image* clone() const = 0;
    virtual void destroy() = 0;
    enum class image_type{ Unknown,FontElement, Bitmap};
    image_type get_image_type()const{return m_image_type;}
    void set_image_type ( image_type in){ m_image_type = in;}
protected:
    osd_image(image_type t):m_image_type{t}{}
    virtual ~osd_image(){}
private:
    image_type m_image_type;

};

struct osd_bitmap : osd_image{
   
   osd_bitmap(std::string const & name_in,size_type const & size_in, image_type image_type_in = image_type::Bitmap) 
   : osd_image{image_type_in}
   ,m_name{name_in},
      m_size{size_in}, 
      m_data{size_in.x * size_in.y,colour::transparent}
   {}

   // box is new size overlaid on old assuming old is at 0,0
   // N.B bitmap is text mode
   // bottom is at the top of the box as dispalyed
   // and top is at bottom
   // ie top is a larger integer value than bottom
   bool resize(quan::two_d::box<int> const & new_size);
   
   size_type get_size() const { return m_size;}
   std::string const & get_name() const { return m_name;}
   void set_name (std::string const & name){ m_name = name;}
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
   :  osd_image{image_type::Bitmap}
      ,m_name{in.m_name}
      ,m_size{in.m_size}
      ,m_data{in.m_data}
   {}
   osd_bitmap & operator = (osd_bitmap const &) = delete;
   std::string m_name;
   size_type m_size;
   std::vector<colour> m_data;
};


wxBitmap* ConvertTo_wxBitmap(osd_image const& in, wxColour const * (&colours)[4]);
wxImage* ConvertTo_wxImage(osd_image const& in);
osd_bitmap* ConvertTo_osd_bitmap(std::string const & name,wxImage const& image);

#endif // OSD_BITMAP_MAKER_OSD_IMAGE_HPP_INCLUDED

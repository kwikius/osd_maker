#ifndef OSD_BITMAP_MAKER_OSD_IMAGE_HPP_INCLUDED
#define OSD_BITMAP_MAKER_OSD_IMAGE_HPP_INCLUDED

#include <vector>
#include <quan/two_d/vect.hpp>

struct osd_null_image;
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
    static osd_null_image* get_null_image(){return m_null_image;}
    // delete usually 
    virtual void destroy() = 0;
    osd_image(){}
protected:
    virtual ~osd_image(){}
private:
   static osd_null_image* m_null_image;
   friend void create_osd_null_image();
   // convert to wxbitmap or rebuild bitmap from data
   // convert to wximage or rebuild image from
   // save as ..
};

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

struct osd_null_image : osd_image{
   size_type get_size() const { return size_type{0,0};}
   bool get_pixel_colour( pos_type const & p, colour & c) const { c = colour::transparent; return true;}
   bool set_pixel_colour( pos_type const & p, colour c) { return false;}
   void destroy() { }
   private: 
      osd_null_image(){}
   friend void create_osd_null_image();
   
};

// virtual base for fonts and bitmap libs
// in practise either a font or an image_lib
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

#endif // OSD_BITMAP_MAKER_OSD_IMAGE_HPP_INCLUDED

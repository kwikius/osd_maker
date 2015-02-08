#ifndef OSD_BITMAP_MAKER_DYNAMIC_FONT_HPP_INCLUDED
#define OSD_BITMAP_MAKER_DYNAMIC_FONT_HPP_INCLUDED

#ifndef OSD_BITMPAP_MAKER_FONT_HPP_INCLUDED
#define OSD_BITMPAP_MAKER_FONT_HPP_INCLUDED


#include <vector>
#include <string>
#include <quan/uav/osd/dynamic/bitmap.hpp>
#include <quan/uav/osd/dynamic/font.hpp>
#include <cassert>

struct db_font : quan::uav::osd::dynamic::font{
    typedef quan::uav::osd::size_type size_type;
    typedef quan::uav::osd::dynamic::bitmap * dynamic_bitmap_ptr;
  // satisfy basic_font abc
   int32_t get_begin()const ;
   int32_t get_num_elements()const;
   int32_t get_char_height()const ;
   int32_t get_char_width() const ;
   // ~satisfy dynamic::font abc
   std::string const & get_name()const;// {return m_name;}
   // index of the value nb not zero index but char index
   bool get_handle_at( int i, int& out)const;
  // index of the value nb not zero index but char index
   bool set_handle_at(int i, int val);
   //returns nullptr!
   quan::uav::osd::basic_bitmap const * get_char_at(int32_t i)const ;
   void set_char_size(size_type const & size);
   db_font(std::string const & name_in, size_type size_in, int begin);
   ~db_font(){}
private:
   db_font & operator = ( db_font const &) = delete;
   db_font ( db_font const &) = delete;
   std::vector <int> m_elements;
   std::string m_name;
   size_type m_size;
   int m_begin;
};




#endif // OSD_BITMPAP_MAKER_FONT_HPP_INCLUDED



#endif
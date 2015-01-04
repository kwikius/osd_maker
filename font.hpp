#ifndef OSD_BITMPAP_MAKER_FONT_HPP_INCLUDED
#define OSD_BITMPAP_MAKER_FONT_HPP_INCLUDED


#include <vector>

#include "osd_image.hpp"

struct font{
   static constexpr int begin = 32 ;// ( 0x20); first ( space)
   static constexpr int end = 127; //( 0x7F) one after last
   font(std::string const & name_in, osd_image::size_type size_in)
   : m_name{name_in},m_size{size_in}
   {
     for ( auto v = begin; v < end; ++v){
         m_elements.push_back(-1);
     }
   }
   ~font(){}
   std::string const & get_name()const {return m_name;}
   bool get_handle_at( int i, int& out)
   {
      if ((i < begin) || ( i >=end)){
         return false;
      }
      out = m_elements.at( i - begin);
   }
   bool set_handle_at(int i, int val){ 
      if ((i < begin) || ( i >=end)){
         return false;
      }
      m_elements.at(i-begin) = val;
      return true;
   }
private:
   std::vector <int> m_elements;
   std::string m_name;
   osd_image::size_type m_size;
};

#endif // OSD_BITMPAP_MAKER_FONT_HPP_INCLUDED

#ifndef OSD_BITMPAP_MAKER_FONT_HPP_INCLUDED
#define OSD_BITMPAP_MAKER_FONT_HPP_INCLUDED


#include <vector>

#include "osd_image.hpp"

struct font{
   //static constexpr int begin = 32 ;// ( 0x20); first ( space)
   //static constexpr int end = 127; //( 0x7F) one after last
   font(std::string const & name_in, osd_image::size_type size_in, int begin)
   : m_name{name_in},m_size{size_in}, m_begin{begin}
   {
       assert( m_begin >= 0 && __LINE__);
   }
   int get_begin()const{return m_begin;}
   ~font(){}
   std::string const & get_name()const {return m_name;}
   // index of the value nb not zero index but char index
   bool get_handle_at( int i, int& out)const
   {
      if ((i < m_begin) || ( i >= (m_begin + static_cast<int>(m_elements.size())))){
         return false;
      }
      out = m_elements.at( i - m_begin);
      return true;
   }
  // index of the value nb not zero index but char index
   bool set_handle_at(int i, int val){ 
      int pos = i - m_begin;
      if ( pos < 0){
         return false;
      }
      while ( m_elements.size() <= static_cast<size_t>(pos)){
          m_elements.push_back(-1);  
      }
      m_elements.at(pos) = val;
      return true;
   }
   int get_num_elements()const {return static_cast<int>(m_elements.size());}
private:
   font & operator = ( font const &) = delete;
   font ( font const &) = delete;
   std::vector <int> m_elements;
   std::string m_name;
   osd_image::size_type m_size;
   int m_begin;
};

#endif // OSD_BITMPAP_MAKER_FONT_HPP_INCLUDED

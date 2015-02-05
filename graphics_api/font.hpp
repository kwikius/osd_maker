#ifndef OSD_BITMPAP_MAKER_FONT_HPP_INCLUDED
#define OSD_BITMPAP_MAKER_FONT_HPP_INCLUDED


#include <vector>
#include <quan/uav/osd/basic_font.hpp>

#include <quan/uav/osd/dynamic/bitmap.hpp>

struct font : quan::uav::osd::basic_font{
    typedef quan::uav::osd::size_type size_type;
    typedef quan::uav::osd::bitmap_ptr bitmap_ptr;

   font(std::string const & name_in, size_type size_in, int begin)
   : m_name{name_in},m_size{size_in}, m_begin{begin}
   {
       assert( m_begin >= 0 && __LINE__);
   }
  // satisfy abc
   int32_t get_begin()const{return m_begin;}
   int32_t get_num_elements()const {return m_elements.size();}
   int32_t get_char_height()const { return m_size.y;}
   int32_t get_char_width() const {return m_size.x;}
   bitmap_ptr       get_char_at(int32_t i)const ;
   // ~satisfy abc
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

   void set_char_size(size_type const & size){ m_size = size;}
private:
   font & operator = ( font const &) = delete;
   font ( font const &) = delete;
   std::vector <int> m_elements;
   std::string m_name;
   size_type m_size;
   int m_begin;
};

#endif // OSD_BITMPAP_MAKER_FONT_HPP_INCLUDED

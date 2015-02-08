#include  "db_font.hpp"
#include "../../osd_bmp_app.hpp"
#include "../../document.hpp"

int32_t db_font::get_begin()const{return m_begin;}
int32_t db_font::get_num_elements()const {return m_elements.size();}
int32_t db_font::get_char_height()const { return m_size.y;}
int32_t db_font::get_char_width() const {return m_size.x;}
void db_font::set_char_size(size_type const & size){ m_size = size;}
std::string const & db_font::get_name()const {return m_name;}

db_font::db_font(std::string const & name_in, size_type size_in, int begin)
: m_name{name_in},m_size{size_in}, m_begin{begin}
{
    assert( m_begin >= 0 && __LINE__);
}

quan::uav::osd::basic_bitmap const *  db_font::get_char_at(int32_t i)const 
{
   int char_handle =-1;
   if ( !this->get_handle_at(i,char_handle)){
      return nullptr;
   }
   return wxGetApp().get_document()->get_bitmap(char_handle);
}

bool db_font::get_handle_at( int i, int& out)const
{
   if ((i < m_begin) || ( i >= (m_begin + static_cast<int>(m_elements.size())))){
      return false;
   }
   out = m_elements.at( i - m_begin);
   return true;
}

bool db_font::set_handle_at(int i, int val){ 
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




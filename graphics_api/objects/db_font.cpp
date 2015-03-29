#include  "db_font.hpp"
#include "../../osd_bmp_app.hpp"
#include "../../document.hpp"
#include <quan/uav/osd/dynamic/bitmap.hpp>
#include <quan/conversion/itoa.hpp>

int32_t db_font::get_begin()const{return m_begin;}
bool db_font::set_begin(int32_t pos)
{
   if ( pos < m_begin){
      return false;
   }
   while ( m_begin < pos){
      if ( pop_front() == -1){
         return false;
      }
   }
   return true;
}

int db_font::pop_front()
{
   int result = -1;
   if (m_elements.size() > 1){
      result = m_elements[0];
      m_elements.erase(m_elements.begin());
      ++m_begin;
      return result;
   }else{
      return -1;
   }
}
int db_font::pop_back()
{
   int result = -1;
   if (m_elements.size() > 1){
      result = m_elements.back();
      m_elements.pop_back();
      return result;
   }else{
      return -1;
   }
}
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
  // assert( m_begin >= 0 && __LINE__);
   if ((i < m_begin) || ( i >= (m_begin + static_cast<int>(m_elements.size())))){
      return false;
   }
   out = m_elements.at( i - m_begin);
   return true;
}

bool db_font::set_handle_at(int i, int val){ 
     // assert( m_begin >= 0 && __LINE__);
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

namespace {

 //const char* name_map[256] = {0};
//   char wanted[] =
//      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//      "abcdefghijklmnopqrstuvwxyz"
//      "1234567890"
//      "!\"$ %^&*()_-+={}[]:;'@#~|\\,<>.?/`";


//  std::string get_char_name fun(char i)
//  {
//     if (( i >= 'A') && ( i <= ';')){
//         return wanted[i];
//     }else{
//         return "no char";
//     }
//  }

}

void db_font::output_header( std::string const & type_name, std::string const & object_name,std::ostream & out)const
{
   assert( m_begin >= 0 && __LINE__);
   int32_t const begin = get_begin();
   int32_t end = begin + get_num_elements();
   std::string font_name = this->get_name();

   std::string font_array_elements;
   for ( int32_t i = begin; i < end; ++i){
      // output char
      quan::uav::osd::basic_bitmap const *  bmp = this->get_char_at(i);
      if ( bmp){
         auto dynbmp 
         = dynamic_cast<quan::uav::osd::dynamic::bitmap const *>(bmp);
         if ( dynbmp){
             char buf[sizeof(int)*8+2];
             std::string element_name = "font_" + font_name + std::string{"_"} + quan::itoasc(i,buf,10);
             dynbmp->output_header(element_name + "_type",element_name,out);
             if ( i == begin){
               font_array_elements += "      ";
             }else{
               font_array_elements += "\n      ,";
             }
             font_array_elements += "&" + element_name;
             
         }else{
            // fail else array access will be out!!!!
         }
      }
   }
   out << " constexpr quan::uav::osd::bitmap_ptr font_" << font_name 
            << "_char_array[" << this->get_num_elements() << "] = {\n";
   out << font_array_elements;
   out << " };\n\n";    

   out << " struct font_" << font_name  << "_type : quan::uav::osd::basic_font{\n";
   out << "    int32_t  get_begin() const {return " << get_begin() << ";}\n";
   out << "    int32_t  get_char_height() const { return " << get_char_height() << ";}\n";
   out << "    int32_t  get_char_width() const { return " << get_char_width() << ";}\n";
   out << "    int32_t  get_num_elements() const {return " << get_num_elements() << ";}\n";
   out << "    quan::uav::osd::basic_bitmap const * get_char_at(int32_t i)const\n";
   out << "    {\n";
   out << "       int32_t const array_pos = i - get_begin();\n";
   out << "       if ((array_pos >= 0) && (array_pos < " << get_num_elements() << ")){\n";
   out << "          return font_" << font_name << "_char_array[array_pos];\n";
   out << "       }else{\n";
   out << "          return nullptr;\n";
   out << "       }\n";
   out << "    }\n";
   out << " } font_" << font_name << ";\n";
}




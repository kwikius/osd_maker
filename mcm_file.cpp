
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct mcm_file{
   static bool upload_font (std::istream & in,std::vector <std::vector<uint8_t> > & chars_vect);
   static bool line_string_to_val (std::string const & str, uint8_t & out);
};

bool mcm_file::upload_font (std::istream & in,std::vector <std::vector<uint8_t> > & chars_vect)
{
   std::string str;
   std::getline (in,str);
   int count = 0;
   while (in && ! in.eof()) {
      std::vector<uint8_t>  char_vect;
      for (uint32_t i = 0; i < 64; ++i) {
         std::getline (in,str);
         uint8_t out = 0;
         if (mcm_file::line_string_to_val (str,out)) {
            char_vect.push_back (out);
         }else{
           // std::cout << "mcm string to val failed\n";
            return false;
         }
      }
      chars_vect.push_back (char_vect);
      // std::cout << "char " << count << " done\n";
      if (++count == 256) {
        // std::cout << "all chars done\n";
         return true;
      }
   }
  // std::cout << "not got all chars\n";
   return false;
}

bool mcm_file::line_string_to_val (std::string const & str, uint8_t & out)
{
   if ( str.length() < 8){
      return false;
   }
   out = 0;
   for (uint8_t j = 0; j < 8; ++j) {
      switch (str.at (j)) {
      case '0':
         break;
      case '1':
         out  |= (1 << j);
         break;
      default:
         return false;
      }
   }
   return true;
}

void  process_mcm_char (uint8_t inval, std::ostream & out)
{
   for (uint8_t j = 0; j < 8; j+=2) {
      uint8_t code = (inval >> j) & 0b11;
      switch (code) {
      case  0b00: // black
         out << "#";
         break;
      case  0b01: // white
         out << " ";
         break;
      case  0b10: // transparent
      case  0b11: 
         out << "*" ;
         break;
      }
   }
}

void output_mcm_ascii_art( uint8_t ch, std::vector <std::vector<uint8_t> > const & chars_vect, std::ostream & out)
{
   std::vector<uint8_t> const & v = chars_vect.at(ch);
   uint32_t iter = 0;
   for ( uint32_t line = 0; line < 18 ; ++line){
      for ( uint32_t row_elem = 0; row_elem < 3; ++row_elem){
         process_mcm_char(v.at(iter),out);
         ++iter;
      }
      out <<'\n';
   }
}

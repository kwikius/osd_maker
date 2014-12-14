#ifndef QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED
#define QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED

#include <quan/two_d/vect.hpp>
#include <quan/three_d/vect.hpp>
#include <quan/length.hpp>
#include <vector>

struct document{
   document();

  // quan::uav::position const & get_aircraft_position()const { return m_aircraft_position;}
   quan::two_d::vect<quan::length::mm> const & 
   get_map_size() const {return m_map_size;}
  // void  set_aircraft_position(quan::uav::position const & pos){ m_aircraft_position = pos;}
   void set_map_size( quan::two_d::vect<quan::length::mm> const & size)
   { m_map_size = size;}

   quan::two_d::vect<uint32_t> const & get_bitmap_size()const
   {
      return m_bitmap_size;
   }
   quan::two_d::vect<quan::length::mm> const & get_pixel_size_mm()const
   {
      return m_pixel_size;
   }
   bool get_pixel_colour(quan::two_d::vect<int> const & pos,  uint8_t & colour)
   {
      if ( colour > 3){
         return false;
      }
      if (( pos.x <0) || (pos.y < 0)){
         return false;
      }
      if ( (static_cast<uint32_t>(pos.x) < m_bitmap_size.x) 
         && (static_cast<uint32_t>(pos.y) < m_bitmap_size.y)
      ){
         uint32_t ar_pos = pos.y * m_bitmap_size.x + pos.x;
         colour = pixel_array.at(ar_pos) ;
         return true;
      }else{
         return false;
      }
   }
   bool set_pixel_colour(quan::two_d::vect<int> const & pos, uint8_t colour)
   {
      if ( colour > 3){
         return false;
      }
      if (( pos.x <0) || (pos.y < 0)){
         return false;
      }
      if ( (static_cast<uint32_t>(pos.x) < m_bitmap_size.x) 
         && (static_cast<uint32_t>(pos.y) < m_bitmap_size.y)
      ){
         uint32_t ar_pos = pos.y * m_bitmap_size.x + pos.x;
         pixel_array.at(ar_pos) = colour;
         return true;
      }else{
         return false;
      }
   }
   static constexpr uint8_t grey = 0;
   static constexpr uint8_t black = 1;
   static constexpr uint8_t white = 2;
   static constexpr uint8_t transparent = 3;
   private:

   quan::two_d::vect<quan::length::mm> m_map_size;
   quan::length::mm  m_click_bucket;
   quan::two_d::vect<uint32_t> m_bitmap_size; //{12,18};
   quan::two_d::vect<quan::length::mm> m_pixel_size; // {mm{10},mm{10}};

   std::vector<uint8_t> pixel_array;
};

#endif // QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED


#ifndef OSD_BITMAP_MAKER_AIRCRAFT_HPP_INCLUDED
#define OSD_BITMAP_MAKER_AIRCRAFT_HPP_INCLUDED

/*
 Copyright (c) 2012 - 2015 Andy Little 

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>
*/

#include <quan/time.hpp>
#include <quan/length.hpp>
#include <quan/velocity.hpp>
#include <quan/voltage.hpp>
#include <quan/current.hpp>
#include <quan/charge.hpp>
#include <quan/angle.hpp>
#include <quan/uav/position.hpp>
#include <quan/uav/attitude.hpp>

struct aircraft{
   typedef quan::angle_<int32_t>::deg10e7                   lat_lon_type;
   typedef quan::length_<float>::mm                         altitude_type;
   typedef quan::uav::position<lat_lon_type,altitude_type>  position_type;

   position_type const & get_position() const { return m_position;}
   position_type const & get_home_position() const { return m_home_position;}
   quan::uav::attitude<float> const & get_attitude() const { return m_attitude;}
   quan::angle_<float>::deg const & get_heading()const { return m_heading;}
   quan::velocity_<float>::m_per_s const & get_airspeed()const { return m_airspeed;}
   quan::velocity_<float>::m_per_s const & get_groundspeed()const { return m_groundspeed;}
   // bool hav_airspeed
   void set_heading(quan::angle_<float>::deg const & value_in)
   {
      auto value = value_in;
      while( value > quan::angle::deg{360}){
         value -= quan::angle::deg{360};
      }
      while( value < quan::angle::deg{0}){
         value += quan::angle::deg{360};
      }
      this->m_heading = value;
   }
   private:
   position_type                       m_position;
   position_type                       m_home_position;
   quan::uav::attitude<float>          m_attitude;
   quan::angle_<float>::deg            m_heading;              
   quan::velocity_<float>::m_per_s     m_airspeed;            
   quan::velocity_<float>::m_per_s     m_groundspeed;             
  public:               
   aircraft():
      m_heading{0}
      ,m_airspeed{0}
      ,m_groundspeed{0}
   {}

};

extern aircraft the_aircraft;

#endif // OSD_BITMAP_MAKER_AIRCRAFT_HPP_INCLUDED

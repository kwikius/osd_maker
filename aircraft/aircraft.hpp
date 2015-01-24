
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
//
//struct location_t{
//
// //  quan::time_<uint64_t>::us                             gps_time_stamp;
//   quan::angle_<int32_t>::deg10e7                        gps_lat;  
//   quan::angle_<int32_t>::deg10e7                        gps_lon; 
//  // quan::velocity_<uint16_t>::cm_per_s                   gps_vog;  
// //  quan::angle_<uint16_t>::cdeg                          gps_cog;
//   quan::length_<int32_t>::mm                            gps_alt;  
// //  quan::length_<uint16_t>::cm                           gps_hdop;
// //  quan::length_<uint16_t>::cm                           gps_vdop; 
// };


/*
struct gps_t{
   uint8_t num_sats;
   uint8_t fix_type;
   bool  has_home;
};
*/

/*
See http://gentlenav/googlecode.com/files/DCMdraft2.pdf 

"The orientation of the body frame with respect to the fixed earth system
can be determined in the following manner. Imagine the aircraft to be 
positioned so that the body axis system is parallel to the fixed frame
and then apply the following rotations:
1. Rotate the body about its zbody axis through the yaw angle thetaZ
2. Rotate the body about its ybody axis through the pitch angle thetaY
3. Rotate the body about its xbody axis through the roll angle thetaX 

The rotation matrix R =
 [ 
  cos(thetaY) * cos(thetaZ) , 
      sin(thetaX) * sin(thetaY) * cos(thetaZ) - cos(thetaX) * sin(thetaZ),
         cos(thetaX) * sin(thetaY) * cos(thetaZ) - sin(thetaX) * sin(thetaZ)
   cos(thetaY) * sin(thetaZ),
      sin(thetaX) * sin(thetaY) * sin(thetaZ) + cos(thetaX) * cos(thetaZ),
         cos(thetaX) * sin(thetaY) * sin(thetaZ) - sin(thetaX) * cos(thetaZ)
   -sin(thetaY),
      sin(thetaX) * cos(thetaY),
         cos(thetaX) * cos(thetaY)
]
*/

struct attitude_t{
   
   quan::angle_<float>::deg            pitch;                  
   quan::angle_<float>::deg            roll;                   
   quan::angle_<float>::deg            yaw; 
   attitude_t():pitch{0},roll{0},yaw{0}{}
};

struct battery{
   battery(std::string const & name,quan::charge_<float>::mA_h const & capacity)
   : m_name{name}, m_capacity{capacity}
   {}
   std::string const & get_name() const{return m_name;}
   quan::voltage_<float>::V get_voltage() const { return m_voltage;}
   quan::current::A get_current() const { return m_current;}
   quan::charge::mA_h get_charge_remaining() const{ return m_charge_remaining;}
   quan::charge::mA_h get_capacity()const{return m_capacity;}
   
   private:
   std::string                         m_name;
   quan::voltage_<float>::V            m_voltage;
   quan::current_<float>::A            m_current;
   quan::charge_<float>::mA_h          m_charge_remaining;
   quan::charge_<float>::mA_h const    m_capacity;
};

struct aircraft{
   typedef quan::angle_<int32_t>::deg10e7 lat_lon_type;
   typedef quan::length_<float>::mm        altitude_type;
   typedef quan::uav::position<lat_lon_type,altitude_type> position_type;

   position_type const & get_position() const { return m_position;}
   position_type const & get_home_position() const { return m_home_position;}
   attitude_t const & get_attitude() const { return m_attitude;}
   private:
         
   position_type                       m_position;
   position_type                       m_home_position;
   attitude_t                          m_attitude;
     
   quan::angle_<float>::deg            heading;              
   quan::velocity_<float>::m_per_s     airspeed;            
   quan::velocity_<float>::m_per_s     groundspeed;

      // ADD rc output values              
  public:               
   aircraft():
     // throttle{0},
      heading{0}
      ,airspeed{0}
      ,groundspeed{0}

   {}

   private:

};

extern aircraft the_aircraft;

#endif // OSD_BITMAP_MAKER_AIRCRAFT_HPP_INCLUDED

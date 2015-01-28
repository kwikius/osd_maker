
#include <quan/two_d/rotation.hpp>
#include "../aircraft/aircraft.hpp"
#include "display.hpp"
//#include "../view.hpp"
//#include "../app.h"

/*
See http://gentlenav/googlecode.com/files/DCMdraft2.pdf 
1. Rotate the body about its zbody axis through the yaw angle psi
2. Rotate the body about its ybody axis through the pitch angle theta
3. Rotate the body about its xbody axis through the roll angle phi 

The rotation matrix R =
 [ cos(theta) * cos(psi) , 
      sin(phi) * sin(theta) * cos(psi) - cos(phi) * sin(psi),
         cos(phi) * sin(theta) * cos(psi) - sin(phi) * sin(psi)
   cos(theta) * sin(psi),
      sin(phi) * sin(theta) * sin(psi) + cos(phi) * cos(psi),
         cos(phi) * sin(theta) * sin(psi) - sin(phi) * cos(psi)
   -sin(theta),
      sin(phi) * cos(theta),
         cos(phi) * cos(theta)
]
// if yaw == 0 then psi == 0
// sin(psi) == sin(0) ==  0
// cos(psi) == cos(0) ==  1 
The rotation matrix R =
 [ cos(theta), sin(phi) * sin(theta), cos(phi) * sin(theta)  
   0, cos(phi), - sin(phi) ,
   -sin(theta), sin(phi) * cos(theta),cos(phi) * cos(theta)
]
Perspective transform
( If the horizon is on screen then no need for perspective transformation

*/

void draw_artificial_horizon( display_layout & d, attitude_t const & attitude)
{
   constexpr int width = 127;
   constexpr int outer_h_bar_len = 16;
   constexpr int outer_stop_height = 8;
   int centre_rad = 5;
   typedef display_layout::pxp pxp_t;
   typedef display_layout::colour colour;
   pxp_t left_end{-width/2,0};
   pxp_t right_end{width/2,0};

   quan::two_d::rotation rotate{quan::angle::deg{15}};

   for ( int i = -1; i < 2; ++i){
      const colour c = (i)?colour::black:colour::white;
      d.line_out(rotate(left_end + pxp_t{1,i}), rotate(left_end + pxp_t{outer_h_bar_len ,i}), c);
      d.line_out(rotate(right_end+ pxp_t{0,i}), rotate(right_end + pxp_t{-outer_h_bar_len,i}),c);
      d.line_out(rotate(pxp_t{left_end.x - i,outer_stop_height/2}),rotate(pxp_t{left_end.x - i,-outer_stop_height/2}),c);
      d.line_out(rotate(pxp_t{right_end.x +i,outer_stop_height/2}),rotate(pxp_t{right_end.x +i,-outer_stop_height/2}),c);
      d.circle1_out({0,0},centre_rad+i,c);
      d.line_out(rotate(pxp_t{-20,i}),rotate(pxp_t{-centre_rad -1 ,i}), c);
      d.line_out(rotate(pxp_t{20,i}),rotate(pxp_t{centre_rad +1,i}), c); 
   }
}

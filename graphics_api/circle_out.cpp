#include "../display_layout.hpp"


void display_layout::circle_out(pxp const & centre, int radius, int numsegs, colour c)
{
   auto angle_inc =  (quan::angle::deg{360})/numsegs;
   //pxp offset = m_origin; // to do add centre
   for ( auto i = 0; i < numsegs; ++i){
         auto angle1 =  angle_inc * i;
         auto angle2 =  angle1 + angle_inc;
         auto c1 = cos(angle1);
         auto s1 = sin(angle1);
         auto c2 = cos(angle2);
         auto s2 = sin(angle2);
         pxp const from{c1 * radius,s1 * radius};
         pxp const to {c2 * radius, s2 * radius};
         line_out(from + centre ,to + centre,c);
   }
  
}
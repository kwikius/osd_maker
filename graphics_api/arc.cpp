
#include "../display_layout.hpp"

/*
normalised arc end angle > start angle
*/
void display_layout::normalised_arc(pxp const & centre, uint32_t radius, quan::angle::deg const & start_angle,
      quan::angle::deg const & end_angle, uint32_t numsegs, colour c)
{
   auto angle_inc =  (end_angle - start_angle )/numsegs;
   //pxp offset = m_origin; // to do add centre
   for ( auto i = 0U; i < numsegs; ++i){
         auto angle1 = start_angle + angle_inc * i;
         auto angle2 = angle1 + angle_inc;
         auto c1 = cos(angle1);
         auto s1 = sin(angle1);
         auto c2 = cos(angle2);
         auto s2 = sin(angle2);
         pxp const from{c1 * radius +0.5,s1 * radius+0.5};
         pxp const to {c2 * radius+0.5, s2 * radius+0.5};
         line_out(from + centre ,to + centre,c);
   }
}

namespace {

   quan::angle::deg normalise_angle ( quan::angle::deg const & angle_in)
   {
      auto angle = angle_in;
      while ( angle >= quan::angle::deg{360}){
            angle -= quan::angle::deg{360};
      }
      while (angle < quan::angle::deg{0}){
          angle += quan::angle::deg{360};
      }
      return angle;
   }
}

void display_layout::arc_out(pxp const & centre, uint32_t radius, quan::angle::deg const & start_angle_in,
      quan::angle::deg const & end_angle_in, uint32_t numsegs, colour c)
{
      auto start_angle = normalise_angle(start_angle_in);
      auto end_angle = normalise_angle(end_angle_in);
      if ( end_angle > start_angle){
         normalised_arc(centre,radius,start_angle, end_angle,numsegs, c);
      }else{
         auto first_angle = quan::angle::deg{360} - start_angle;
         auto first_segs = (numsegs * first_angle) / (first_angle + end_angle);
         auto last_segs = numsegs - first_segs;
         normalised_arc(centre,radius, start_angle, quan::angle::deg{360}, first_segs,c);
         normalised_arc(centre,radius, quan::angle::deg{0},end_angle, last_segs,c);
      }
}    





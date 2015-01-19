
#include <quan/two_d/clip.hpp>
#include <cmath>
#include <utility>
#include "../display_layout.hpp"

void display_layout::line_out(pxp const & p0_in_full, pxp const & p1_in_full, colour c )
{

   // clip box should be transfomed?
   //clip_result_type clip_result = m_clip({transform_to_raw(p0_in_full),transform_to_raw(p1_in_full)});
   clip_result_type clip_result = m_clip({p0_in_full,p1_in_full});
   if (!clip_result.first){
      return;
   }
   auto  p0 = transform_to_raw(clip_result.second.from) ;
   auto  p1 = transform_to_raw(clip_result.second.to);

	bool const steep = abs(p1.y - p0.y) > abs(p1.x - p0.x);

   if (steep){
      std::swap(p0.x,p0.y);
      std::swap(p1.x,p1.y);
   }

   if ( p0.x > p1.x){
        std::swap(p1,p0);
   }

	int const deltax = p1.x - p0.x;
	int const deltay = abs(p1.y - p0.y);
	float error = deltax / 2.f;
	int32_t const ystep = (p0.y < p1.y)?1:-1;
	int32_t y = p0.y ;

	for (int32_t x = p0.x ;x < p1.x ; ++x) {
		if (!steep) {
// can use ll_set_pixel here ( unchecked)
         set_pixel({x,y},c);
         // asm volatile ("nop":::);
      }else{
			set_pixel({y,x},c);
          // asm volatile ("nop":::);
		}
		error -= deltay;
		if (error < 0.0) {
			y     += ystep;
			error += deltax;
		}
	}
}

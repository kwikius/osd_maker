
#include <quan/two_d/clip.hpp>
#include <cmath>
#include <utility>
#include "../display_layout.hpp"

void display_layout::line_out(pxp const & p0_in_full, pxp const & p1_in_full, colour c )
{
#if 1
   clip_result_type clip_result = m_clip({m_origin + p0_in_full,m_origin + p1_in_full});
   if (!clip_result.first){
      return;
   }
   auto  p0 = clip_result.second.from;
   auto  p1 = clip_result.second.to;

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
#else

// in members.chell.at/~easyfilter/bresenham.html
void display_layout::line_out(pxp const & p0_in, pxp const & p1_in, colour c )
{

	// Based on http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	bool const steep = fabs(p1_in.y - p0_in.y) > fabs(p1_in.x - p0_in.x);

   quan::two_d::vect<float> p0;
   quan::two_d::vect<float> p1;
	if (!steep) {
      p0 = p0_in;
      p1 = p1_in;
   }else{
       p0 = quan::two_d::vect<float>{  p0_in.y,p0_in.x};
       p1 = quan::two_d::vect<float>{  p1_in.y,p1_in.x};
   }

   if ( p0.x > p1.x){
      quan::two_d::vect<float> const tmp = p0;
      p0 = p1;
      p1 = tmp;
   }

	float const deltax = p1.x - p0.x;
	float const deltay = fabs(p1.y - p0.y);
	float error = deltax / 2.f;
	int32_t const ystep = (p0.y < p1.y)?1:-1;
	int32_t y = static_cast<int32_t>(p0.y + 0.5f);

	for (int32_t x = static_cast<int32_t>(p0.x +0.5f), end = static_cast<int32_t>(p1.x + 0.5f);x < end ; ++x) {
		if (!steep) {
         set_pixel({x,y},c);
          asm volatile ("nop":::);
      }else{
			set_pixel({y,x},c);
           asm volatile ("nop":::);
		}
		error -= deltay;
		if (error < 0.f) {
			y     += ystep;
			error += deltax;
		}
	}
}
#endif


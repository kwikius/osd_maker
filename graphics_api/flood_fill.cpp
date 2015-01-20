#include "../display_layout.hpp"

#include <stack>

void display_layout::flood_fill(pxp const & start_pos, colour new_colour)
{
   //assert ( new_colour == colour::black && __LINE__);
   pxp raw_pxp = transform_to_raw(start_pos);
   if ( get_pixel(raw_pxp) == new_colour){
      return;
   } 
   
   auto image_size = get_display_size();

   assert(raw_pxp.x >= 0) ;
   assert(raw_pxp.x < image_size.x) ;

   assert(raw_pxp.y >= 0) ;
   assert(raw_pxp.y < image_size.y) ;
   
   std::stack<pxp> stack;
   assert(stack.empty());
   stack.push(raw_pxp);
   int count = 0;
   while (! stack.empty()){
      ++ count;
      //assert( (count < 1000) && __LINE__);
      if ( count > 5000){
         break;
      }
      auto n = stack.top();
     // assert ( (n == raw_pxp) && __LINE__);
      stack.pop();
      set_pixel(n,new_colour);

      if ( n.x > 0){
         auto west = pxp {n.x-1,n.y};
         if ( get_pixel(west) != new_colour){
            stack.push(west);
         }
      } 
      if ( n.x < static_cast<int>((image_size.x -1))){
         auto east = pxp {n.x+1,n.y};
         if ( get_pixel(east) != new_colour){
            stack.push(east);
         }
      }
      if ( n.y > 0){
         auto north = pxp {n.x,n.y -1};
         if ( get_pixel(north) != new_colour){
            stack.push(north);
         }
      }
      if( n.y < static_cast<int>((image_size.y -1))){
       auto south = pxp {n.x,n.y+1};
       if ( get_pixel(south) != new_colour){
            stack.push(south);
       }
      }
   }
  
}
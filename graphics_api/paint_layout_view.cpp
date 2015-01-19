
#include "../view.hpp"

void view::paint_layout_view(wxPaintEvent & event)
{
     m_display_layout.clear();
//     if( this->have_image()){
//         m_display_layout.bitmap_out({100,100},m_current_image);
//     }
     auto f = this->get_current_font();
     if( f != nullptr){
         m_display_layout.text_out({100,100},"HELLO WORLD",f);
     }
     m_display_layout.set_origin( m_display_layout.get_display_size()/2);
     m_display_layout.line_out({-100,-100},{-50,-50},display_layout::colour::white);
     m_display_layout.line_out({100,100},{50,50},display_layout::colour::white);
     m_display_layout.line_out({-50,50},{-100,100},display_layout::colour::black);
     m_display_layout.line_out({50,-50},{100,-100},display_layout::colour::black);

     m_display_layout.arc_out(100,quan::angle::deg{150},quan::angle::deg{30},20,display_layout::colour::black);
     m_display_layout.arc_out(99,quan::angle::deg{150},quan::angle::deg{30},20,display_layout::colour::white);
    // m_display_layout.arc_out(98,quan::angle::deg{150},quan::angle::deg{30},20,display_layout::colour::white);
    // m_display_layout.arc_out(97,quan::angle::deg{150},quan::angle::deg{30},20,display_layout::colour::black);
     auto image_size = m_display_layout.get_display_size();
    // m_display_layout.rescale(image_size*2);

     wxBitmap bitmap{m_display_layout.get_image()};
     wxPaintDC dc(this);
     dc.SetBackground(* wxBLACK_BRUSH); 
     dc.Clear();
     dc.DrawBitmap(bitmap,0,0);
}

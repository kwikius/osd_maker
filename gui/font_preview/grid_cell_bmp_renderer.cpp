
#include "../../osd_bmp_app.hpp"
#include "../../document.hpp"

#include "../../graphics_api/font.hpp"

#include "../font_preview.hpp"

#include "grid_cell_bmp_renderer.hpp"


grid_cell_bmp_renderer::grid_cell_bmp_renderer()
{}

wxSize grid_cell_bmp_renderer::GetBestSize(wxGrid & grid, wxGridCellAttr& attr, wxDC & dc, int row, int col)
{
   // want to work out font size here ?
   return {14,18};
}

wxGridCellRenderer* grid_cell_bmp_renderer::Clone() const
{
   return new grid_cell_bmp_renderer{};
}

namespace {
 wxColour const * colour_array[4] = {wxLIGHT_GREY,wxBLACK,wxWHITE,wxBLUE};

}

void grid_cell_bmp_renderer::Draw( wxGrid & grid,
      wxGridCellAttr& attr,
      wxDC& dc, 
      wxRect const & rect, 
      int row, 
      int col, 
      bool is_selected
   )
{
  // int num_rows = grid.GetNumberRows();
   int num_cols = grid.GetNumberCols();
   int ascii_char = row * num_cols + col;

   font* selected_font = wxGetApp().get_font_preview()->get_font();

   if (selected_font){
      int bmp_handle = -1;
      if (selected_font->get_handle_at(ascii_char, bmp_handle)){
         // get th bitmap at the handle
         osd_image* image = wxGetApp().get_document()->get_image(bmp_handle);
         if ( image){
            
            // convert to a bitmap
            auto bmp = ConvertTo_wxBitmap(*image, colour_array);
            wxImage image = bmp->ConvertToImage();
            wxSize is {image.GetWidth(),image.GetHeight()};
            image.Rescale(is.x * 2, is.y * 2);
            delete bmp;
            wxBitmap  bmp1 (image);
            dc.DrawBitmap(bmp1,rect.x,rect.y);
            
            return;
         }
      }
   }
   // if here something not available so just draw the char text
   char ch = static_cast<char>(ascii_char);
   dc.DrawText(wxString::Format(wxT("%c"),ch), rect.x,rect.y);
}







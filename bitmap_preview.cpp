//#include <cctype>

#include <wx/bitmap.h>
#include <wx/rawbmp.h>
#include <quan/max.hpp>
#include "app.h"
#include "osd_image.hpp"
#include "document.hpp"
#include "view.hpp"
#include "bitmap_preview.hpp"
#include "font.hpp"
#include "grid_cell_bmp_renderer.hpp"

void bitmap_preview::reset()
{
   m_current_font_handle = -1;
}

void bitmap_preview::set_font_handle(int font_handle)
{
   m_current_font_handle = font_handle;
   this->Refresh();
}

font * bitmap_preview::get_font()const
{
   if (m_current_font_handle == -1){
      return nullptr;
   }

   return wxGetApp().get_document()->get_font(m_current_font_handle);
}

BEGIN_EVENT_TABLE(bitmap_preview,wxWindow)

       EVT_GRID_CMD_CELL_LEFT_DCLICK(idGrid,bitmap_preview::OnGridCellLeftDblClick)
       EVT_GRID_CMD_CELL_RIGHT_CLICK(idGrid,bitmap_preview::OnGridCellRightClick)
END_EVENT_TABLE()

void bitmap_preview::OnGridCellRightClick(wxGridEvent& event)
{
   wxMessageBox(wxT("Grid Right Click"));
   /*
      menu
      export to bitmap
   */
}

void bitmap_preview::OnGridCellLeftDblClick(wxGridEvent& event)
{
   font* selected_font = this->get_font();
   if (selected_font == nullptr){
      return;
   }
   int const x = event.GetCol();
   int const y = event.GetRow();

  int const num_elements = 256;//selected_font->get_num_elements();
  int const num_rows = 8;
  int const rem = ((num_elements % num_rows)==0)?0:1;
  int const num_cols = (num_elements / num_rows) + rem;
  int const font_pos = num_cols * y + x;
  int const ch = font_pos ;//+ selected_font->get_begin();
  int event_handle = -1;
  if (! selected_font->get_handle_at(ch, event_handle)){
      wxMessageBox(wxT("Failed to get font image handle"));
      return ;
   }
   if ( event_handle == -1){
    wxMessageBox(wxT("Invalid font element handle"));
      return ;
   }
   wxGetApp().get_view()->sync_with_image_handle(event_handle);
}

bitmap_preview::bitmap_preview(wxWindow* parent)
     : wxWindow{parent, wxID_ANY},m_grid{nullptr},m_current_font_handle{-1}
{
  wxSize cell_size{14 *2,18 *2};
  int num_elements = 256;
  int const num_rows = 8;
  int const rem = ((num_elements % num_rows)==0)?0:1;
  int const num_cols = (num_elements / num_rows) + rem;
  wxSize grid_size{num_cols * cell_size.x,num_rows * cell_size.y};
  this->m_grid = new wxGrid{this,idGrid,wxPoint{0,0},grid_size*2};
  m_grid->CreateGrid(num_rows,num_cols);
  m_grid->SetDefaultColSize(cell_size.x);
  m_grid->SetDefaultRowSize(cell_size.y);
  m_grid->SetRowLabelSize(0);
  m_grid->SetColLabelSize(0);
  m_grid->DisableDragGridSize();
  auto renderer = new grid_cell_bmp_renderer{};
  m_grid->SetDefaultRenderer(renderer);
  for ( int y = 0; y < num_rows; ++y){
    for ( int x = 0; x < num_cols; ++x){
      int ar_pos = y * num_cols + x;
      if ( ar_pos < num_elements){
            char ch = static_cast<char>(ar_pos);
            m_grid->SetCellValue(y,x,wxString::Format(wxT("%c"),ch));
            m_grid->SetReadOnly(y,x);
      }
    }
  }  
  this->Refresh();    
}

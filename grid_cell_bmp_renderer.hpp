#ifndef GRID_CELL_BMP_RENDERER_HPP_INCLUDED
#define GRID_CELL_BMP_RENDERER_HPP_INCLUDED

#include <wx/grid.h>

struct grid_cell_bmp_renderer : wxGridCellRenderer{

   grid_cell_bmp_renderer( );
   void Draw( wxGrid & grid,
      wxGridCellAttr& attr,
      wxDC& dc, 
      wxRect const & rect, 
      int row, int col, 
      bool is_selected
   );
   wxSize GetBestSize(wxGrid & grid, wxGridCellAttr& attr, wxDC & dc, int row, int col);
   wxGridCellRenderer* Clone() const;

};

#endif // GRID_CELL_BMP_RENDERER_HPP_INCLUDED

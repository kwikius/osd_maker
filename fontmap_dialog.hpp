#ifndef QUAN_OSD_BITMAP_MAKER_FONTMAP_DIALOG_HPP_INCLUDED
#define QUAN_OSD_BITMAP_MAKER_FONTMAP_DIALOG_HPP_INCLUDED

#include <wx/dialog.h>

struct fontmap_dialog : wxDialog
{
   fontmap_dialog(wxWindow* parent)
   : wxDialog(parent,-1,wxT("Font Dialog")
      ,wxDefaultPosition
      ,wxDefaultSize
      ,wxDEFAULT_DIALOG_STYLE
      )
   {}

  // read the doc and display the font
  // or do a file open if no font
  // open mcm file and display the font?
  // open fontdialog and get the font
};


#endif // QUAN_OSD_BITMAP_MAKER_FONTMAP_DIALOG_HPP_INCLUDED

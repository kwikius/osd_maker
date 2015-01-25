#ifndef NEW_BITMAP_DIALOG_HPP_INCLUDED
#define NEW_BITMAP_DIALOG_HPP_INCLUDED

#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/stattext.h>
/*
modal dialog with OK cancel

size of bitmap in x, y
 name of bitmap, name must be unique in bitmap tree
*/
struct new_bitmap_dialog : wxDialog{

   enum
   {
      idNewBitmapDialog = 1100,
      idXSize,
      idYSize,
      idName
   };

   wxTextCtrl * m_name_text;
   wxTextCtrl * m_x_text;
   wxTextCtrl * m_y_text;

   wxButton* ok_button;
   wxButton* cancel_button;
   
   new_bitmap_dialog(wxWindow * parent);
};
#endif // NEW_BITMAP_DIALOG_HPP_INCLUDED

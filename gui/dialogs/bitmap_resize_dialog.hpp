#ifndef OSD_BITMAP_MAKER_BITMAP_RESIZE_DIALOG_HPP_INCLUDED
#define OSD_BITMAP_MAKER_BITMAP_RESIZE_DIALOG_HPP_INCLUDED

#include <wx/dialog.h>
#include <wx/textctrl.h>
//#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/stattext.h>

struct bitmap_resize_dialog : wxDialog{

   enum {
      idResizeBitmapDialog = 1100,
      idLeftIncr,
      idRightIncr,
      idTopIncr,
      idBottomIncr
   };

   wxTextCtrl * m_left_incr;
   wxTextCtrl * m_right_incr;
   wxTextCtrl * m_top_incr;
   wxTextCtrl * m_bottom_incr;

   wxButton* ok_button;
   wxButton* cancel_button;
   
   bitmap_resize_dialog(wxWindow * parent);
};

#endif // OSD_BITMAP_MAKER_BITMAP_RESIZE_DIALOG_HPP_INCLUDED

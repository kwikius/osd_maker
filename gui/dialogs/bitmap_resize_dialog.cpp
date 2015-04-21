
#include "bitmap_resize_dialog.hpp"

   bitmap_resize_dialog::bitmap_resize_dialog(wxWindow * parent, 
      wxString const & text) 
   : wxDialog{
      parent, 
      idResizeBitmapDialog,
      text,
      wxDefaultPosition,
      wxSize{400,200}
   }
   {

      new wxStaticText{this,-1,wxT("Left incr"),wxPoint{20,20}};
      m_left_incr = new wxTextCtrl{this,idLeftIncr,wxT("0"),wxPoint{150,20},wxSize{200,20}};

      new wxStaticText{this,-1,wxT("Right incr"),wxPoint{20,50}};
      m_right_incr = new wxTextCtrl{this,idRightIncr,wxT("0"),wxPoint{150,50},wxSize{150,20}};

      new wxStaticText{this,-1,wxT("Top incr"),wxPoint{20,80}};
      m_top_incr = new wxTextCtrl{this,idTopIncr,wxT("0"),wxPoint{150,80},wxSize{150,20}};

      new wxStaticText{this,-1,wxT("Bottom incr"),wxPoint{20,110}};
      m_bottom_incr = new wxTextCtrl{this,idBottomIncr,wxT("0"),wxPoint{150,110},wxSize{150,20}};

      ok_button = new wxButton(this,wxID_OK,wxT("OK"),wxPoint{20,140});
      cancel_button = new wxButton(this, wxID_CANCEL,wxT("Cancel"),wxPoint{150,140});

   };
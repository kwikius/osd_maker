#include "new_bitmap_dialog.hpp"

   new_bitmap_dialog::new_bitmap_dialog(wxWindow * parent) 
   : wxDialog{
      parent, 
      idNewBitmapDialog,
      wxT(" New Bitmap Dialog"),
      wxDefaultPosition,
      wxSize{400,170}
   }
   {
      new wxStaticText{this,-1,wxT("Bitmap Name"),wxPoint{20,20}};
      m_name_text = new wxTextCtrl{this,idName,wxT("Bitmap"),wxPoint{150,20},wxSize{200,20}};

      new wxStaticText{this,-1,wxT("Bitmap X size"),wxPoint{20,50}};
      m_x_text = new wxTextCtrl{this,idXSize,wxT("50"),wxPoint{150,50},wxSize{150,20}};

      new wxStaticText{this,-1,wxT("Bitmap Y size"),wxPoint{20,80}};
      m_y_text = new wxTextCtrl{this,idYSize,wxT("50"),wxPoint{150,80},wxSize{150,20}};

      ok_button = new wxButton(this,wxID_OK,wxT("OK"),wxPoint{20,120});
      cancel_button = new wxButton(this, wxID_CANCEL,wxT("Cancel"),wxPoint{150,120});

   };

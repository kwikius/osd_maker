#define wxUSE_CHOICEDLG 1
#include <wx/choicdlg.h>

#include <quan/gx/wxwidgets/from_wxString.hpp>
#include <quan/gx/wxwidgets/to_wxString.hpp>

#include "view.hpp"
#include "panel.hpp"
#include "bitmap_tree.hpp"


struct osd_handle : public wxTreeItemData {
   osd_handle(int handle) : m_osd_handle{handle}{}
   int get_handle()const{ return m_osd_handle;}
  private:
   int const m_osd_handle;
};

void panel::add_bitmap_handle(std::string const & name, int handle)
{
   m_bitmap_tree->AppendItem(
      m_bitmap_tree->get_bitmaps_id(),
      quan::gx::wxwidgets::to_wxString(name)
      ,-1
      ,-1
      ,new osd_handle{handle}
   );
}

panel::panel (wxWindow* parent)
//:  wxScrolledWindow{parent},
 : wxWindow{parent,wxID_ANY},
  m_bitmap_tree{new bitmap_tree{this,idTreeControl}}
{
 m_bitmap_tree->Expand(m_bitmap_tree->GetRootItem());
 m_bitmap_tree->SetSize(20,20,200,200);

}

BEGIN_EVENT_TABLE (panel, wxWindow)
  EVT_TREE_ITEM_ACTIVATED(idTreeControl,panel::OnTreeItemActivated)
  EVT_TREE_ITEM_RIGHT_CLICK(idTreeControl,panel::OnTreeItemRightClick)
END_EVENT_TABLE()

bool panel::get_handle(wxTreeEvent & event, int & result_out)const
{
   auto id = event.GetItem();
   auto abc_data = m_bitmap_tree->GetItemData(id);
   if ( abc_data == nullptr){
      return false;
   }
   auto d = dynamic_cast<osd_handle*>(abc_data);
      if ( d == nullptr){
      return false;
   }
   result_out = d->get_handle();
   return true;
}


void panel::OnTreeItemRightClick(wxTreeEvent & event)
{
  int handle = -1;
  if ( !get_handle( event,handle)){
      return;
  }
  wxString choices[] =
  {
      wxT("Rename")
     ,wxT("Delete")
  };
  int style = wxDEFAULT_DIALOG_STYLE | wxOK | wxCANCEL | wxCENTRE;
  wxSingleChoiceDialog dlg{ 
      this
      ,wxT("Tree Item Actions")
      ,wxT("Tree Item Actions")
      ,sizeof(choices) / sizeof(wxString)
      ,choices
      ,NULL
      ,style
  };

  if (dlg.ShowModal() == wxID_OK){
      auto result = dlg.GetSelection();
      wxMessageBox(wxString::Format(wxT("result = %d"),result));
  }
}

void panel::OnTreeItemActivated(wxTreeEvent & event)
{
   int event_handle=-1;
   if ( ! get_handle(event,event_handle)){
      return;
   }
   auto view = wxGetApp().get_view();
   int view_handle = view->get_doc_image_handle();
   
   if ( view_handle == event_handle){
      if ( !view->is_modified()){
         return;
      }else{
         if( wxMessageBox(
           wxT("[OK] to Revert view to live-tree?"),
           wxT("Confirm View Revert"),
           wxICON_QUESTION |wxOK | wxCANCEL ) != wxOK){
           return;
         }
      }
   }else{ // not same image in view
     if ( view->is_modified()){
        if ( view->sync_hmi_view() == wxCANCEL){
            return;
        }
     }
   }
   view->copy_to_current_image(event_handle);
}



#define wxUSE_CHOICEDLG 1
#include <wx/choicdlg.h>

#include <quan/gx/wxwidgets/from_wxString.hpp>
#include <quan/gx/wxwidgets/to_wxString.hpp>

#include "view.hpp"
#include "panel.hpp"
#include "bitmap_tree.hpp"

namespace {
struct osd_bitmap_handle : public wxTreeItemData {
   osd_bitmap_handle(int handle) : m_handle{handle}{}
   int get_handle()const{ return m_handle;}
  private:
   int const m_handle;
};

struct osd_font_handle : public wxTreeItemData {
   osd_font_handle(int handle) : m_handle{handle}{}
   int get_handle()const{ return m_handle;}
  private:
   int const m_handle;
};

} // namespace 



void panel::reset()
{
   delete m_bitmap_tree ;
   m_bitmap_tree = new bitmap_tree{this,idTreeControl};
   m_bitmap_tree->Expand(m_bitmap_tree->GetRootItem());
   m_bitmap_tree->SetSize(20,20,200,200); 
}

panel::selection_type panel::get_selection_type( wxTreeEvent & event)
{
    auto id = event.GetItem();
    auto parent_id = m_bitmap_tree->GetItemParent(id);
    if( parent_id == m_bitmap_tree->get_bitmaps_id()){
         return selection_type::Bitmap;
    }else{
      if(parent_id == m_bitmap_tree->get_fonts_id()){
         return selection_type::Font;
      }else{
         return selection_type::Unknown;
      }
    }
}

void panel::add_bitmap_handle(std::string const & name, int handle)
{
   m_bitmap_tree->AppendItem(
      m_bitmap_tree->get_bitmaps_id(),
      quan::gx::wxwidgets::to_wxString(name)
      ,-1
      ,-1
      ,new osd_bitmap_handle{handle}
   );
}
void panel::add_font_handle(std::string const & name, int handle)
{
   m_bitmap_tree->AppendItem(
      m_bitmap_tree->get_fonts_id(),
      quan::gx::wxwidgets::to_wxString(name)
      ,-1
      ,-1
      ,new osd_font_handle{handle}
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

bool panel::get_bitmap_handle(wxTreeEvent & event, int & result_out)const
{
   auto id = event.GetItem();
   auto abc_data = m_bitmap_tree->GetItemData(id);
// assert?
   if ( abc_data == nullptr){
      return false;
   }
   auto d = dynamic_cast<osd_bitmap_handle*>(abc_data);
      if ( d == nullptr){
      return false;
   }
   result_out = d->get_handle();
   return true;
}

bool panel::get_font_handle(wxTreeEvent & event, int & result_out)const
{
   auto id = event.GetItem();
   auto abc_data = m_bitmap_tree->GetItemData(id);
   if ( abc_data == nullptr){
      return false;
   }
   auto d = dynamic_cast<osd_font_handle*>(abc_data);
      if ( d == nullptr){
      return false;
   }
   result_out = d->get_handle();
   return true;
}



void panel::OnTreeItemActivated(wxTreeEvent & event)
{


  // find out if font or bitmap
   auto seltype = get_selection_type(event);
   switch(seltype){
      case selection_type::Bitmap:
         on_bitmap_item_activated(event);
      break;
      case selection_type::Font:
         on_font_item_activated(event);
      break;
      default:
      break;
   }

}

void panel::on_font_item_activated(wxTreeEvent & event)
{
   // then we need to select the selected font into the preview
   // whatever is in the view can stay until a new item is selected into the view
   // so its previews job to sort it
}

void panel::on_bitmap_item_activated(wxTreeEvent & event)
{
   int event_handle=-1;
   if ( ! get_bitmap_handle(event,event_handle)){
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


void panel::OnTreeItemRightClick(wxTreeEvent & event)
{
  int handle = -1;
  if ( !get_bitmap_handle( event,handle)){
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





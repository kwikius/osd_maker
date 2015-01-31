/***************************************************************
 * Name:      quantracker_simMain.h
 * Purpose:   Defines Application Frame
 * Author:    Andy (kwikius@yahoo.com)
 * Created:   2013-03-06
 * Copyright: Andy (http://www.softpainter.org)
 * License:
 **************************************************************/

#ifndef QUANTRACKER_SIMMAIN_H
#define QUANTRACKER_SIMMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class sp_in_thread;
class main_frame_splitter;
class panel;

class main_frame: public wxFrame
{
    public:
        main_frame(wxFrame *frame, const wxString& title, wxSize const & size);
        ~main_frame();
        bool Destroy();
        void enable_save_project(bool b);
        void enable_save_project_as(bool b);
        void enable_import_image(bool b);
        void enable_import_font(bool b);
        void enable_commit_view_to_tree(bool b);
        void clear();
    private:
        void enable_menu_item(int id, bool b);
        enum
        {
            idMenuQuit = 1000,
            idMenuAbout,
            idNewBitmap,
            idImportBitmap,
            idImportFont,
            idTimer,
            idCommitViewToTree,
            idResizeViewBitmap
        };
        void OnNewProject(wxCommandEvent & event);
        void OnOpenProject(wxCommandEvent & event);
        void OnCloseProject(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnNewBitmap(wxCommandEvent & event);
        void OnImportBitmap(wxCommandEvent & event);
        void OnImportFont(wxCommandEvent & event);
        void OnSaveProject(wxCommandEvent & event);
        void OnSaveProjectAs(wxCommandEvent & event);
        void OnCommitViewToTree(wxCommandEvent & event);
        void OnResizeViewBitmap(wxCommandEvent & event);
        void OnTimer(wxTimerEvent& event);
        
        DECLARE_EVENT_TABLE()

        main_frame_splitter* m_splitter;
        wxTimer * Timer;
        wxCriticalSection m_thread_CS;
        sp_in_thread* m_sp_in_thread;
       
       void start_sp_thread();
       void create_menus();
       void create_statusbar();
       friend class OsdBmpApp;
       friend class sp_in_thread;
       friend class panel;
};


#endif // QUANTRACKER_SIMMAIN_H

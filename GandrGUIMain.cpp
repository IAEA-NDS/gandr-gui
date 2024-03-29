/***************************************************************
 * Name:      GandrguiMain.cpp
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for the application frame.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "wx/richtext/richtextctrl.h"

#include "GandrGUIMain.h"
#include "GlobalConfiguration.h"
#include "GlobalSettingsDialog.h"

 //helper functions
enum wxbuildinfoformat {
    short_f, long_f
};

wxString wxbuildinfo(wxbuildinfoformat format) {
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f)
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

BEGIN_EVENT_TABLE(GandrguiFrame, wxFrame)
EVT_CLOSE(GandrguiFrame::OnClose)
EVT_MENU(idMenuLoad, GandrguiFrame::OnLoad)
EVT_MENU(idMenuSave, GandrguiFrame::OnSave)
EVT_MENU(idMenuPref, GandrguiFrame::OnPref)
EVT_MENU(idMenuQuit, GandrguiFrame::OnQuit)
EVT_MENU(idMenuAbout, GandrguiFrame::OnAbout)
EVT_MENU(idMenuDocs, GandrguiFrame::OnDocs)
END_EVENT_TABLE()

GandrguiFrame::GandrguiFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title) {

    this->SetSize(wxSize(900, 400));
    wxPanel *panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer *BoxSizerH = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *BoxSizerLeft = new wxBoxSizer(wxVERTICAL);
    eventPanel = new EventListPanel(panel);

    BoxSizerLeft->Add(eventPanel, 1, wxEXPAND, 10);
    BoxSizerH->Add(BoxSizerLeft, 1, wxEXPAND, 10);
    BoxSizerH->SetSizeHints(this);
    panel->SetSizerAndFit(BoxSizerH);

#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuLoad, _("&Load Configuration"), _("Load configuration from a file."));
    fileMenu->Append(idMenuSave, _("&Save Configuration"), _("Save configuration to a file"));
    fileMenu->Append(idMenuPref, _("&Global settings"), _("Edit global settings"));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuDocs, _("&Documentation"), _("Show info available documentation"));
    helpMenu->Append(idMenuAbout, _("&About"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    //CreateStatusBar(2);
    //SetStatusText("In-development software", 0);
    //SetStatusText(wxbuildinfo(short_f), 1);
#endif // wxUSE_STATUSBAR

    Singleton* s = Singleton::getInstance();

    wxString readDir;
    if (wxGetEnv("ZOTTDIR", &readDir))
        s->zottDir = readDir;
    else {
        wxDirDialog
            openFolderDialog(this, _("Select path to ZOTTVL directory"));
        if (openFolderDialog.ShowModal() == wxID_CANCEL) {
            wxMessageBox("ZOTTVL directory not selected, some functionality will not be available.");
            return;     // the user changed idea...
        }
        else {
            s->zottDir = openFolderDialog.GetPath();
        }
            
    }
    if (wxGetEnv("GANDRDIR", &readDir))
        s->gandrDir = readDir;

}

GandrguiFrame::~GandrguiFrame() {

}

void GandrguiFrame::OnClose(wxCloseEvent &event) {
    Destroy();
}

void GandrguiFrame::OnLoad(wxCommandEvent &event) {
    eventPanel->evList->loadRuns();
}

void GandrguiFrame::OnSave(wxCommandEvent &event) {
    eventPanel->evList->saveRuns();
}

void GandrguiFrame::OnPref(wxCommandEvent &event) {
    GlobalSettingsDialog *sframe = new GlobalSettingsDialog(this);
    sframe->Show(true);
}

void GandrguiFrame::OnQuit(wxCommandEvent &event) {
    Destroy();
}

void GandrguiFrame::OnAbout(wxCommandEvent &event) {
    // wxString msg = wxbuildinfo(long_f);
    wxMessageBox(("GANDR User Interface, version 1.0.\n9.1.2020, Vienna"));
}

void GandrguiFrame::OnDocs(wxCommandEvent &event) {
    wxDialog *dialog = new wxDialog(this, wxID_ANY, "Documentation");
    wxRichTextCtrl *text = new  wxRichTextCtrl(dialog, wxID_ANY, wxEmptyString, wxDefaultPosition,
    wxSize(600, 300));
    wxBoxSizer *BoxSizer = new wxBoxSizer(wxHORIZONTAL);
    text->SetEditable(false);
    text->WriteText("The documentation can be located on the official website:\n");
    text->BeginURL("");
    text->BeginTextColour(wxColour(0, 0, 255));
    text->BeginUnderline();
    text->WriteText("https://www-nds.iaea.org/gandr/GandrGuiUserManual.pdf");
    text->EndUnderline();
    text->EndTextColour();
    text->EndURL();
    text->WriteText("For the source code consult the official GitHub website.\n");
    text->Update();
    BoxSizer->Add(text);
    dialog->SetSizerAndFit(BoxSizer);
    dialog->ShowModal();
}

/***************************************************************
 * Name:      GandrguiMain.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Define GandrguiFrame class.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#ifndef GANDRGUIMAIN_H
#define GANDRGUIMAIN_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "GandrguiApp.h"
#include "EventListPanel.h"


class GandrguiFrame : public wxFrame
{
public:
    GandrguiFrame(wxFrame *frame, const wxString& title);
    ~GandrguiFrame();
    EventListPanel *eventPanel;

private:
    enum
    {
        idMenuQuit = 1000,
        idMenuLoad = 1001,
        idMenuSave = 1002,
        idMenuPref = 1004,
        idMenuAbout
    };
    void OnClose(wxCloseEvent& event);
    void OnLoad(wxCommandEvent & event);
    void OnSave(wxCommandEvent & event);
    void OnPref(wxCommandEvent & event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    DECLARE_EVENT_TABLE()
};


#endif // GANDRGUIMAIN_H

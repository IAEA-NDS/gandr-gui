/***************************************************************
 * Name:      EvenetListPanel.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Define EventListPanel class.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#ifndef EVENTLISTPANEL_H
#define EVENTLISTPANEL_H
#include <wx/wx.h>
#include "EventListCtrl.h"
#include "AddRunWizard.h"
#include "InitSetupWizard.h"
#include "GandrSettingsFrame.h"
#include <wx/msgdlg.h>

 /*
 EventListPanel class is derived from wxPanel.
 It holds the necessary GUI elements needed to
 manage GANDR runs.
 */
class EventListPanel : public wxPanel
{
public:
    EventListPanel(wxPanel *parent);
    CalculationList *evList;
    wxButton *addRunButton;
    wxButton *initSetupButton;
    wxButton *removeRunButton;
    wxButton *runRunButton;
    wxButton *saveConfButton;
    wxButton *loadConfButton;

    void AddButtonClick(wxCommandEvent& event);
    void InitButtonClick(wxCommandEvent& event);
    void RemoveButtonClick(wxCommandEvent& event);
    void runRunButtonClick(wxCommandEvent & event);
    void saveButtonClick(wxCommandEvent & event);
    void loadButtonClick(wxCommandEvent & event);

    void onListItemDoubleClicked(wxCommandEvent & event);

    //void ContextMenu(wxchecklistboxevent& event);

    std::string zottdir = "../";

protected:

private:
};

#endif // EVENTLISTPANEL_H

/***************************************************************
 * Name:      GlobalSettinsDialog.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Header file for global settings dialog
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2011-11-1
 * License:   GPL-3
 **************************************************************/

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/radiobut.h>
#include "GanrunConfiguration.h"
#include "GlobalConfiguration.h"

class GlobalSettingsDialog : public wxDialog
{
public:
    GlobalSettingsDialog(wxWindow *parent);
private:
    wxTextCtrl *zottTextPathCtrl;
    wxButton *pathToZottButton;
    wxButton *confirmButton;
    void onConfirm(wxCommandEvent& event);
    void onBrowse(wxCommandEvent& event);



};
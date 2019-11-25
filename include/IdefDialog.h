/***************************************************************
 * Name:      IdefDialog.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Header for a class that defines a dialog used to 
 *            make custom definitions from exfor.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-11-7
 * License:   GPL-3
 **************************************************************/

#include <wx/wx.h>
#include <wx/grid.h>
#include "GanrunConfiguration.h"

class IdefDialog : public wxDialog
{
public:
    IdefDialog(const wxString& title, std::array<bool, NRMAX> *xexfor, std::array<reaction, 25> *reactions);
private:
    wxCheckListBox* checklist;
    wxButton *confirmButton;
    wxSize wSize = wxSize(400, 400);
    std::array<bool, NRMAX> *xexfor;

    void onConfirm(wxCommandEvent& event);


};
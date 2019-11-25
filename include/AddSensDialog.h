/***************************************************************
 * Name:      AddSensDialog.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Define a custom dialog used to addeitional 
 *            sensitivity information when fitting integral data as
 *            standards.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-11-7
 * License:   GPL-3
 **************************************************************/

#include <wx/wx.h>
#include <wx/grid.h>
#include "GanrunConfiguration.h"

class SensDialog : public wxDialog
{
public:
    SensDialog(const wxString& title, additional_sens_data *sensData);
    additional_sens_data *sensData;
private:
    wxTextCtrl *matValueTextBox;
    wxGrid *dataGrid;
    wxButton *confirmButton;
    wxButton *setAllButton;
    wxTextCtrl *setAllTextBox;
    wxSize wSize = wxSize(400, 400);

    void onConfirm(wxCommandEvent& event);
    void onSetAllButton(wxCommandEvent& event);


};
/***************************************************************
 * Name:      AddCovDialog.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Define a custom dialog used to add cov. values.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#include <wx/wx.h>
#include "GanrunConfiguration.h"

class CovDialog : public wxDialog
{
public:
    CovDialog(const wxString& title, std::pair<int, int> range, additional_cov_data *covData);
    ~CovDialog();
    additional_cov_data *covData;
private:
    wxTextCtrl *covValueTextBox;
    wxTextCtrl *startValueTextBox;
    wxTextCtrl *endValueTextBox;
    wxRadioBox *covRadioBox;
    wxButton *confirmButton;
    wxSize wSize = wxSize(400, 400);
    void OnClose(wxCloseEvent& event);

    void onResize(wxSizeEvent& event) {
        Layout();
        event.Skip();
    }

    void onConfirm(wxCommandEvent& event);


};
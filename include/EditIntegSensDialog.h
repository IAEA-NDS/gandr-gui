/***************************************************************
 * Name:      EditIntegSensDIalog.h
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
#include "GanrunConfiguration.h"

class SensConfigDialog : public wxDialog
{
public:
    intReac *data;
	std::string name;
	SensConfigDialog(intReac* data, std::string name, intReac* fillData);
	intReac* fData;
private:

	wxTextCtrl *nameTextBox;
;   wxTextCtrl *zaTextBox;
    wxTextCtrl *matnamTextBox;
    wxTextCtrl *reacnoTextBox;
	wxTextCtrl *tempTextBox;

	wxButton *confirmButton;
	std::map<int, int> endfnums;

    void onConfirm(wxCommandEvent& event);
	void zaEntered(wxCommandEvent& event);

};
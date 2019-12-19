/***************************************************************
 * Name:      GlobalSettingsDialog.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for custom dialog used to modify the global 
 *            settings
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-11-7
 * License:   GPL-3
 **************************************************************/

#include "GlobalSettingsDialog.h"


GlobalSettingsDialog::GlobalSettingsDialog(wxWindow *parent)
    :wxDialog(parent, wxID_ANY, "Global settings", wxDefaultPosition, wxSize(400, 200)) {

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);


    wxStaticText *text = new wxStaticText(this, wxID_ANY,
        "Set path to the ZOTTVL directory");
    mainSizer->Add(text);

    wxBoxSizer *zottSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(zottSizer);

    Singleton* s = Singleton::getInstance(); 
    zottTextPathCtrl = new wxTextCtrl(this, wxID_ANY, s->zottDir);
    pathToZottButton = new wxButton(this, wxID_ANY, "Browse");
    zottSizer->Add(zottTextPathCtrl, 1, wxALL);
    zottSizer->Add(pathToZottButton, 1, wxALL);


    mainSizer->Add(0, 1, wxEXPAND);
    confirmButton = new wxButton(this, wxID_ANY, "Confirm");
    mainSizer->Add(confirmButton);

    Connect(pathToZottButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(GlobalSettingsDialog::onBrowse));
    Connect(confirmButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(GlobalSettingsDialog::onConfirm));

    SetSizer(mainSizer);
}

void GlobalSettingsDialog::onConfirm(wxCommandEvent & event) {
    Singleton* s = Singleton::getInstance();
    s->zottDir = zottTextPathCtrl->GetValue();
	zottTextPathCtrl->SetValue(s->zottDir);
	wxDialog::EndModal(wxID_OK);
}

void GlobalSettingsDialog::onBrowse(wxCommandEvent & event) {
    wxDirDialog
        openFolderDialog(this, _("Select path to ZOTTVL directory"));
    if (openFolderDialog.ShowModal() == wxID_CANCEL) {
        //wxMessageBox("ZOTTVL directory not selected, some functionality will not be available.");
        return;     // the user changed idea...
    }
    else {
        Singleton* s = Singleton::getInstance();
        s->zottDir = openFolderDialog.GetPath();
		zottTextPathCtrl->SetValue(s->zottDir);
    }
}

/***************************************************************
 * Name:      IdefDialog.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for a custom dialog used to define the exfor 
 *			  reaction.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-11-7
 * License:   GPL-3
 **************************************************************/

#include "IdefDialog.h"


IdefDialog::IdefDialog(const wxString & title, std::array<bool, NRMAX> *xexfor, std::array<reaction, 25> *reactions)
    :wxDialog(NULL, wxID_ANY, "Exfor reaction definitions", wxDefaultPosition, wxSize(200, 200)), xexfor(xexfor) {
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *topLeftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topRightSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(topSizer, wxEXPAND);
    topSizer->Add(topLeftSizer, 1);
    topSizer->AddSpacer(1);
    topSizer->Add(topRightSizer, 2, wxEXPAND);

    wxStaticText *instructionText = new wxStaticText(this, wxID_ANY,
        "Use the text box bellow to enter a material number "
        "and the table on the right to specify the sensitivities "
        "to the 25 GANDR reactions.");
    topLeftSizer->Add(instructionText, 1, wxEXPAND, 5);
   // create checked list and add here
    checklist = new wxCheckListBox(this, wxID_ANY);
    topRightSizer->Add(checklist, 1, wxEXPAND, 5);

    instructionText->Wrap(0.6 * wSize.GetWidth());
    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    confirmButton = new wxButton(this, wxID_ANY, "Confirm");
    mainSizer->Add(bottomSizer, 1, wxEXPAND, 5);
    topLeftSizer->Add(confirmButton, 0, wxALIGN_LEFT);


    bool reactions_available = 0;

    if (reactions != nullptr) {
        reactions_available = 1;
        for (int i = 0; i < 25; i++) {
            checklist->Insert(reactions->at(i).label, i);
        }
    }

    if (reactions_available) {
    }
    else {
        for (int i = 0; i < 25; i++) {
            checklist->Insert("Gandr reaction" + std::to_string(i + 1), i);
        }
    }

    for (int i = 0; i < 25; i++) {
        if (xexfor->at(i))
            checklist->Check(i);
    }

    Connect(confirmButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(IdefDialog::onConfirm));

    SetSizer(mainSizer);
    SetSize(wSize);
    instructionText->Wrap(0.6 * wSize.GetWidth());
    auto colWidth = 0.2 * wSize.GetWidth();

}


void IdefDialog::onConfirm(wxCommandEvent & event) {
    wxArrayInt selections;
    checklist->GetCheckedItems(selections);
    xexfor->fill(false);
    for (auto s : selections) {
        xexfor->at(s) = true;
    }
    
    if (true)
        wxDialog::EndModal(wxID_OK);
    else
        wxMessageBox("Invalid data passed.");

}


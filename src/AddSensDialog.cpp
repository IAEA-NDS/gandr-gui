/***************************************************************
 * Name:      AddSensDialog.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for custom dialog used to addeitional
 *            sensitivity information when fitting integral data as
 *            standards.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-11-7
 * License:   GPL-3
 **************************************************************/

#include "AddSensDialog.h"


SensDialog::SensDialog(const wxString & title, additional_sens_data *sensData)
    :wxDialog(NULL, wxID_ANY, "Sensitivity data", wxDefaultPosition, wxSize(200, 200)), sensData(sensData) {
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
    matValueTextBox = new wxTextCtrl(this, wxID_ANY, std::to_string(sensData->nam));
    topLeftSizer->Add(matValueTextBox);

    dataGrid = new wxGrid(this, wxID_ANY);
    dataGrid->CreateGrid(NRMAX, 1);
    for (int i = 0; i < NRMAX; i++) {
        std::string data = std::to_string(sensData->xexary.at(i));
        dataGrid->SetCellValue(i, 0, data);
    }
    topRightSizer->Add(dataGrid, 5, wxEXPAND);
    instructionText->Wrap(0.6 * wSize.GetWidth());
    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    confirmButton = new wxButton(this, wxID_ANY, "Confirm");
    mainSizer->Add(bottomSizer, 1, wxEXPAND, 5);
    

    topLeftSizer->Add(new wxStaticText(this, wxID_ANY,
        "Set all elements to value"));
    setAllTextBox = new wxTextCtrl(this, wxID_ANY, "0");
    topLeftSizer->Add(setAllTextBox);
    setAllButton = new wxButton(this, wxID_ANY, "Set");
    topLeftSizer->Add(setAllButton);

	topLeftSizer->Add(confirmButton, 0, wxALIGN_LEFT);

    
    Connect(confirmButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SensDialog::onConfirm));
    Connect(setAllButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SensDialog::onSetAllButton));

    SetSizer(mainSizer);
    SetSize(wSize);
    instructionText->Wrap(0.6 * wSize.GetWidth());
    auto colWidth = 0.2 * wSize.GetWidth();
    //colWidth -= dataGrid->GetColLabelSize();
    dataGrid->SetColSize(0, colWidth);
    dataGrid->SetRowLabelSize(0.2 * wSize.GetWidth()-20);

}


void SensDialog::onConfirm(wxCommandEvent & event) {
    for (int i = 0; i < NRMAX; i++) {
        double data = std::stof(std::string(dataGrid->GetCellValue(i, 0).c_str()));
        sensData->xexary.at(i) = data;
    }
    sensData->nam = std::stoi(std::string(matValueTextBox->GetValue().c_str()));
    if (sensData->nam)
        wxDialog::EndModal(wxID_OK);
    else
        wxMessageBox("Enter a valid material number.");

}

void SensDialog::onSetAllButton(wxCommandEvent & event) {
    if (setAllTextBox->GetValue().IsNumber()) {
        wxMessageBox("Invalid number entered");
        return;
    }

    for (int i = 0; i < NRMAX; i++)
        dataGrid->SetCellValue(i, 0, setAllTextBox->GetValue());
}


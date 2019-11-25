#include "AddCovDialog.h"
/***************************************************************
 * Name:      AddCovDialog.cpp
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for a custom dialog used to add cov. values.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

enum {
    M_ABSOLUTE,
    M_RELATIVE
};

CovDialog::CovDialog(const wxString & title, std::pair<int, int> range, additional_cov_data *covData)
    :wxDialog(NULL, wxID_ANY, "Covariance data", wxDefaultPosition, wxSize(200, 200)), covData(covData) {
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *topLeftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topRightSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(topSizer);
    topSizer->Add(topLeftSizer, 1);
    topSizer->AddSpacer(1);
    topSizer->Add(topRightSizer, 1);
    topLeftSizer->Add(new wxStaticText(this, wxID_ANY, "Enter absolute value:"));
    covValueTextBox = new wxTextCtrl(this, wxID_ANY, std::to_string(covData->covfc), wxDefaultPosition, wxDefaultSize);
    topLeftSizer->Add(covValueTextBox);
    wxString choices[2];
    choices[M_ABSOLUTE] = "Absolute";
    choices[M_RELATIVE] = "Relative";
    covRadioBox = new wxRadioBox(this, wxID_ANY, "Value is",
        wxDefaultPosition, wxDefaultSize, WXSIZEOF(choices), choices,
        1, wxRA_SPECIFY_COLS);
    if (covData->irelfc)
        covRadioBox->SetSelection(M_RELATIVE);
    else
        covRadioBox->SetSelection(M_ABSOLUTE);
    mainSizer->Add(covRadioBox);
    wxStaticText *covInstruct = new wxStaticText(this, wxID_ANY,
            "If the supplied covariance does not"
            "apply to all data points in the experiment, use the "
            "following fields to enter starting and edning values.");
    mainSizer->Add(covInstruct, 1, wxEXPAND);
    //covInstruct->Wrap(covInstruct->GetClientSize().x);
    covInstruct->Wrap(wSize.GetX());

    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(bottomSizer);
    startValueTextBox = new wxTextCtrl(this, wxID_ANY, std::to_string(range.first),
        wxDefaultPosition, wxDefaultSize, 0L, wxTextValidator(wxFILTER_NUMERIC));
    endValueTextBox = new wxTextCtrl(this, wxID_ANY, std::to_string(range.second),
        wxDefaultPosition, wxDefaultSize, 0L, wxTextValidator(wxFILTER_NUMERIC));
    bottomSizer->Add(startValueTextBox, 1);
    bottomSizer->Add(endValueTextBox, 1);
    confirmButton = new wxButton(this, wxID_OK, "Confirm");
    mainSizer->Add(confirmButton, 0, wxALIGN_RIGHT);

    
    Connect(confirmButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(CovDialog::onConfirm));
    Bind(wxEVT_CLOSE_WINDOW, &CovDialog::OnClose, this);
    SetSizerAndFit(mainSizer);
    SetSize(wSize);

}

CovDialog::~CovDialog() {
}

void CovDialog::OnClose(wxCloseEvent & event) {
    EndModal(wxID_CANCEL);
}

void CovDialog::onConfirm(wxCommandEvent & event) {
    int low_lim, high_lim;
    low_lim = wxAtoi(startValueTextBox->GetValue());
    high_lim = wxAtoi(endValueTextBox->GetValue());
    covData->range = std::make_pair(low_lim, high_lim);
    covData->covfc = wxAtof(covValueTextBox->GetValue());
    covData->irelfc = covRadioBox->GetSelection();
    wxDialog::EndModal(wxID_OK);
}

/***************************************************************
 * Name:      ExforCovDialog.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for custom dialog used add additional 
 *            covariance information to the fit.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-11-7
 * License:   GPL-3
 **************************************************************/

#include "ExforCovDialog.h"

enum {
    diag_ID1=101,
    diag_ID2=102,
    diag_ID3=103,
    cov_ID1 = 104,
    cov_ID2 = 105,
    cov_ID3 = 106
};


ExforCovDialog::ExforCovDialog(const wxString & title, int expno, int data_len, exfor_cov_data *data, int *thinning)
    :wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(200, 200)), data(data),
    data_len(data_len), expno(expno), thinning(thinning) {
    mainSizer = new wxBoxSizer(wxHORIZONTAL);
    leftSizer = new wxBoxSizer(wxVERTICAL);
    rightSizer = new wxBoxSizer(wxVERTICAL);

    createGeneralControl();
    if (expno >= 0) {
        createFunctionControl();
    }
    confirmButton = new wxButton(this, wxID_OK, "Confirm");
    leftSizer->Add(confirmButton);
    loadData();

    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(ExforCovDialog::onConfirm));
	
    Connect(diagButtons[0]->GetId(), wxEVT_COMMAND_RADIOBUTTON_SELECTED,
        wxCommandEventHandler(ExforCovDialog::OnRadioBoxClickedDiag));
    Connect(diagButtons[1]->GetId(), wxEVT_COMMAND_RADIOBUTTON_SELECTED,
        wxCommandEventHandler(ExforCovDialog::OnRadioBoxClickedDiag));
    Connect(diagButtons[2]->GetId(), wxEVT_COMMAND_RADIOBUTTON_SELECTED,
        wxCommandEventHandler(ExforCovDialog::OnRadioBoxClickedDiag));
    Connect(covminButtons[0]->GetId(), wxEVT_COMMAND_RADIOBUTTON_SELECTED,
        wxCommandEventHandler(ExforCovDialog::OnRadioBoxClickedCovmin));
    Connect(covminButtons[1]->GetId(), wxEVT_COMMAND_RADIOBUTTON_SELECTED,
        wxCommandEventHandler(ExforCovDialog::OnRadioBoxClickedCovmin));
    Connect(covminButtons[2]->GetId(), wxEVT_COMMAND_RADIOBUTTON_SELECTED,
        wxCommandEventHandler(ExforCovDialog::OnRadioBoxClickedCovmin));
    

    Connect(absRelRadioBox->GetId(), wxEVT_RADIOBOX,
        wxCommandEventHandler(ExforCovDialog::OnRadioBoxClickedRelAbs));

    SetSizerAndFit(mainSizer);
    SetSize(wxSize(800, 800));
}

ExforCovDialog::~ExforCovDialog() {
}

void ExforCovDialog::createGeneralControl() {
    wxSize smallBoxSize(70, 25);

    mainSizer->Add(leftSizer, 1, wxALL, 5);
    mainSizer->Add(rightSizer, 1, wxALL, 5);

    leftSizer->Add(new wxStaticText(this, wxID_ANY,
        "Apply the following algorithm the diagonal elements"),
        0, 0, 3);

    wxBoxSizer *diag1Sizer = new wxBoxSizer(wxHORIZONTAL);
    diagButtons[0] = new wxRadioButton(this, diag_ID1, "",
        wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    diagButtons[1] = new wxRadioButton(this, diag_ID2, "");
    diagButtons[2] = new wxRadioButton(this, diag_ID3, "");
    diag1Sizer->Add(diagButtons[0], 0, wxALIGN_LEFT, 5);
    sigmaTextBox = new wxTextCtrl(this, wxID_ANY, "2",
        wxDefaultPosition, smallBoxSize);
    diag1Sizer->Add(sigmaTextBox, 1, wxALL, 5);
    diag1Sizer->Add(new wxStaticText(this, wxID_ANY,
        " - sigma correction"));

    wxBoxSizer *diag2Sizer = new wxBoxSizer(wxHORIZONTAL);

    diag2Sizer->Add(diagButtons[1], 0, wxALIGN_LEFT, 5);
    diag2Sizer->Add(new wxStaticText(this, wxID_ANY,
        "Band of consistency with width of "));
    bandTextBox = new wxTextCtrl(this, wxID_ANY, "7",
        wxDefaultPosition, smallBoxSize);
    //bandTextBox->Enable(false);
    diag2Sizer->Add(bandTextBox, 0, wxALIGN_LEFT, 5);

    wxBoxSizer *diag3Sizer = new wxBoxSizer(wxHORIZONTAL);

    diag3Sizer->Add(diagButtons[2], 0, wxALL, 5);
    diag3Sizer->Add(new wxStaticText(this, wxID_ANY,
        "No correction of diagonal elements"), 0, wxALIGN_LEFT, 5);

    leftSizer->Add(diag1Sizer, 0, wxALIGN_LEFT);
    leftSizer->Add(diag2Sizer, 0, wxALIGN_LEFT);
    leftSizer->Add(diag3Sizer, 0, wxALIGN_LEFT);
    leftSizer->AddStretchSpacer(1);

    leftSizer->Add(new wxStaticText(this, wxID_ANY,
        "Apply the following systematic covariance option"),
        0, 0, 3);

    wxBoxSizer *sys1Sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *sys2Sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *sys3Sizer = new wxBoxSizer(wxHORIZONTAL);

    covminButtons[0] = new wxRadioButton(this, cov_ID1, "",
        wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    covminButtons[1] = new wxRadioButton(this, cov_ID2, "");
    covminButtons[2] = new wxRadioButton(this, cov_ID3, "");

    sys1Sizer->Add(covminButtons[0], 1, wxALIGN_LEFT, 5);
    sys1Sizer->Add(new wxStaticText(this, wxID_ANY,
        "Value of "), 0, wxALIGN_LEFT, 5);
    covminTextBox = new wxTextCtrl(this, wxID_ANY, "0",
        wxDefaultPosition, smallBoxSize, wxEXPAND);
    sys1Sizer->Add(covminTextBox, 1, wxALL, 5);
    sys1Sizer->Add(new wxStaticText(this, wxID_ANY,
        " added to off-diagonal elements."), 0, wxALIGN_LEFT, 5);

    sys2Sizer->Add(covminButtons[1], 1, wxALIGN_LEFT, 5);;
    sys2Sizer->Add(new wxStaticText(this, wxID_ANY,
        "Variance of most accurate value, devided by "), 0, wxALIGN_LEFT, 5);
    varianceTextBox = new wxTextCtrl(this, wxID_ANY, "0.00001",
        wxDefaultPosition, wxDefaultSize);
    //varianceTextBox->Enable(false);
    sys2Sizer->Add(varianceTextBox, 1, wxALL, 5);

    sys3Sizer->Add(covminButtons[2], 1, wxALIGN_LEFT, 5);;
    sys3Sizer->Add(new wxStaticText(this, wxID_ANY,
        "No systematic variance correction."), 0, wxALIGN_LEFT, 5);;

    leftSizer->Add(sys1Sizer, 1, wxALIGN_LEFT);
    leftSizer->Add(sys2Sizer, 1, wxALIGN_LEFT);
    leftSizer->Add(sys3Sizer, 1, wxALIGN_LEFT);
    leftSizer->AddStretchSpacer(1);


    wxString choices[2];
    choices[0] = "Absolute";
    choices[1] = "Relative";
    absRelRadioBox = new wxRadioBox(this, wxID_ANY,
        "The symmetric covariance is", wxDefaultPosition,
        wxDefaultSize, WXSIZEOF(choices), choices);
    leftSizer->Add(absRelRadioBox, 1, wxEXPAND, 5);
    leftSizer->AddStretchSpacer(1);


}

void ExforCovDialog::createFunctionControl() {
    leftSizer->Add(new wxStaticText(this, wxID_ANY, 
        "Enter a thinning value"));
    thinningTextBox = new wxTextCtrl(this, wxID_ANY, 
        std::to_string(*thinning), wxDefaultPosition, wxDefaultSize, 
        0, wxTextValidator(wxFILTER_DIGITS));
    leftSizer->Add(thinningTextBox);
    
    wxString choices[2];
    choices[0] = "Disable";
    choices[1] = "Enable";
    enableDisableRadioBox = new wxRadioBox(this, wxID_ANY,
        "Enable/disable system function", wxDefaultPosition,
        wxDefaultSize, WXSIZEOF(choices), choices);
    leftSizer->Add(enableDisableRadioBox);

    sysFuncGrid = new wxGrid(this, wxID_ANY);

    rightSizer->Add(sysFuncGrid, 1, wxEXPAND | wxRIGHT, 1);
    sysFuncGrid->CreateGrid(data_len, 1);
    sysFuncGrid->Enable(false);
    sysFuncGrid->AutoSizeColumns();
    SetSizerAndFit(mainSizer);

    wxSize size = rightSizer->GetSize();
    sysFuncGrid->SetColSize(0, rightSizer->GetSize().GetX() - sysFuncGrid->GetDefaultColSize()-19);
    sysFuncGrid->SetScrollRate(1, 1);
    leftSizer->AddStretchSpacer(1);

    SetSizerAndFit(mainSizer);
    Connect(enableDisableRadioBox->GetId(), wxEVT_RADIOBOX,
        wxCommandEventHandler(ExforCovDialog::OnRadioBoxClickedRelFunc));
}

void ExforCovDialog::loadData() {
    wxCommandEvent event(wxEVT_COMMAND_RADIOBUTTON_SELECTED);
    if (data->inca > 0) {
        diagButtons[0]->SetValue(true);
        sigmaTextBox->SetValue(std::to_string(data->inca));
        event.SetId(diag_ID1);
    }
    else if (data->inca < 0) {
        diagButtons[1]->SetValue(true);
        bandTextBox->SetValue(std::to_string(std::abs(data->inca)));
        event.SetId(diag_ID2);
    }
    else {
        diagButtons[2]->SetValue(true);
        event.SetId(diag_ID3);
    }
        OnRadioBoxClickedDiag(event);

    if (data->covmin > 0) {
        covminButtons[0]->SetValue(true);
        covminTextBox->SetValue(std::to_string(data->covmin));
        event.SetId(cov_ID1);
    }
    else if (data->covmin < 0) {
        covminButtons[1]->SetValue(true);
        varianceTextBox->SetValue(std::to_string(std::abs(data->covmin)));
        event.SetId(cov_ID2);
    }
    else {
        covminButtons[2]->SetValue(true);
        event.SetId(cov_ID3);
    }
        OnRadioBoxClickedCovmin(event);

    absRelRadioBox->SetSelection(data->irelco);

    if (data->isys && expno >= 0) {
        enableDisableRadioBox->SetSelection(1);
        sysFuncGrid->Enable(true);
        for (int i = 0; i < data_len; i++) {
            sysFuncGrid->SetCellValue(i, 0, std::to_string(data->xsys.at(i)));
        }
    }

}

void ExforCovDialog::setData() {
}

void ExforCovDialog::onConfirm(wxCommandEvent & event) {
    int diag_sel = 0;
    for (int i = 0; i < 3; i++) {
        diag_sel = diagButtons[i]->GetValue() ? i : diag_sel;
    }
    if (diag_sel == 0) {
        data->inca = wxAtof(sigmaTextBox->GetValue());
    }
    else if (diag_sel == 1) {
        data->inca = -wxAtof(bandTextBox->GetValue());
    }
    else {
        data->inca = 0;
    }

    int covmin_sel = 0;
    for (int i = 0; i < 3; i++) {
        covmin_sel = covminButtons[i]->GetValue() ? i : covmin_sel;
    }
    if (covmin_sel == 0) {
        data->covmin = std::atof(covminTextBox->GetValue().c_str());
    }
    else if (covmin_sel == 1) {
        data->covmin = -std::atof(varianceTextBox->GetValue().c_str());
    }
    else {
        data->covmin = 0;
    }

    data->irelco = absRelRadioBox->GetSelection();

    data->isys = 0;
    if (expno >= 0) {
        if (enableDisableRadioBox->GetSelection()) {
            char *ending = 0;
            data->xsys.clear();
            for (int i = 0; i < data_len; i++) {
                double val = std::strtof(sysFuncGrid->GetCellValue(i, 0).c_str(), &ending);
                /* Check later why this does not work!!
                if (*ending != 0) {
                    wxMessageBox("Invalid input in table");
                    return;
                }
                */
                data->xsys.push_back(val);
            }
            data->isys = 1;
        }
    }
    data->expno = expno;
	if (expno > 0)
		*thinning = wxAtoi(thinningTextBox->GetValue());
    exfor_cov_data empty_data;
    if (!(*data == empty_data))
        data->edited = true;
    wxDialog::EndModal(wxID_OK);
}

void ExforCovDialog::OnRadioBoxClickedDiag(wxCommandEvent & event) {
    switch (event.GetId()) {
    case diag_ID1:
        sigmaTextBox->Enable(true);
        bandTextBox->Enable(false);
        break;
    case diag_ID2:
        sigmaTextBox->Enable(false);
        bandTextBox->Enable(true);
        break;
    case diag_ID3:
        sigmaTextBox->Enable(false);
        bandTextBox->Enable(false);
        break;
    }
}

void ExforCovDialog::OnRadioBoxClickedCovmin(wxCommandEvent & event) {
    switch (event.GetId()) {
    case cov_ID1:
        covminTextBox->Enable(true);
        varianceTextBox->Enable(false);
        break;
    case cov_ID2:
        covminTextBox->Enable(false);
        varianceTextBox->Enable(true);
        break;
    case cov_ID3:
        covminTextBox->Enable(false);
        varianceTextBox->Enable(false);
        break;
    }
}

void ExforCovDialog::OnRadioBoxClickedRelAbs(wxCommandEvent & event) {
    //wxMessageBox("Radio box clicked abs rel");
    
}

void ExforCovDialog::OnRadioBoxClickedRelFunc(wxCommandEvent & event) {
    if (enableDisableRadioBox->GetSelection()) {
        sysFuncGrid->Enable(true);
        if (data->xsys.size() > 0) {
            for (int i = 0; i < data_len; i++) {
                sysFuncGrid->SetCellValue(i, 0, std::to_string(data->xsys.at(i)));
            }
        }
        else {
            for (int i = 0; i < data_len; i++) {
                sysFuncGrid->SetCellValue(i, 0, "0");
            }
        }
    }
    else {
        sysFuncGrid->Enable(false);
    }
}


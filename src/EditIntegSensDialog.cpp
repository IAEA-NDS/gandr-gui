/***************************************************************
 * Name:      EditIntegSensDialog.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for custom dialog used to add additional
 *            sensitivity information when fitting integral data.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-11-7
 * License:   GPL-3
 **************************************************************/

#include "EditIntegSensDialog.h"


SensConfigDialog::SensConfigDialog(intReac *data, std::string name, intReac* fillData)
    :wxDialog(NULL, wxID_ANY, "Edit int reaction", wxDefaultPosition, wxSize(200, 200)), data(data), name(name) {
    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    auto nameSizer = new wxBoxSizer(wxHORIZONTAL);
    auto matSizer = new wxBoxSizer(wxHORIZONTAL);
	auto reacSizer = new wxBoxSizer(wxHORIZONTAL);
	auto tempSizer = new wxBoxSizer(wxHORIZONTAL);
	auto confirmSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* nameLabel   = new wxStaticText(this, wxID_ANY, "Name: ");
	wxStaticText* azLabel     = new wxStaticText(this, wxID_ANY, "ZA: ");
	wxStaticText* matnamLabel = new wxStaticText(this, wxID_ANY, "matnam: ");
	wxStaticText* reacnoLabel = new wxStaticText(this, wxID_ANY, "mt: ");
	wxStaticText* tempLabel   = new wxStaticText(this, wxID_ANY, "Temp: ");


	if (data->za == 1) {
		data->za = fillData->za;
		data->matna = fillData->matna;
		data->temp = fillData->temp;
	}

	mainSizer->Add(nameSizer, 1, wxALL, 5);
	nameSizer->Add(nameLabel, 1, wxALL, 5);
	nameTextBox = new wxTextCtrl(this, wxID_ANY, data->rnam);
	nameSizer->Add(nameTextBox, 1, wxALL, 5);

	mainSizer->Add(matSizer, 1, wxALL, 5);
	matSizer->Add(azLabel, 1, wxALL, 5);
	zaTextBox = new wxTextCtrl(this, wxID_ANY, std::to_string(data->za));
	matSizer->Add(zaTextBox, 1, wxALL, 5);
	matSizer->Add(matnamLabel, 1, wxALL, 5);
	matnamTextBox = new wxTextCtrl(this, wxID_ANY, std::to_string(data->matna));
	matSizer->Add(matnamTextBox, 1, wxALL, 5);

	mainSizer->Add(reacSizer, 1, wxALL, 5);
	reacSizer->Add(reacnoLabel, 1, wxALL, 5);
	reacnoTextBox = new wxTextCtrl(this, wxID_ANY, std::to_string(data->mtd));
	reacSizer->Add(reacnoTextBox, 1, wxALL, 5);

	mainSizer->Add(tempSizer, 1, wxALL, 5);
	tempSizer->Add(tempLabel, 1, wxALL, 5);
	tempTextBox = new wxTextCtrl(this, wxID_ANY, std::to_string(data->temp));
	tempSizer->Add(tempTextBox, 1, wxALL, 5);

	mainSizer->Add(confirmSizer, 1, wxALL, 5);
	confirmButton = new wxButton(this, wxID_ANY, "Confirm");
	confirmSizer->Add(confirmButton, 1, wxALL, 5);


    Connect(confirmButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SensConfigDialog::onConfirm));
	fData = fillData;
    SetSizerAndFit(mainSizer);
    SetSize(wxSize(600, 400));
    Centre();
}

void SensConfigDialog::onConfirm(wxCommandEvent& event) {
	data->rnam = std::string(nameTextBox->GetValue().c_str());
	data->za = std::stoi(std::string(zaTextBox->GetValue().c_str()));
	data->matna = std::stoi(std::string(matnamTextBox->GetValue().c_str()));
	data->mtd = std::stoi(std::string(reacnoTextBox->GetValue().c_str()));
	data->temp = std::stoi(std::string(tempTextBox->GetValue().c_str()));	
	if (fData->za == 1 ) {
		fData->za = data->za;
		fData->matna = data->matna;
		fData->temp = data->temp;
	}
	wxDialog::EndModal(wxID_OK);
}
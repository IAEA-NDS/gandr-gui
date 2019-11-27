/***************************************************************
 * Name:      AddRunWizard.cpp
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Source code for AddRunWizard, a wizard that can add 
 *            experiments to the "STANDARDS run".
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#include "AddMaterialWizard.h"
#include "EndfNumbers.h"


AddMaterialWizard::AddMaterialWizard(wxWindow *parent, std::string fileName = "92235.pendf", imodes imode = STANDARDS, standards_experiment *experiment = nullptr)
    : fileName(fileName),  experiment(experiment), imode(imode) {

    Create(parent, wxID_ANY, "Add material wizard",
        wxNullBitmap, wxDefaultPosition,
        wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
	SetPageSize(wSize);
	m_page1 = new StandardExpInitPage(this);
	
    m_conv = new AddConvPage(this);
    m_integ1 = new DataPointsPage(this);
    m_addsens = new AdditionalSensPage(this);
    
    m_page1->SetNext(m_conv);
    m_conv->SetPrev(m_page1);
    m_conv->SetNext(m_integ1);
    m_integ1->SetPrev(m_conv);
	
    m_integ1->SetNext(m_addsens);
    m_addsens->SetPrev(m_integ1);
    
}

AddMaterialWizard::~AddMaterialWizard() {
}

wxWizardPage * AddMaterialWizard::GetSecondPage(short unsigned int page) {
    if (experiment->ismg == DIFFERENTIAL_EXP) {
        return m_addsens;
    }
    else if (experiment->ismg == INTEGRAL_EXP) {
        return m_integ1;
    }
}

wxWizardPage * AddMaterialWizard::GetPrevPage() {
    if (experiment->ismg == DIFFERENTIAL_EXP) {
        return m_conv;
    }
    else if (experiment->ismg == INTEGRAL_EXP) {
        return m_integ1;
    }
}

void AddMaterialWizard::redoDataGrid() {
    DataPointsPage *dp = dynamic_cast<DataPointsPage*>(m_integ1);
    setGridRowNumbers(dp->dataGrid, experiment->nps);
    experiment->mgrid.resize(experiment->nps);
    for (int i = 0; i < experiment->nps; i++) {
        dp->dataGrid->SetCellValue(i, 0, std::to_string(experiment->mgrid.at(i)));
    }
}

StandardExpInitPage::StandardExpInitPage(wxWizard *parent) : wxWizardPageSimple(parent) {
    
	wizard = dynamic_cast<AddMaterialWizard*>(parent);
    wxString choices[2];
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    choices[DIFFERENTIAL_EXP] = "Differential data";
    choices[INTEGRAL_EXP] = "Integral data";
    matnumTextCtrl = new wxTextCtrl(this, wxID_ANY);
    mteTextCtrl = new wxTextCtrl(this, wxID_ANY, "1");
    m_radio = new wxRadioBox(this, wxID_ANY, "I want to fit:",
        wxDefaultPosition, wxDefaultSize,
        WXSIZEOF(choices), choices,
        1, wxRA_SPECIFY_COLS);
    if (wizard->experiment->ismg == DIFFERENTIAL_EXP)
        m_radio->SetSelection(DIFFERENTIAL_EXP);
    else
        m_radio->SetSelection(INTEGRAL_EXP);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Material number"));
    mainSizer->Add(matnumTextCtrl);
    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Reaction number"));
    mainSizer->Add(mteTextCtrl);
    mainSizer->Add(
        m_radio,
        0, // No stretching
        wxALL,
        5 // Border
    );

    wxString msg("Select integral or differential data");
	elToMap(&endfnums);
    mainSizer->Add(
        new wxStaticText(this, wxID_ANY, msg),
        1, // No stretching
        wxALIGN_LEFT | wxEXPAND,
        5 // Border
    );

    Bind(wxEVT_WIZARD_BEFORE_PAGE_CHANGED, &StandardExpInitPage::onFinishEvent, this);

    matnumTextCtrl->SetValue(std::to_string(wizard->experiment->mats));
    mteTextCtrl->SetValue(std::to_string(wizard->experiment->mts));
    SetSizerAndFit(mainSizer);
	
}

void StandardExpInitPage::onFinishEvent(wxWizardEvent & event) {
    ENDFReader reader(wizard->fileName);
    wizard->experiment->mats = wxAtoi(matnumTextCtrl->GetValue());
	auto pair = reverseLookup(&endfnums, wizard->experiment->mats);
	wizard->experiment->nampl = pair.first;
    wizard->experiment->mts = wxAtoi(mteTextCtrl->GetValue());
    wizard->experiment->mtpl = wizard->experiment->mts;
    wizard->experiment->ismg = m_radio->GetSelection();
    
    if (wizard->imode == STANDARDS) {
        try {
            reader.read_cs(3, wizard->experiment->mts, wizard->csData);
        }
        catch (const std::invalid_argument& e) {
            wxMessageBox(e.what());
            event.Veto();
            return;
        }

        wizard->experiment->nps = wizard->csData.len;
        //wizard->experiment->nampl = wizard->csData.NA;

        if (!wizard->csData.len) {
            wxMessageBox("Reaction not found in supplied PENDF file.");
            event.Veto();
        }
    }
}

AddConvPage::AddConvPage(wxWizard *parent) : wxWizardPageSimple(parent) {
    wizard = dynamic_cast<AddMaterialWizard*>(parent);
    wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(leftSizer, 1, wxEXPAND);
    mainSizer->Add(rightSizer, 1, wxEXPAND);
    sCovarianceList = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    rightSizer->Add(sCovarianceList, 1, wxEXPAND);

    AddCovarianceButton = new wxButton(this, wxID_OK, "Add Covariance");
    RemoveCovarianceButton = new wxButton(this, wxID_ANY, "Remove covariance");

    leftSizer->Add(new wxStaticText(this, wxID_ANY, "Use the "
        "\"Add Covariance\" and \"Remove Covariance\"\n buttons to add "
        "additional covariances."));
    leftSizer->Add(AddCovarianceButton);
    leftSizer->Add(RemoveCovarianceButton);

    Connect(AddCovarianceButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(AddConvPage::onAddCovarianceButtonClicked));
    Connect(RemoveCovarianceButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(AddConvPage::onRemoveCovarianceButtonClicked));
    Connect(sCovarianceList->GetId(), wxEVT_LISTBOX_DCLICK,
        wxCommandEventHandler(AddConvPage::onListItemDoubleClicked));
    Bind(wxEVT_WIZARD_BEFORE_PAGE_CHANGED, &AddConvPage::onFinishEvent, this);

    SetSizerAndFit(mainSizer);
    loadValues();
}

void AddConvPage::onAddCovarianceButtonClicked(wxCommandEvent & event) {
    auto range = std::make_pair(1, wizard->csData.len);
    additional_cov_data covData;
    CovDialog dialog("Add covariance", range, &covData);
    if (dialog.ShowModal() == wxID_OK) {
        wizard->experiment->cov_data.push_back(*dialog.covData);
        //int elements = sCovarianceList->GetCount();
        std::string set_string = std::to_string(dialog.covData->covfc);
        set_string += " from " + std::to_string(dialog.covData->range.first);
        set_string += " to " + std::to_string(dialog.covData->range.second);
        if (dialog.covData->irelfc)
            set_string += " Rel";
        else
            set_string += " Abs";
        sCovarianceList->Insert(set_string, 0);
    }
}

void AddConvPage::loadValues() {
    additional_cov_data covData;
    covData.range.second = wizard->csData.len;
    CovDialog dialog("Add covariance", covData.range, &covData);
    for (auto cov : wizard->experiment->cov_data) {
        std::string set_string = std::to_string(cov.covfc);
        set_string += " from " + std::to_string(cov.range.first);
        set_string += " to " + std::to_string(cov.range.second);
        if (cov.irelfc)
            set_string += " Rel";
        else
            set_string += " Abs";
        sCovarianceList->Insert(set_string, 0);
    }
}


void AddConvPage::onRemoveCovarianceButtonClicked(wxCommandEvent & event) {
    auto selection = sCovarianceList->GetSelection();
    if (selection != wxNOT_FOUND) {
        sCovarianceList->Delete(selection);
        wizard->experiment->cov_data.erase(
            wizard->experiment->cov_data.begin() + selection);
    }
}

void AddConvPage::onListItemDoubleClicked(wxCommandEvent & event) {
    auto config_no = event.GetInt();
    auto covData = wizard->experiment->cov_data.at(config_no);
    CovDialog dialog("Add covariance", covData.range, &covData);
    if (dialog.ShowModal() == wxID_OK) {
        std::string set_string = std::to_string(dialog.covData->covfc);
        set_string += " from " + std::to_string(dialog.covData->range.first);
        set_string += " to " + std::to_string(dialog.covData->range.second);
        sCovarianceList->SetString(config_no, set_string);
        wizard->experiment->cov_data.at(config_no) = covData;
    }
}

void AddConvPage::onFinishEvent(wxWizardEvent & event) {
    if (!wizard->experiment->ismg)
        wizard->finished = true;
    wizard->redoDataGrid();
}

DataPointsPage::DataPointsPage(wxWizard *parent) : wxWizardPageSimple(parent) {
    wizard = dynamic_cast<AddMaterialWizard*>(parent);
    wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(leftSizer, 1, wxEXPAND);
    mainSizer->Add(rightSizer, 1, wxEXPAND);

    wxStaticText *instructionText = new wxStaticText(this, wxID_ANY,
        "For each data point in the experiment, mark:\n"
        "0 - if the corresponding data value in file 3 is "
        "an energy dependent cross section,\n"
        "ng - if the corresponding data value in file 3 is "
        "a multigroup cross section (units b) averaged over "
        "ng-th energy group considered by sepost"
        "(see file mgbse.dat)\n"
        "-ng - if the corresponding data value in file 3 "
        "has been integrated over the energy range of the ng-"
        "th energy group (units b-eV)");
    leftSizer->Add(instructionText, 1, wxEXPAND);
    dataGrid = new wxGrid(this, wxID_ANY);
    dataGrid->CreateGrid(1, 1);
    dataGrid->HideColLabels();

    rightSizer->Add(dataGrid, 1, wxEXPAND | wxALL);
    rightSizer->Add(new wxPanel(this), 1, 0);
    instructionText->Wrap(0.5 * wizard->wSize.GetWidth());

    Bind(wxEVT_WIZARD_BEFORE_PAGE_CHANGED, &DataPointsPage::onFinishEvent, this);

    SetSizerAndFit(mainSizer);
    dataGrid->SetRowLabelSize(0.1 * wizard->wSize.GetWidth());
    dataGrid->SetColSize(0, 0.4 * wizard->wSize.GetWidth()-20);
}

void DataPointsPage::onFinishEvent(wxWizardEvent & event) {
    for (int i = 0; i < wizard->experiment->nps; i++) {
        int data=0;
        try {
            data = std::atoi(dataGrid->GetCellValue(i, 0).c_str());
        }
        catch (...) {
            wxMessageBox("Invalid entry in the table");
            event.Veto();
        }
        wizard->experiment->mgrid.at(i) = data;
    }
}

AdditionalSensPage::AdditionalSensPage(wxWizard *parent) : wxWizardPageSimple(parent) {
    wizard = dynamic_cast<AddMaterialWizard*>(parent);

    wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(leftSizer, 1, wxEXPAND);
    mainSizer->Add(rightSizer, 1, wxEXPAND);
    sensListBox = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    rightSizer->Add(sensListBox, 1, wxEXPAND);

    AddSensButton = new wxButton(this, wxID_ANY, "Add Sensitivty");
    RemoveSensButton = new wxButton(this, wxID_ANY, "Remove Sensitivity");

    wxStaticText *instructions = new wxStaticText(this, wxID_ANY, "Use \"Add Sensitivity\" "
        "to add additional sensitivities to a new material. "
        "");
    leftSizer->Add(instructions);
    leftSizer->Add(AddSensButton);
    leftSizer->Add(RemoveSensButton);
    // 570 is wizard size, replace with reference
    instructions->Wrap(0.5 * 570);


    Connect(AddSensButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(AdditionalSensPage::onAddSensButtonClicked));
    Connect(RemoveSensButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(AdditionalSensPage::onRemoveSensButtonClicked));
    Connect(sensListBox->GetId(), wxEVT_LISTBOX_DCLICK,
        wxCommandEventHandler(AdditionalSensPage::onListItemDoubleClicked));

    Bind(wxEVT_WIZARD_BEFORE_PAGE_CHANGED, &AdditionalSensPage::onFinishEvent, this);

    SetSizerAndFit(mainSizer);
    loadValues();
}

void AdditionalSensPage::onAddSensButtonClicked(wxCommandEvent & event) {
    additional_sens_data sens_data = additional_sens_data();
    SensDialog addSensDialog("Add sensitivity to material", &sens_data);
    if (addSensDialog.ShowModal() == wxID_OK) {
        wizard->experiment->sens_data.push_back(*addSensDialog.sensData);
        int elements = sensListBox->GetCount();
        std::string set_string = "Sens to " + std::to_string(addSensDialog.sensData->nam);
        sensListBox->Insert(set_string, 0);
    }
}

void AdditionalSensPage::onRemoveSensButtonClicked(wxCommandEvent & event) {
    auto selection = sensListBox->GetSelection();
    if (selection != wxNOT_FOUND) {
        sensListBox->Delete(selection);
        wizard->experiment->sens_data.erase(
            wizard->experiment->sens_data.begin() + selection);
    }
}

void AdditionalSensPage::onFinishEvent(wxWizardEvent & event) {
    wizard->finished = true;
}

void AdditionalSensPage::loadValues() {

    for (auto s : wizard->experiment->sens_data) {
        std::string set_string = "Sens to " + std::to_string(s.nam);
        sensListBox->Insert(set_string, 0);
    }
}

void AdditionalSensPage::onListItemDoubleClicked(wxCommandEvent & event) {
    auto config_no = event.GetInt();
    auto sensData = wizard->experiment->sens_data.at(config_no);
    SensDialog addSensDialog("Add sensitivity to material", &sensData);
    if (addSensDialog.ShowModal() == wxID_OK) {
        std::string set_string = "Sens to " + std::to_string(sensData.nam);
        sensListBox->SetString(config_no, set_string);
        wizard->experiment->sens_data.at(config_no) = sensData;
    }
}

void setGridRowNumbers(wxGrid *grid, int no_rows) {
    int diff_row = no_rows - grid->GetNumberRows();
    if (diff_row == 0)
        return;
    else if (diff_row > 0)
        grid->AppendRows(diff_row);
    else
        grid->DeleteRows(0, std::abs(diff_row));
    return;
}
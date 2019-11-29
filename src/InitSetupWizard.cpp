/***************************************************************
 * Name:      InitSetupWizard.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for IinitSetupWizard class.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#include "InitSetupWizard.h"
#include "csv.h"

InitSetupWizard::InitSetupWizard(wxWindow *parent, GanrunConfiguration *config) :
    wxWizard(parent, wxID_ANY, "Setup environment", wxNullBitmap, wxDefaultPosition,
        wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER), config(config) {

    m_page1 = new wxInitFilesPage(this);
    m_reactions = new wxSetReactionsPage(this);
    m_gridEdit = new wxGridEditPage(this);
    Bind(wxEVT_WIZARD_FINISHED, &InitSetupWizard::onFinishEvent, this);

    config->iloop = 1;
    config->imode = CLASSIC;
    config->inopt = 0;
    config->ipick = 0;
    config->idef = REUSE;
    config->iprint = 0;
    config->iredo = 0;
    m_page1->SetNext(m_reactions);
    m_reactions->SetPrev(m_page1);
    m_reactions->SetNext(m_gridEdit);
    m_gridEdit->SetPrev(m_reactions);
    threshMatName = config->threshName;
    SetPageSize(wxSize(530, 650));
}

void InitSetupWizard::UpdateReactions() {
    wxSetReactionsPage *mr = dynamic_cast<wxSetReactionsPage*>(m_reactions);
    mr->update_reactions();
}

InitSetupWizard::~InitSetupWizard() {
}

void InitSetupWizard::onFinishEvent(wxWizardEvent & event) {
    config->label = "Setup run";
    finished = true;
}

wxWizardPage *InitSetupWizard::GetNextPage(short unsigned int page) {
    switch (page) {
    case 0:
        return m_gridEdit;
    case 1:
        return m_reactions;
    }
}

wxWizardPage *InitSetupWizard::GetPrevPage() {
    if (config->custom_reactions)
        return m_reactions;
    else
        return m_page1;
}


std::array<reaction, NRMAX> ReadReactions(std::ifstream &stream) {
    std::string line;
    std::array<reaction, NRMAX> reactions;
    while (std::getline(stream, line)) {
        auto splitted = split(line);
        std::stringstream name;
        std::vector<int> composition;
        reaction current;
        for (auto it = splitted.begin() + 5; it != splitted.end(); it++) {
            name << *it;
        }
        int mt = std::stoi(splitted.at(1));
        int num = std::stoi(splitted.at(0));
        current.label = name.str();
        current.mte = mt;
        if (mt < 0) {
            while (composition.size() < std::abs(mt)) {
                std::getline(stream, line);
                try {
                    std::vector<double> reac_numbers = valFromString<double>(line);
                    composition.insert(composition.end(), reac_numbers.begin(), reac_numbers.end());
                }
                catch (...) {
                    wxMessageBox("Reaction composition for " + name.str() + "not valid");
                }
            }
            current.composition = composition;
        }
        if ((num-1) < NRMAX)
            reactions.at(num-1) = current;
    }

    return reactions;
}

wxInitFilesPage::wxInitFilesPage(wxWizard *parent) : wxWizardPageSimple(parent) {
    wizard = dynamic_cast<InitSetupWizard*>(parent);
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hSizer1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *hSizer2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *hSizer3 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *hSizer4 = new wxBoxSizer(wxHORIZONTAL);
    wxSize smallBoxSize(60, 20);

    wxStaticText *matText = new wxStaticText(this, wxID_ANY, "Please enter "
        "the material ZA number");
    mainSizer->Add(matText, 0, wxALL, 10);
    matReacBox = new wxTextCtrl(this, wxID_ANY, wizard->threshMatName,
        wxDefaultPosition, smallBoxSize);
    matReacBox->SetValue(wizard->config->threshName);
    mainSizer->Add(matReacBox, 0, wxALL, 10);

    CustomButtonPENDF = new wxButton(this, wxID_ANY, wxT("Set custom PENDF file     "));
    CustomButtonCOV = new wxButton(this, wxID_ANY, wxT("Set custom covariance matrix"));

    FileBrowseButtonPENDF = new wxButton(this, wxID_ANY, wxT("Browse PENDF"));
    FileBrowseButtonPENDF->Enable(false);
    FileBrowseButtonCOV = new wxButton(this, wxID_ANY, wxT("Browse COV"));
    FileBrowseButtonCOV->Enable(false);

    PENDFFileBox = new wxTextCtrl(this, wxID_ANY, PENDFFileName, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    PENDFFileBox->Enable(false);
    COVFileBox = new wxTextCtrl(this, wxID_ANY, COVFileName, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    COVFileBox->Enable(false);

    openFileDialog = new wxFileDialog(this);

    wxStaticText *pendfText = new wxStaticText(this, wxID_ANY, "By default, GANDR will use default"
        "PENDF file, located in the "
        "ZOTTVL directory.\nPress \"Set custom PENDF file\" to locale a custom file.");

    wxStaticText *covText = new wxStaticText(this, wxID_ANY, "By default, GANDR will use default"
        "covariance matrix, located in the "
        "ZOTTVL directory.\nPress \"Set custom covariance matrix\" to locale a custom file.");
    if (wizard->config->label == "Setup run") {
        PENDFFileBox->SetValue(wizard->config->pendfFileName);
        COVFileBox->SetValue(wizard->config->covFileName);
    }
    pendfText->Wrap(pendfText->GetClientSize().x);
    mainSizer->Add(pendfText, 0, wxALL, 10);
    mainSizer->Add(CustomButtonPENDF, 0, wxALL, 0);
    mainSizer->Add(hSizer2, 0, wxALL, 10);
    hSizer2->Add(PENDFFileBox);
    hSizer2->Add(FileBrowseButtonPENDF);
    mainSizer->AddStretchSpacer(0.05);

    pendfText->Wrap(pendfText->GetClientSize().x);
    mainSizer->Add(covText, 0, wxALL, 10);
    mainSizer->Add(CustomButtonCOV, 0, wxALL, 0);
    mainSizer->Add(hSizer4, 0, wxALL, 10);
    hSizer4->Add(COVFileBox);
    hSizer4->Add(FileBrowseButtonCOV);

    wxBoxSizer *reacSizer = new wxBoxSizer(wxVERTICAL);
    ReacSetupBox[0] = new wxRadioButton(this, 0, "Use default reaction definitions");
    wxBoxSizer *reacEditSizer = new wxBoxSizer(wxHORIZONTAL);
    ReacSetupBox[1] = new wxRadioButton(this, 1, "Enter custom reactions");

    reacEditSizer->Add(ReacSetupBox[1]);
    //reacEditSizer->Add(matReacBox);
    reacSizer->Add(ReacSetupBox[0], 1, wxEXPAND | wxALL);
    reacSizer->Add(reacEditSizer, 1, wxEXPAND | wxALL);
    mainSizer->Add(reacSizer);
    
    if (wizard->config->igrid >= 0)
        ReacSetupBox[0]->SetValue(true);
    else
        ReacSetupBox[1]->SetValue(true);

	if (wizard->config->ipend) {
		FileBrowseButtonPENDF->Enable(true);
		PENDFFileBox->Enable(true);
		CustomButtonPENDF->SetLabel("Set default PENDF file");
	}

	if (wizard->config->imoc) {
		FileBrowseButtonCOV->Enable(true);
		COVFileBox->Enable(true);
		CustomButtonCOV->SetLabel("Set default covariance matrix");
	}

    SetSizerAndFit(mainSizer);
    Connect(CustomButtonPENDF->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(wxInitFilesPage::PENDFCustomButtonClick));
    Connect(FileBrowseButtonPENDF->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(wxInitFilesPage::PENDFBrowseButtonClick));
    Connect(PENDFFileBox->GetId(), wxEVT_COMMAND_TEXT_ENTER,
        wxCommandEventHandler(wxInitFilesPage::PENDFFileNameEntered));

    Connect(CustomButtonCOV->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(wxInitFilesPage::COVCustomButtonClick));
    Connect(FileBrowseButtonCOV->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(wxInitFilesPage::COVBrowseButtonClick));
    Connect(COVFileBox->GetId(), wxEVT_COMMAND_TEXT_ENTER,
        wxCommandEventHandler(wxInitFilesPage::COVFileNameEntered));

    Bind(wxEVT_WIZARD_PAGE_CHANGING, &wxInitFilesPage::onNextEvent, this);

}

void wxInitFilesPage::PENDFCustomButtonClick(wxCommandEvent & event) {
    if (FileBrowseButtonPENDF->IsEnabled()) {
        FileBrowseButtonPENDF->Enable(false);
        PENDFFileBox->Enable(false);
        CustomButtonPENDF->SetLabel("Set custom PENDF file");
    }
    else {
        FileBrowseButtonPENDF->Enable(true);
        PENDFFileBox->Enable(true);
        CustomButtonPENDF->SetLabel("Set default PENDF file");
    }
}

void wxInitFilesPage::COVCustomButtonClick(wxCommandEvent & event) {
    if (FileBrowseButtonCOV->IsEnabled()) {
        FileBrowseButtonCOV->Enable(false);
        COVFileBox->Enable(false);
        CustomButtonCOV->SetLabel("Set custom covariance matrix");
    }
    else {
        FileBrowseButtonCOV->Enable(true);
        COVFileBox->Enable(true);
        CustomButtonCOV->SetLabel("Set default covariance matrix");
    }
}

void wxInitFilesPage::PENDFBrowseButtonClick(wxCommandEvent& event) {
    if (openFileDialog->ShowModal() == wxID_OK) {
        PENDFFileName = openFileDialog->GetPath();
        PENDFFileBox->ChangeValue(PENDFFileName);
    }
}

void wxInitFilesPage::PENDFFileNameEntered(wxCommandEvent& event) {
    PENDFFileName = PENDFFileBox->GetValue();
}

void wxInitFilesPage::COVBrowseButtonClick(wxCommandEvent& event) {
    if (openFileDialog->ShowModal() == wxID_OK) {
        COVFileName = openFileDialog->GetPath();
        COVFileBox->ChangeValue(COVFileName);
    }
}

void wxInitFilesPage::COVFileNameEntered(wxCommandEvent& event) {
    COVFileName = COVFileBox->GetValue();
}

void wxInitFilesPage::onNextEvent(wxWizardEvent & event) {
    wizard->config->ipend = FileBrowseButtonPENDF->IsEnabled() ? 1 : 0;
    wizard->config->pendfFileName = PENDFFileBox->GetValue();
    wizard->config->imoc = FileBrowseButtonCOV->IsEnabled() ? 1 : 0;
    wizard->config->covFileName = COVFileBox->GetValue();
    wizard->config->custom_reactions = ReacSetupBox[1]->GetValue();
    wizard->threshMatName = matReacBox->GetValue();
    wizard->config->threshName = wizard->threshMatName;
    try
    {
        wizard->config->matnam = std::stoi(std::string(wizard->threshMatName.mbc_str()));
    }
    catch (const std::exception&)
    {
        wxMessageBox("Please enter a valid material name.");
        event.Veto();
    }
    
    if (wizard->config->matnam < 1e3 || wizard->config->matnam > 1e6) {
        wxMessageBox("Please enter a valid material name.");
        event.Veto();
    }
    wizard->UpdateReactions();
}

void wxSetReactionsPage::update_reactions() {
    Singleton* s = Singleton::getInstance();
    std::string threshLocation = std::string(s->zottDir.mb_str()) +
        "/thresh/" + wizard->config->threshName;
    std::ifstream thFile(threshLocation);
    reactions = ReadReactions(thFile);
    thFile.close();
}

wxSetReactionsPage::~wxSetReactionsPage() {
}

void wxSetReactionsPage::onFinishEvent(wxWizardEvent & event) {
    reactionsFromBoxes();
    wizard->config->reactions = reactions;
}

void wxSetReactionsPage::reactionsFromBoxes() {
    for (int i = 0; i < reactions.size(); i++) {
        reactions[i].label = reacPane->nameBoxes.at(i)->GetValue();
        reactions[i].composition = valFromString<int>(std::string(reacPane->mtBoxes.at(i)->GetValue().mb_str()));
        reactions[i].gan_no = i + 1;
        if (reactions[i].composition.size() == 1) {
            reactions[i].mte = reactions[i].composition.at(0);
        }
        else {
            reactions[i].mte = -static_cast<int>(reactions[i].composition.size());
        }
    }
}


ScrolledWidgetsPane::ScrolledWidgetsPane(wxWindow* parent, wxWindowID id, std::array<reaction, NRMAX> *reactions) : wxScrolledWindow(parent, id) {
    page = dynamic_cast<wxSetReactionsPage*>(parent);
    gs = new wxFlexGridSizer(26, 3, 5, 5);
    gs->Add(new wxStaticText(this, wxID_ANY, "No."));
    gs->Add(new wxStaticText(this, wxID_ANY, "Label"));
    gs->Add(new wxStaticText(this, wxID_ANY, "mt"));
    for (int i = 0; i < reactions->size(); i++) {
        gandrNumbers.push_back(new wxStaticText(this, wxID_ANY, std::to_string(i + 1)));
        nameBoxes.push_back(new wxTextCtrl(this, wxID_ANY, reactions->at(i).label));
        wxSize size = nameBoxes.back()->GetSize();
        size.SetWidth(300);
        nameBoxes.back()->SetMinSize(size);
        if (reactions->at(i).mte > 0)
            mtBoxes.push_back(new wxTextCtrl(this, wxID_ANY, std::to_string(reactions->at(i).mte)));
        else {
            std::stringstream ss;
            for (auto mt : reactions->at(i).composition) {
                ss << mt << " ";
            }
            mtBoxes.push_back(new wxTextCtrl(this, wxID_ANY, ss.str()));
        }
        gs->Add(gandrNumbers.back());
        size.SetWidth(200);
        mtBoxes.back()->SetMinSize(size);
        gs->Add(nameBoxes.back());
        gs->Add(mtBoxes.back(), wxEXPAND);
    }
    gs->AddGrowableCol(2, 1);
    SetSizer(gs);

    this->FitInside(); // ask the sizer about the needed size
    this->SetScrollRate(5, 5);
    this->ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_DEFAULT);
}


wxSetReactionsPage::wxSetReactionsPage(wxWizard *parent) : wxWizardPageSimple(parent) {
    wizard = dynamic_cast<InitSetupWizard*>(parent);
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    if (wizard->config->label == "Setup run" && wizard->config->igrid < 0) {
        reactions = wizard->config->reactions;
    }
    else {
        update_reactions();
    }
    reacPane = new ScrolledWidgetsPane(this, wxID_ANY, &reactions);
    
    mainSizer->Add(reacPane, 1, wxEXPAND);
    
    SetSizerAndFit(mainSizer);
    SetAutoLayout(true);
    

    Bind(wxEVT_WIZARD_BEFORE_PAGE_CHANGED, &wxSetReactionsPage::onFinishEvent, this);
}

wxGridEditPage::wxGridEditPage(wxWizard * parent) : wxWizardPageSimple(parent) {
    wizard = dynamic_cast<InitSetupWizard*>(parent);
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    wxArrayString choices;
    choices.Add("74 node grid");
    choices.Add("56 node grid");
    choices.Add("28 node grid");
    choices.Add("Custom");

    gridRadioBox = new wxRadioBox(this, wxID_ANY, "Select Grid type",
        wxDefaultPosition, wxDefaultSize, choices);
    if (wizard->config->label == "Setup run"){
        gridRadioBox->SetSelection(std::abs(wizard->config->igrid) - 1);
    }
    mainSizer->Add(gridRadioBox);
    wxBoxSizer *thinSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(thinSizer);
    wxArrayString choicesYESNO;
	choicesYESNO.Add("No");
	choicesYESNO.Add("Yes");
    thinRadioBox = new wxRadioBox(this, wxID_ANY, "Automatic thinning: ",
        wxDefaultPosition, wxDefaultSize, choicesYESNO);
    wxStaticText *thText = new wxStaticText(this, wxID_ANY, "Value: ");
    thinValue = new wxTextCtrl(this, wxID_ANY, std::to_string(wizard->config->thin));
    if (wizard->config->thin > 0){
        thinRadioBox->SetSelection(1);
        thinValue->SetValue(std::to_string(wizard->config->thin));
		thinValue->Enable(true);
    } else {
        thinRadioBox->SetSelection(0);
        thinValue->SetValue(std::to_string(0));
		thinValue->Enable(false);
    }
    thinSizer->Add(thinRadioBox);
    thinSizer->Add(thinValue);


    SetSizerAndFit(mainSizer);
    
    Connect(gridRadioBox->GetId(), wxEVT_RADIOBOX,
        wxCommandEventHandler(wxGridEditPage::onRadioBox));
    Bind(wxEVT_WIZARD_BEFORE_PAGE_CHANGED, &wxGridEditPage::onFinishEvent, this);
	Connect(thinRadioBox->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED,
		wxCommandEventHandler(wxGridEditPage::listboxToggled));

}

void wxGridEditPage::listboxToggled(wxCommandEvent& event) {
	if (thinRadioBox->GetSelection()) {
		thinValue->Enable(true);
		gridRadioBox->SetSelection(3);
	}
	else {
		thinValue->Enable(false);
		gridRadioBox->SetSelection(0);
	}
	wizard->config->igrid = gridRadioBox->GetSelection() + 1;
}

void wxGridEditPage::onFinishEvent(wxWizardEvent & event) {
    wizard->config->igrid = gridRadioBox->GetSelection() + 1;

    if (thinRadioBox->GetSelection()){
		wizard->config->thin = wxAtoi(thinValue->GetValue());
    }

    if (wizard->config->custom_reactions)
        wizard->config->igrid = -abs(wizard->config->igrid);
    else
        wizard->config->igrid = abs(wizard->config->igrid);
}

wxGridEditPage::~wxGridEditPage() {
}

void wxGridEditPage::onRadioBox(wxCommandEvent & event) {
    if (gridRadioBox->GetSelection() < 3) {
        wizard->config->igrid = gridRadioBox->GetSelection() + 1;
		thinValue->Enable(false);
		thinValue->SetValue("0");
    }
    else {
		wizard->config->thin = 0;
        GridEditDialog grid_dialog("Edit grid", &wizard->config->iflex);
        if (grid_dialog.ShowModal() == wxID_OK) {
            wizard->config->igrid = 4;

            wizard->config->iflex = grid_dialog.gridPanel->iflex;
        }
        else
            gridRadioBox->SetSelection(0);
        
    }
}

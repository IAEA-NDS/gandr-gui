/***************************************************************
 * Name:      AddRunWizard.cpp
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Source code for AddRunWizard, a wizard that 
 *            allows the user to add new GANDR runs.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#include "AddRunWizard.h"


AddRunWizard::AddRunWizard(wxWindow *parent, GanrunConfiguration *configuration, GanrunConfiguration *initConfig): 
    config(configuration){
    Create(parent, wxID_ANY, "Add run wizard",
        wxNullBitmap, wxDefaultPosition,
        wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    eReader = new ExforReader("");
    m_page1 = new wxRunSelectPage(this);
    m_exfor = new wxSetExforPage(this);
    m_exfor_cov = new wxSetExforCovPage(this);
    m_stand = new wxAddStandardPage(this);
    m_integ = new wxAddIntegralPage(this);
    m_sexp = new AddStandardExperimentsPage(this);
    m_page1->SetNext(m_exfor);
    m_exfor->SetNext(m_exfor_cov);
    m_exfor->SetPrev(m_page1);
    m_exfor_cov->SetPrev(m_exfor);
    m_stand->SetPrev(m_page1);
    m_stand->SetNext(m_sexp);
    m_sexp->SetPrev(m_page1);
    m_integ->SetPrev(m_page1);
 
    SetPageSize(wSize);
    Bind(wxEVT_WIZARD_FINISHED, &AddRunWizard::onFinishEvent, this);

}

wxWizardPage *AddRunWizard::GetNextPage(short unsigned int page) {
    switch (page) {
    case M_CLASSIC:
        return m_exfor;
    case M_STANDARDS:
        return m_stand;
    case M_MIXED:
        return m_exfor;
    case M_STANDARDS_NEXT:
        return m_sexp;
	case M_INTEGRAL:
		return m_integ;
    default:
        return m_exfor;
    }
}

void AddRunWizard::onFinishEvent(wxWizardEvent & event) {
    if ((config->exp_data.size() && config->imode == 10) 
        || config->imode == 11)
        finished = true;
    if (finished)
        config->genLabel();
}

wxRunSelectPage::wxRunSelectPage(wxWizard *parent) : wxWizardPageSimple(parent) {
    wizard = dynamic_cast<AddRunWizard*>(parent);
    wxString choices[4];
    choices[M_CLASSIC] = "Exfor data (imode=12)";
    choices[M_MIXED] = "Mixed mode data (imode=11)";
    choices[M_STANDARDS] = "Standards data (imode=10)";
	choices[M_INTEGRAL] = "Integral data (imode=15)";

    m_radio = new wxRadioBox(this, wxID_ANY, "I want to fit:",
        wxDefaultPosition, wxDefaultSize,
        WXSIZEOF(choices), choices,
        1, wxRA_SPECIFY_COLS);

    switch (wizard->config->imode) {
    case CLASSIC:
        m_radio->SetSelection(M_CLASSIC);
        break;
    case MIXTURE:
        m_radio->SetSelection(M_MIXED);
        break;
    case STANDARDS:
        m_radio->SetSelection(M_STANDARDS);
	case INTEGRAL:
		m_radio->SetSelection(M_INTEGRAL);
    }

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(
        m_radio,
        0, // No stretching
        wxALL,
        5 // Border
    );
    m_next = wizard->GetNextPage(CLASSIC);


    wxString msg("Option \"Exfor data\" allows you to use differential "
        "data in computational format.\n"
        "Choose \"Standards data\" to provide standards in ENDF format.\n"
        "Selecting \"Integral data\" fitting of experiment data "
        "with custom sensitivities.");

    mainSizer->Add(
        new wxStaticText(this, wxID_ANY, msg),
        1, // No stretching
        wxALIGN_LEFT | wxEXPAND,
        5 // Border
    );

    mainSizer->Add(
        new wxStaticText(this, wxID_ANY, "Adjust the most negative parameter\n adjustment permitted: "),
        0, // No stretching
        wxALIGN_TOP,
        5 // Border
    );

    xnegTextCtrl = new wxTextCtrl(this, wxID_ANY,
        std::to_string(wizard->config->xneg));
    mainSizer->Add(xnegTextCtrl, 0, wxALIGN_TOP);
    mainSizer->Add(0, 1, wxEXPAND);

    Bind(wxEVT_WIZARD_BEFORE_PAGE_CHANGED, &wxRunSelectPage::onFinishEvent, this);
    Bind(wxEVT_SIZE, &wxRunSelectPage::onResize, this);

    SetSizerAndFit(mainSizer);
}

void wxRunSelectPage::onFinishEvent(wxWizardEvent & event) {

    if (!event.GetDirection())
        return;

    if (m_radio->GetSelection() == M_CLASSIC) {
        wizard->config->ipick = 0;
        wizard->config->imode = CLASSIC;
    }
    
    if (m_radio->GetSelection() == M_STANDARDS)
        wizard->config->imode = STANDARDS;
    
    if (m_radio->GetSelection() == M_MIXED) 
        wizard->config->imode = MIXTURE;

	if (m_radio->GetSelection() == M_INTEGRAL) {
		wizard->config->imode = INTEGRAL;
	}

    std::string textval = std::string(xnegTextCtrl->GetValue());
    wizard->config->xneg = std::stof(textval);
}


wxSetExforPage::wxSetExforPage(wxWizard *parent) : wxWizardPageSimple(parent) {
    wizard = dynamic_cast<AddRunWizard*>(parent);
	fileName = wizard->config->exforFileName;

    wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
    
    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
    materialBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    reactionBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

    mainSizer->Add(leftSizer, 1, wxEXPAND);

    leftSizer->Add(new wxStaticText(this, wxID_ANY, "ZA number\n10000*A+Z"));
    leftSizer->Add(materialBox);

    leftSizer->Add(new wxStaticText(this, wxID_ANY, "Reaction MT number"));
    leftSizer->Add(reactionBox);

    wxString exforChoices[2];
    exforChoices[0] = "Deduce reaction def. from MT";
    exforChoices[1] = "Define exfor quantitiy manually";

    exforDefRadioBox = new wxRadioBox(this, wxID_ANY, "Exfor quantity definition",
        wxDefaultPosition, wxDefaultSize, WXSIZEOF(exforChoices), exforChoices, 0, wxRA_SPECIFY_ROWS);
    leftSizer->Add(exforDefRadioBox);

    wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(rightSizer, 1, wxEXPAND);
    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *upSizer = new wxBoxSizer(wxHORIZONTAL);
    FileBrowseButton = new wxButton(this, wxID_ANY, wxT("Browse"));

    ExforFileBox = new wxTextCtrl(this, wxID_ANY, fileName, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    openFileDialog = new wxFileDialog(this);
    checklist = new wxCheckListBox(this, wxID_ANY);
    checklist->SetFont(wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    rightSizer->Add(upSizer, 0, wxEXPAND);
    rightSizer->Add(bottomSizer, 1, wxEXPAND);
    upSizer->Add(ExforFileBox, 1, wxEXPAND, 5);
    upSizer->Add(FileBrowseButton, 0, wxEXPAND, 5);

    globalCovButton = new wxButton(this, wxID_ANY, "Set global covar.");
    leftSizer->Add(globalCovButton);

    selectAllButton = new wxButton(this, wxID_ANY, "Select all reactions");
    rightSizer->Add(selectAllButton, 0, wxALL);

    leftSizer->Add(new wxStaticText(this, wxID_ANY, "Press \"Exfor from MAT\" to use the "
        "MAT number\n to use the EXFOR file in the ZOTTVL directory."), 0, wxALL);
    exforFromMatButton = new wxButton(this, wxID_ANY, "EXFOR from MAT");
    leftSizer->Add(exforFromMatButton, 0, wxALL);

    bottomSizer->Add(checklist, 1, wxEXPAND, 5);

    SetSizerAndFit(mainSizer);

    Connect(selectAllButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(wxSetExforPage::selectAllButtonClicked));
    Connect(FileBrowseButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(wxSetExforPage::ExforBrowseButtonClick));
    Connect(ExforFileBox->GetId(), wxEVT_COMMAND_TEXT_ENTER,
        wxCommandEventHandler(wxSetExforPage::fileNameEntered));
    Connect(materialBox->GetId(), wxEVT_COMMAND_TEXT_ENTER,
        wxCommandEventHandler(wxSetExforPage::materialNameEntered));
    Connect(reactionBox->GetId(), wxEVT_COMMAND_TEXT_ENTER,
        wxCommandEventHandler(wxSetExforPage::reactionNameEntered));
    Connect(globalCovButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(wxSetExforPage::advancedCovButtonClicked));
    Connect(exforFromMatButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(wxSetExforPage::exforFromMatButtonClicked));
    Connect(checklist->GetId(), wxEVT_LISTBOX_DCLICK,
        wxCommandEventHandler(wxSetExforPage::onListItemDoubleClicked));
    Connect(exforDefRadioBox->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED,
        wxCommandEventHandler(wxSetExforPage::idefRadioBoxClicked));
    Bind(wxEVT_WIZARD_BEFORE_PAGE_CHANGED, &wxSetExforPage::onFinishEvent, this);

    if (wizard->config->mte > 0)
        exfor_from_zott = true;
    else
        exfor_from_zott = false;

    if (wxFileExists(fileName) && wizard->config->imode == CLASSIC) {
        wizard->eReader->fileName = fileName;
        //readExperiments();
		wxMessageBox("Loading values");
		loadValues();
    }
}

void wxSetExforPage::autofill_matnam_mte() {
    if (wizard->eReader->getTitles().size() && !wizard->config->matnam) {
		if (wizard->eReader->experiments.back().isotope > 0)
			materialBox->SetValue(std::to_string(wizard->eReader->experiments.back().isotope));
    }
    if (wizard->eReader->getTitles().size() && !wizard->config->mte) {
        wizard->config->mte = wizard->eReader->experiments.back().mt;
        reactionBox->SetValue(std::to_string(wizard->eReader->experiments.back().mt));
    }
}

void wxSetExforPage::onFinishEvent(wxWizardEvent & event) {
    if (!event.GetDirection())
        return;
    wxArrayInt  checked;
    checklist->GetCheckedItems(checked);
    wizard->config->exforFileName = fileName;

    if (!wizard->eReader->getTitles().size()) {
        wxMessageBox("No valid data in selected exfor");
        event.Veto();
        return;
    }

    if (!checked.size()) {
        wxMessageBox("No data selected");
        event.Veto();
        return;
    }

    wizard->config->listex.clear();

    wizard->config->listex.resize(static_cast<size_t>(checked.Last()) + 1, 0);
    wizard->config->thinning.resize(static_cast<size_t>(checked.Last()) + 1, 1);
    for (auto c : checked) {
        wizard->config->listex.at(c) = 1;
        wizard->config->listex.at(c) *= wizard->config->thinning.at(c);
    }

    wizard->config->ipick = 1;

    long matnam;
    if (!materialBox->GetValue().ToLong(&matnam)) {
        wxMessageBox("Material number not set");
        event.Veto();
        return;
    }
    wizard->config->matnam = matnam;
    if (!wizard->config->matnam) {
        wxMessageBox("Material number not set");
        event.Veto();
        return;
    }

    long reacnam;
    if (!reactionBox->GetValue().ToLong(&reacnam)) {
        wxMessageBox("Reaction number not set");
        event.Veto();
        return;
    }
    wizard->config->mte = reacnam;
    if (!exfor_from_zott)
        wizard->config->mte = -reacnam;
    if (!wizard->config->mte) {
        wxMessageBox("Reaction number not set");
        event.Veto();
        return;
    }
    wxSetExforCovPage *exforCovPage = dynamic_cast<wxSetExforCovPage*>(wizard->m_exfor_cov);
    auto lens = wizard->eReader->getLengths();
    auto titles_array = wizard->eReader->getTitles();
    std::vector<int> indexes;
    std::vector<std::string> titles;
	auto energies = wizard->eReader->getEnergies();
    indexes.push_back(0);
    for (size_t i = 0; i < checked.GetCount(); i++) {
            indexes.push_back(indexes.back() + lens.at(checked[i]));
            titles.push_back(std::string(titles_array[checked[i]].mb_str()));
    }
    exforCovPage->index_array = indexes;
    exforCovPage->title_array = titles;
	exforCovPage->energy_array = energies;
    exforCovPage->imode = wizard->config->imode;

    if (exforDefRadioBox->GetSelection() == 1)
        wizard->config->idef = LINEAR_COMBINATION;
    else
        wizard->config->idef = REUSE;
}

void wxSetExforPage::advancedCovButtonClicked(wxCommandEvent & event) {
    exfor_cov_data data;
    ExforCovDialog dialog("Edit global covariances", -1, 0, &data, 0);
    if (wizard->eReader->getTitles().size() == 0) {
        wxMessageBox("No exfor data loaded!");
        return;
    }
    wxMessageDialog are_you_sure(this, "This will override all local options",
        "Are you sure?", wxYES_NO | wxICON_EXCLAMATION);
    if (are_you_sure.ShowModal() != wxID_YES)
        return;
    if (dialog.ShowModal() == wxID_OK) {
        for (size_t i = 0; i < wizard->eReader->getTitles().size(); i++) {
            wizard->config->ecov_data.at(i) = data;
        }
    }
}

void wxSetExforPage::selectAllButtonClicked(wxCommandEvent & event) {
    for (size_t i = 0; i < checklist->GetCount(); i++)
        checklist->Check(i, true);
}

void wxSetExforPage::onListItemDoubleClicked(wxCommandEvent & event) {
    //exfor_cov_data data = wizard->config->GetCovData(event.GetInt());
    exfor_cov_data data = wizard->config->ecov_data.at(event.GetInt());
    int thinning = wizard->config->thinning.at(event.GetInt());
    ExforCovDialog dialog("Edit local covariances and data thinning", event.GetInt(), 
        wizard->eReader->experiments[event.GetInt()].d_energy.size(), &data, &thinning);
    if (dialog.ShowModal() == wxID_OK) {
        wizard->config->ecov_data.at(event.GetInt()) = data;
        wizard->config->thinning.at(event.GetInt()) = thinning;
    }
}

void wxSetExforPage::exforFromMatButtonClicked(wxCommandEvent & event) {
    exfor_from_zott = true;
	if (wxAtoi(materialBox->GetValue()) >  1e4)
		readExperiments();
}

void wxSetExforPage::readExperiments() {
    delete wizard->eReader;
    if (!exfor_from_zott)
        wizard->eReader = new ExforReader(fileName);
    else {
        Singleton* s = Singleton::getInstance();
        fileName = s->zottDir;
        std::string slash = "/";
#ifdef __WXMSW__
        slash = "\\";
#endif
        if (fileName.back() == '/')
            fileName += "exfor" + slash + materialBox->GetValue();
        else
            fileName += slash + "exfor" + slash + materialBox->GetValue();
        wizard->eReader = new ExforReader(fileName);
    }
    if (!wxFileExists(fileName)) {
        wxMessageBox("File with experiments not found.");
        return;
    }
    wizard->config->exforFileName = fileName;
    try { 
        wizard->eReader->read();
    }
    catch (...) {
        wxMessageBox("Exception while reading exfor file.");
        return;
    }
    checklist->Set(wizard->eReader->getTitles());
    if (wizard->eReader->getTitles().size() != wizard->config->ecov_data.size())
        wizard->config->InitExforCovData(wizard->eReader->getTitles().size());
    autofill_matnam_mte();
}

void wxSetExforPage::loadValues() {
    auto ipick = wizard->config->ipick;
    materialBox->SetValue(std::to_string(wizard->config->matnam));
    reactionBox->SetValue(std::to_string(std::abs(wizard->config->mte)));
    if (wxFileExists(wizard->config->exforFileName)) {
        fileName = wizard->config->exforFileName;
        ExforFileBox->SetValue(wizard->config->exforFileName);
        wizard->eReader->fileName = wizard->config->exforFileName;
        readExperiments();
        autofill_matnam_mte();
    }
    int range = std::min(wizard->config->listex.size(), wizard->eReader->getTitles().size());
    switch (ipick) {
    case 1:
        //ipickSelectBox->SetSelection(M_PICK_LOCAL);
        for (size_t i = 0; i < range; i++) {
            checklist->Check(i, wizard->config->listex.at(i));
        }
        ExforFileBox->Enable(true);
        FileBrowseButton->Enable(true);
        checklist->Enable(true);
        break;
    case -1:
        //ipickSelectBox->SetSelection(M_ALL_LOCAL);
        for (size_t i = 0; i < wizard->eReader->getTitles().size(); i++) {
            checklist->Check(i, true);
        }
        ExforFileBox->Enable(true);
        FileBrowseButton->Enable(true);
        checklist->Enable(false);
        break;
    case 0:
        //ipickSelectBox->SetSelection(M_REUSE);
        ExforFileBox->Enable(false);
        FileBrowseButton->Enable(false);
        checklist->Enable(false);
        break;

    }
}

void wxSetExforPage::ExforBrowseButtonClick(wxCommandEvent& event) {        
    if (openFileDialog->ShowModal() == wxID_OK) {
        fileName = openFileDialog->GetPath();
        ExforFileBox->ChangeValue(fileName);

        if (wxFileExists(fileName)) {
            wizard->eReader->fileName = fileName;
            exfor_from_zott = false;
            readExperiments();
        }
        for (size_t i = 0; i < wizard->eReader->getTitles().size(); i++) {
            checklist->Check(i, true);
        }
    }
    autofill_matnam_mte();

    }

void wxSetExforPage::idefRadioBoxClicked(wxCommandEvent & event) {
    IdefDialog *dialog;
    if (event.GetInt()) {
        if (wizard->config->igrid < 0)
            dialog = new IdefDialog("Exfor reaction definition", &wizard->config->xexfor, &wizard->config->reactions);
        else
            dialog = new IdefDialog("Exfor reaction definition", &wizard->config->xexfor, 0);
        if (dialog->ShowModal() != wxID_OK) {
            exforDefRadioBox->Select(0);
        }
        delete dialog;
    }
}

void wxSetExforPage::fileNameEntered(wxCommandEvent& event) {
    fileName = ExforFileBox->GetValue();
    if (wxFileExists(fileName)) {
        wizard->eReader->fileName = fileName;
        exfor_from_zott = false;
        readExperiments();
        for (size_t i = 0; i < wizard->eReader->getTitles().size(); i++) {
            checklist->Check(i, true);
        }
    }
    autofill_matnam_mte();
}

void wxSetExforPage::materialNameEntered(wxCommandEvent & event) {
    long matnam;
    if (materialBox->GetValue().ToLong(&matnam))
        wizard->config->matnam = static_cast<long>(matnam);
    else
        wxMessageBox("Matnam not valid");
}

void wxSetExforPage::reactionNameEntered(wxCommandEvent & event) {
    long mte;
    if (materialBox->GetValue().ToLong(&mte))
        wizard->config->mte = static_cast<long>(mte);
    else
        wxMessageBox("Matnam not valid");
}

wxSetExforCovPage::wxSetExforCovPage(wxWizard *parent) : wxWizardPageSimple(parent) {
    wizard = dynamic_cast<AddRunWizard*>(parent);
    mainSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
    
    mainSizer->Add(leftSizer, 1, wxEXPAND);
    mainSizer->Add(rightSizer, 1, wxEXPAND);
    exforCovList = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    rightSizer->Add(exforCovList, 1, wxEXPAND);
    viewIndexTableButton = new wxButton(this, wxID_ANY, "View Indexes");
    
    wxStaticText *instrText = new wxStaticText(this, wxID_ANY, 
    "This dialog allows you to add correlation "
    "coefficients across multiple experiments. To add a value,"
    "it is necessary to specify the absolute value, abs/rel covariance "
    "flag and starting/ending index in the full exfor vector. "
    "Press \"View Indexes\" to determine experiment indexes in the exfor vector.");
    
    instrText->Wrap(0.5 * 570);
    leftSizer->Add(
        instrText,
        1, // No stretching
        wxALIGN_LEFT,
        5 // Border
    );
    
    leftSizer->Add(viewIndexTableButton);

    AddCovButton = new wxButton(this, wxID_ANY, "Add cov. value");
    RemoveCovButton = new wxButton(this, wxID_ANY, "Remove cov. value");
    wxBoxSizer *addRemoveSizer = new wxBoxSizer(wxHORIZONTAL);
    addRemoveSizer->Add(AddCovButton);
    addRemoveSizer->Add(RemoveCovButton);
    leftSizer->Add(addRemoveSizer);
    Connect(viewIndexTableButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(wxSetExforCovPage::onViewIndexButtonClicked));
    Connect(AddCovButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(wxSetExforCovPage::onAddCovButtonClicked));
    Connect(RemoveCovButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(wxSetExforCovPage::onRemoveCovButtonClicked));
    Bind(wxEVT_WIZARD_BEFORE_PAGE_CHANGED, &wxSetExforCovPage::onFinishEvent, this);
    Connect(exforCovList->GetId(), wxEVT_LISTBOX_DCLICK,
        wxCommandEventHandler(wxSetExforCovPage::onListItemDoubleClicked));
    
    SetSizerAndFit(mainSizer);

    if (wizard->config->covf_data.size()) {
        for (auto cd : wizard->config->covf_data) {
            std::string set_string = std::to_string(cd.covfc);
            set_string += " from " + std::to_string(cd.range.first);
            set_string += " to " + std::to_string(cd.range.second);
            if (cd.irelfc)
                set_string += " Rel";
            else
                set_string += " Abs";

            exforCovList->Insert(set_string, 0);
        }
    }
}

void wxSetExforCovPage::onViewIndexButtonClicked(wxCommandEvent &event) {
    ExforIndexGridFrame *indexGrid = new ExforIndexGridFrame(this, index_array, title_array, energy_array);
    indexGrid->Show(true);
}

void wxSetExforCovPage::onAddCovButtonClicked(wxCommandEvent &event) {
    auto range = std::make_pair(1, index_array.back());
    additional_cov_data covData;
    CovDialog dialog("Add covariance", range, &covData);
    if (dialog.ShowModal() == wxID_OK) {
        wizard->config->covf_data.push_back(covData);
        std::string set_string = std::to_string(covData.covfc);
        set_string += " from " + std::to_string(covData.range.first);
        set_string += " to " + std::to_string(covData.range.second);
        if (covData.irelfc)
            set_string += " Rel";
        else
            set_string += " Abs";

        exforCovList->Insert(set_string, 0);
    }
}

void wxSetExforCovPage::onRemoveCovButtonClicked(wxCommandEvent &event) {
    auto selection = exforCovList->GetSelection();
    if (selection != wxNOT_FOUND) {
        exforCovList->Delete(selection);
        wizard->config->covf_data.erase(
            wizard->config->covf_data.begin() + selection);
    }
}

void wxSetExforCovPage::onListItemDoubleClicked(wxCommandEvent & event) {
    auto config_no = event.GetInt();
    additional_cov_data covData = wizard->config->covf_data.at(config_no);
    CovDialog dialog("Add covariance", covData.range, &covData);
    if (dialog.ShowModal() == wxID_OK) {
        wizard->config->covf_data.at(config_no) = covData;
        std::string set_string = std::to_string(covData.covfc);
        set_string += " from " + std::to_string(covData.range.first);
        set_string += " to " + std::to_string(covData.range.second);
        if (covData.irelfc)
            set_string += " Rel";
        else
            set_string += " Abs";

        exforCovList->SetString(config_no, set_string);
    }
}

void wxSetExforCovPage::onFinishEvent(wxWizardEvent & event) {
    wizard->finished = true;
}

wxAddStandardPage::wxAddStandardPage(wxWizard *parent) : wxWizardPageSimple(parent) {

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *exforBrowserSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *textEntryText = new wxStaticText(this, wxID_ANY,
        "Enter complete filename in normal unix style, such as"
        " \"/ usr / data / benchmark / new\", where integral or "
        "differential experimental data to be analyzed "
        " are stored in endf - 6 (file - 3 and file - 33) "
        " format ");
    wizard = dynamic_cast<AddRunWizard*>(parent);
    mainSizer->Add(textEntryText);
    textEntryText->Wrap(textEntryText->GetClientSize().x);
    mainSizer->Add(exforBrowserSizer);
    exforTextCtrl = new wxTextCtrl(this, wxID_ANY, wizard->config->pendfFileName);
    exforBrowserSizer->Add(exforTextCtrl);
    exforBrowseButton = new wxButton(this, wxID_ANY, "Browse");
    exforBrowserSizer->Add(exforBrowseButton);
    openFileDialog = new wxFileDialog(this);
    textEntryText->Wrap(textEntryText->GetClientSize().x);

    Connect(exforTextCtrl->GetId(), wxEVT_COMMAND_TEXT_ENTER,
        wxCommandEventHandler(wxAddStandardPage::onFileNameEntered));
    Connect(exforBrowseButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(wxAddStandardPage::onBrowseButtonClicked));
    Bind(wxEVT_WIZARD_BEFORE_PAGE_CHANGED, &wxAddStandardPage::onFinishEvent, this);
    
    SetSizerAndFit(mainSizer);
}

void wxAddStandardPage::onFinishEvent(wxWizardEvent & event) {
    if (!event.GetDirection())
        return;
    fileName = exforTextCtrl->GetValue();
    wizard->config->pendfFileName = fileName;
    if (wxFileExists(fileName)) {
        wizard->config->expdat = fileName;
        //TODO here read the file
    }
    else {
        wxMessageBox("No such file");
        event.Veto();
    }
}

void wxAddStandardPage::onFileNameEntered(wxCommandEvent & event) {
    fileName = exforTextCtrl->GetValue();
    if (wxFileExists(fileName)) {
        //Do some checks later
    }
}

void wxAddStandardPage::onBrowseButtonClicked(wxCommandEvent& event) {
    if (openFileDialog->ShowModal() == wxID_OK) {
        fileName = openFileDialog->GetPath();
        exforTextCtrl->ChangeValue(fileName);
    }
}

AddStandardExperimentsPage::AddStandardExperimentsPage(wxWizard *parent) : wxWizardPageSimple(parent) {
    wizard = dynamic_cast<AddRunWizard*>(parent);
    wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(leftSizer, 1, wxEXPAND);
    mainSizer->Add(rightSizer, 1, wxEXPAND);
    
    AddMaterialButton = new wxButton(this, wxID_ANY, "Add experiment");
    RemoveMaterialButton = new wxButton(this, wxID_ANY, "Remove experiment");

    wxStaticText *instructions = new wxStaticText(this, wxID_ANY, "Use the "
        "Add experiment and Remove experiment\n buttons to add new "
        "data to the evaluation or remove existing elemetns.");
    leftSizer->Add(instructions);
    leftSizer->Add(AddMaterialButton);
    leftSizer->Add(RemoveMaterialButton);

    experimentList = new wxListBox(this, wxID_ANY);
    rightSizer->Add(experimentList, 1, wxEXPAND);
    
    Connect(AddMaterialButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(AddStandardExperimentsPage::onAddExperimentButtonClicked));
    Connect(RemoveMaterialButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(AddStandardExperimentsPage::onRemoveExperimentButtonClicked));
    Connect(experimentList->GetId(), wxEVT_LISTBOX_DCLICK,
        wxCommandEventHandler(AddStandardExperimentsPage::onListItemDoubleClicked));
    Bind(wxEVT_WIZARD_FINISHED, &AddStandardExperimentsPage::onFinishEvent, this);

    SetSizerAndFit(mainSizer);
    instructions->Wrap(0.5 * wizard->wSize.GetWidth());

    if (wizard->config->exp_data.size()) {
        for (auto e : wizard->config->exp_data) {
            std::string set_string = "Experiment mt = " +
                std::to_string(e.mats);
            experimentList->Insert(set_string, 0);
        }
    }

}

void AddStandardExperimentsPage::onAddExperimentButtonClicked(wxCommandEvent & event) {
    standards_experiment temp_exp;
    AddMaterialWizard matwizard(this->GetParent(), wizard->config->expdat, wizard->config->imode, &temp_exp);
    matwizard.RunWizard(matwizard.GetFirstPage());
    
	if (matwizard.finished) {
        wizard->config->exp_data.push_back(temp_exp);
        //auto elements = wizard->config->exp_data.size();
        std::string set_string = "Experiment mt = " +
            std::to_string(wizard->config->exp_data.back().mats);
        experimentList->Insert(set_string, 0);
    }
}

void AddStandardExperimentsPage::onRemoveExperimentButtonClicked(wxCommandEvent & event) {
    auto selection = experimentList->GetSelection();
    if (selection != wxNOT_FOUND) {
        experimentList->Delete(selection);
        wizard->config->exp_data.erase(
            wizard->config->exp_data.begin() + selection);
    }
}

void AddStandardExperimentsPage::onFinishEvent(wxWizardEvent & event) {
    /* Let's say it is finished if we have defined at least 
    one experiment. It still needs to be determined if experiments 
    have to be added in imode 11 (mixed) */
    //wxMessageBox(std::to_string(wizard->config->exp_data.size()));
    if ((wizard->config->exp_data.size()) || (wizard->config->imoc == 11))
        wizard->finished = true;
}

void AddStandardExperimentsPage::onListItemDoubleClicked(wxCommandEvent & event) {
    auto config_no = event.GetInt();
    standards_experiment temp_exp = wizard->config->exp_data.at(config_no);
    AddMaterialWizard matwizard(this, wizard->config->expdat, wizard->config->imode, &temp_exp);
    matwizard.RunWizard(matwizard.GetFirstPage());
    if (matwizard.finished) {
        std::string set_string = "Experiment mt = " +
            std::to_string(temp_exp.mats);
        wizard->config->exp_data.at(config_no) = temp_exp;
        experimentList->SetString(config_no, set_string);
    }
}

wxAddIntegralPage::wxAddIntegralPage(wxWizard *parent) : wxWizardPageSimple(parent) {

	wizard = dynamic_cast<AddRunWizard*>(parent);
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *fileSizer = new wxBoxSizer(wxHORIZONTAL);
	Bind(wxEVT_WIZARD_FINISHED, &wxAddIntegralPage::onFinishEvent, this);
	wxString msg("Enter the path to a file with sensitivity data. "
		"The file can be prepared by selecting all values in 348 group structure "
		"in the DICE \"table\" view and pasting them in a text editor. "
	    "Check the box \"DICE FORMAT\" to bybass format conversion "
		"if sensitivities are already in DICE format");

	wxStaticText* instrText = new wxStaticText(this, wxID_ANY, msg);
	mainSizer->Add(
		instrText,
		1, // No stretching
		wxALIGN_LEFT,
		5 // Border
	);
	instrText->Wrap(instrText->GetClientSize().x);

	mainSizer->Add(fileSizer);
	
	IntegFileBox = new wxTextCtrl(this, wxID_ANY, fileName, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	FileBrowseButton = new wxButton(this, wxID_ANY, wxT("Browse"));
	fileSizer->Add(IntegFileBox);
	fileSizer->Add(FileBrowseButton);
	openFileDialog = new wxFileDialog(this);

	checklist = new wxCheckListBox(this, wxID_ANY);
	checklist->SetFont(wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	wxBoxSizer* bMeasSizer = new wxBoxSizer(wxHORIZONTAL);
	bMeasSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Measured - reference ")), 1, wxALL, 5);
	bmeasBox = new wxTextCtrl(this, wxID_ANY, std::to_string(wizard->config->bmeas));
	bMeasSizer->Add(bmeasBox);
	bMeasSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Deviation ")), 1, wxALL, 5);
	stdBox = new wxTextCtrl(this, wxID_ANY, std::to_string(wizard->config->std));
	bMeasSizer->Add(stdBox, wxLEFT);
	mainSizer->Add(bMeasSizer);

	wxBoxSizer* matnamSizer = new wxBoxSizer(wxHORIZONTAL);
	matnamBox = new wxTextCtrl(this, wxID_ANY, std::to_string(wizard->config->matnam));
	matnamSizer->Add(new wxStaticText(this, wxID_ANY, wxString("ZA")), 1, wxALL, 5);
	matnamSizer->Add(matnamBox);
	mainSizer->Add(matnamSizer);

	mainSizer->Add(checklist, 1, wxEXPAND);

	
	
	if (wizard->config->sData.sensitivities.size() > 0) {
		for (auto s : wizard->config->sData.sensitivities) {
			names.Add(s.name);
		}
		checklist->Set(names);
		for (auto i : wizard->config->checkedInts) {
			if (i < names.size()) {
				checklist->Check(i);
			}
		}
	}
	
	Connect(FileBrowseButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(wxAddIntegralPage::OpenFileButtonCLicked));
	Connect(IntegFileBox->GetId(), wxEVT_COMMAND_TEXT_ENTER,
		wxCommandEventHandler(wxAddIntegralPage::fileNameEntered));
	Connect(checklist->GetId(), wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,
		wxCommandEventHandler(wxAddIntegralPage::listboxToggled));
	Bind(wxEVT_WIZARD_BEFORE_PAGE_CHANGED, &wxAddIntegralPage::onFinishEvent, this);
    SetSizerAndFit(mainSizer);
}

void wxAddIntegralPage::listboxToggled(wxCommandEvent& event) {
	if (checklist->IsChecked(event.GetInt())) {
		std::string name = std::string(checklist->GetString(event.GetInt()).mb_str());
		if (fillData.za == 1) {
			fillData.za = wxAtoi(matnamBox->GetValue());
		}
		wizard->config->checkedInts.at(event.GetInt()) = 1;
		SensConfigDialog dialog(&wizard->config->intReacs[event.GetInt()], 
			name, &fillData);
		if (dialog.ShowModal() == wxID_OK) {
			names[event.GetInt()] = wxString(wizard->config->intReacs[event.GetInt()].rnam);
			wizard->config->sData.sensitivities[event.GetInt()].name = wxString(wizard->config->intReacs[event.GetInt()].rnam);
			checklist->Set(names);
			for (size_t i = 0; i < names.size(); i++) {
				if (wizard->config->checkedInts.at(i))
					checklist->Check(i);
			}
		}
		else {
			event.Skip();
		}
	} else {
		wizard->config->checkedInts.at(event.GetInt()) = 0;
	}
}

void wxAddIntegralPage::fileNameEntered(wxCommandEvent& event)
{
	fileName = IntegFileBox->GetValue();
	refreshSensitivities(fileName);
}

void wxAddIntegralPage::OpenFileButtonCLicked(wxCommandEvent& event)
{
	if (openFileDialog->ShowModal() == wxID_OK) {
		fileName = openFileDialog->GetPath();
		IntegFileBox->ChangeValue(fileName);

		refreshSensitivities(fileName);
	}
}

void wxAddIntegralPage::refreshSensitivities(std::string fileName) {
	if (wxFileExists(fileName)) {
		wizard->config->sData.sensitivities.clear();
		wizard->config->sData.readSens(fileName);
		for (auto s : wizard->config->sData.sensitivities) {
			names.Add(s.name);
		}
		checklist->Set(names);
		size_t size = wizard->config->sData.sensitivities.size();
		wizard->config->intReacs.clear();
		wizard->config->intReacs.assign(size, intReac());
		for (size_t i = 0; i < size; i++) {
			wizard->config->intReacs.at(i).rnam = std::string(names.Item(i).mb_str());
		}
		wizard->config->checkedInts.clear();
		wizard->config->checkedInts.assign(wizard->config->intReacs.size(), 0);
		for (int c : checkboxes)
			wizard->config->checkedInts.at(c) = 1;
	}
}

void wxAddIntegralPage::onFinishEvent(wxWizardEvent& event) {
	wizard->config->nreac = wizard->config->intReacs.size();
	checklist->GetCheckedItems(checkboxes);
	wizard->config->checkedInts.clear();
	wizard->config->checkedInts.assign(wizard->config->intReacs.size(), 0);
	for (int c : checkboxes)
		wizard->config->checkedInts.at(c) = 1;
	wizard->config->bmeas = wxAtof(bmeasBox->GetValue());
	wizard->config->std =   wxAtof(stdBox->GetValue());
	wizard->config->matnam = wxAtof(matnamBox->GetValue());

	if (wizard->config->nreac == 0)
		event.Veto();
	else {
		
		wizard->finished = true;
	}
		
}
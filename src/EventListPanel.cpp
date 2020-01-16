/***************************************************************
 * Name:      EvenetListPanel.cpp
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for EventListPanel class.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#include "EventListPanel.h"
#include "wx/dataview.h"


EventListPanel::EventListPanel(wxPanel *parent) : wxPanel(parent, wxID_ANY) {
    //ctor
    evList = new CalculationList(this, wxID_ANY);
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);
    hbox1->Add(evList, 1, wxEXPAND, 10);
    vbox->Add(hbox1, 1, wxEXPAND, 10);
    wxBoxSizer *hbox2 = new wxBoxSizer(wxHORIZONTAL);
    addRunButton = new wxButton(this, wxID_ANY, wxT("Add new fit"));
    initSetupButton = new wxButton(this, wxID_ANY, wxT("Init setup"));
    removeRunButton = new wxButton(this, wxID_ANY, wxT("Remove selected"));
    runRunButton = new wxButton(this, wxID_ANY, wxT("Run selected"));
    loadConfButton = new wxButton(this, wxID_ANY, wxT("Load conf"));
    saveConfButton = new wxButton(this, wxID_ANY, wxT("Save conf"));

    hbox2->Add(addRunButton, 0, 0);
    hbox2->Add(removeRunButton, 0, 0);
    hbox2->Add(initSetupButton, 0, 0);
    hbox2->Add(runRunButton, 0, 0);
    hbox2->Add(saveConfButton, 0, 0);
    hbox2->Add(loadConfButton, 0, 0);
    vbox->Add(hbox2);
    SetSizer(vbox);
    

    Connect(addRunButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(EventListPanel::AddButtonClick));
    Connect(removeRunButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(EventListPanel::RemoveButtonClick));
    Connect(initSetupButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(EventListPanel::InitButtonClick));
    Connect(runRunButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(EventListPanel::runRunButtonClick));
    Connect(saveConfButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(EventListPanel::saveButtonClick));
    Connect(loadConfButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(EventListPanel::loadButtonClick));
    Connect(evList->GetId(), wxEVT_LISTBOX_DCLICK,
        wxCommandEventHandler(EventListPanel::onListItemDoubleClicked));
}

void EventListPanel::AddButtonClick(wxCommandEvent& event) {
    GanrunConfiguration *config = new GanrunConfiguration();
    GanrunConfiguration *initConfig;
    bool del_init = 0;
    if (evList->runs.size() > 0)
        initConfig = &evList->runs.at(0);
    else {
        del_init = 1;
        initConfig = new GanrunConfiguration();
    }
    AddRunWizard wizard(this, config, initConfig);
    wizard.RunWizard(wizard.GetFirstPage());
    if (wizard.finished) {
        evList->appendRun(*config);
    }
    delete config;
    if (del_init)
        delete initConfig;
}

void EventListPanel::InitButtonClick(wxCommandEvent& event) {
    auto config = evList->getInitConfig();
    InitSetupWizard wizard(this, &config);
    wizard.RunWizard(wizard.GetFirstPage());
    if (wizard.finished)
        evList->updateFirstRun(config);
}

void EventListPanel::RemoveButtonClick(wxCommandEvent& event) {
    evList->deleteSelected();
}

void EventListPanel::runRunButtonClick(wxCommandEvent& event) {
    evList->runSelected();
}

void EventListPanel::saveButtonClick(wxCommandEvent& event) {
    evList->saveRuns();
}

void EventListPanel::loadButtonClick(wxCommandEvent& event) {
    evList->loadRuns();
}

void EventListPanel::onListItemDoubleClicked(wxCommandEvent &event) {
    auto config_no = event.GetInt();
    GanrunConfiguration config = evList->runs.at(config_no);
	if (config.label != "Setup run") {
		GanrunConfiguration *initConfig;
		if (evList->runs.size() > 0)
			initConfig = &evList->runs.at(0);
		else
			initConfig = new GanrunConfiguration();
		AddRunWizard wizard(this, &config, initConfig);
		wizard.RunWizard(wizard.GetFirstPage());
		if (wizard.finished) {
			evList->runs.at(config_no) = config;
			evList->SetString(config_no, config.label);
		}
	}
	else {
		auto config = evList->getInitConfig();
		InitSetupWizard wizard(this, &config);
		wizard.RunWizard(wizard.GetFirstPage());
		if (wizard.finished)
			evList->updateFirstRun(config);
	}
}

/***************************************************************
 * Name:      AddRunWizard.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Define a wizard that allows the user to add
 *            new GANDR runs.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#ifndef ADDRUNWIZARD_H
#define ADDRUNWIZARD_H

#include "wx/wx.h"
#include "wx/textctrl.h"
#include "wx/listctrl.h"
#include "wx/wizard.h"
#include "wx/sizer.h"
#include "wx/frame.h"
#include "wx/stattext.h"
#include "wx/radiobox.h"
#include "wx/filefn.h" 


#include "ExforReader.h"
#include "GanrunConfiguration.h"
#include "AddMaterialWizard.h"
#include "ExforCovDialog.h"
#include "ExforIndexGridDialog.h"
#include "IdefDialog.h"
#include "GlobalConfiguration.h"
#include "EditIntegSensDialog.h"

#pragma once

enum {
    M_STANDARDS,
    M_MIXED,
    M_CLASSIC,
	M_INTEGRAL,
    M_STANDARDS_NEXT
};

enum {
    M_PICK_LOCAL,
    M_ALL_LOCAL,
    M_REUSE
};

class AddRunWizard : public wxWizard {
public:
    AddRunWizard(wxWindow *parent, GanrunConfiguration *configuration, GanrunConfiguration *initConfig);

    wxWizardPage *GetFirstPage() const { return m_page1; }
    wxWizardPage *GetNextPage(short unsigned int page);
    void onFinishEvent(wxWizardEvent& event);
    GanrunConfiguration *config;
    GanrunConfiguration *initConf;
    bool finished = false;
    ExforReader *eReader;

    wxWizardPageSimple *m_page1;
    wxWizardPageSimple *m_exfor;
    wxWizardPageSimple *m_exfor_cov;
    wxWizardPageSimple *m_stand;
    wxWizardPageSimple *m_integ;
    wxWizardPageSimple *m_sexp;

    wxSize wSize = wxSize(570, 600);
};

class wxRunSelectPage : public wxWizardPageSimple {
public:
    AddRunWizard *wizard;
    wxRunSelectPage(wxWizard *parent);

    virtual wxWizardPage *GetNext() const override {
        return wizard->GetNextPage(m_radio->GetSelection());
    }

    virtual wxWizardPage *GetPrev() const override {
        return wizard->GetFirstPage();
    }


private:
    wxRadioBox *m_radio;
    wxWizardPageSimple m_exfor;
    void onFinishEvent(wxWizardEvent& event);
    wxWizardPage *m_next;
    wxTextCtrl *xnegTextCtrl;

    void onResize(wxSizeEvent& event) {
        Layout();
        event.Skip();
    }
};

class wxSetExforPage : public wxWizardPageSimple
{
public:
    wxSetExforPage(wxWizard *parent);
    void autofill_matnam_mte();
    void ExforBrowseButtonClick(wxCommandEvent& event);
    void radioBoxClicked(wxCommandEvent &event);
    void idefRadioBoxClicked(wxCommandEvent &event);


    void fileNameEntered(wxCommandEvent &event);
    void materialNameEntered(wxCommandEvent &event);
    void reactionNameEntered(wxCommandEvent &event);

    void onFinishEvent(wxWizardEvent &event);

    void advancedCovButtonClicked(wxCommandEvent &event);
    void selectAllButtonClicked(wxCommandEvent &event);

    void onListItemDoubleClicked(wxCommandEvent &event);
    void exforFromMatButtonClicked(wxCommandEvent &event);

    void readExperiments();
    void loadValues();
       
private:
    wxFileDialog *openFileDialog;
    wxTextCtrl *ExforFileBox;
    wxButton *FileBrowseButton;
    wxTextCtrl *materialBox;
    wxTextCtrl *reactionBox;
    wxRadioBox *ipickSelectBox;
    std::string fileName = "./92235.exfor";
    wxRadioBox *exforDefRadioBox;
    wxButton *selectAllButton;

    // idef controls
    wxRadioBox *idefSelectBox;
    wxButton *exforDefinitionButton;

    
    AddRunWizard *wizard;
    wxCheckListBox* checklist;
    wxButton *globalCovButton;

    wxButton *exforFromMatButton;
    bool exfor_from_zott = false;

};

class wxSetExforCovPage : public wxWizardPageSimple
{
public:
    wxSetExforCovPage(wxWizard *parent);
    void onViewIndexButtonClicked(wxCommandEvent & event);
    void onAddCovButtonClicked(wxCommandEvent & event);
    void onRemoveCovButtonClicked(wxCommandEvent & event);
    void onListItemDoubleClicked(wxCommandEvent &event);
    void onFinishEvent(wxWizardEvent &event);
    std::vector<int> index_array;
    std::vector<std::string> title_array;
    wxBoxSizer *mainSizer;

    /* In case of imode_11, we have to check if 
    maybe we need to add data in ENDF format too
    and maybe jump to "standards mode" */
    virtual wxWizardPage *GetNext() const override {
        if (imode == 12)
            return nullptr;
        else
            return wizard->GetNextPage(M_STANDARDS_NEXT);
    }

    virtual wxWizardPage *GetPrev() const override {
        return wizard->GetFirstPage();
    }
    
    int imode = 12;

private:   
    AddRunWizard *wizard;
    wxButton *AddCovButton;
    wxButton *RemoveCovButton;
    wxButton *viewIndexTableButton;
    wxGrid *indexGrid;
    wxListBox *exforCovList;

};

class wxAddStandardPage : public wxWizardPageSimple {
public:
    AddRunWizard *wizard;
    wxAddStandardPage(wxWizard *parent);

private:
    wxTextCtrl *exforTextCtrl;
    wxButton   *exforBrowseButton;
    wxFileDialog *openFileDialog;
    void onFinishEvent(wxWizardEvent& event);
    wxWizardPage *m_next;
    void onFileNameEntered(wxCommandEvent& event);
    void onBrowseButtonClicked(wxCommandEvent& event);
    std::string fileName;

    void onResize(wxSizeEvent& event) {
        Layout();
        event.Skip();
    }
};

class AddStandardExperimentsPage : public wxWizardPageSimple
{
public:
    AddStandardExperimentsPage(wxWizard *parent);

private:
    wxListBox *sExperimentList;
    AddRunWizard *wizard;
    wxButton *AddMaterialButton;
    wxButton *RemoveMaterialButton;
    wxListBox *experimentList;

    void onAddExperimentButtonClicked(wxCommandEvent& event);
    void onRemoveExperimentButtonClicked(wxCommandEvent& event);
    void onFinishEvent(wxWizardEvent& event);
    void onListItemDoubleClicked(wxCommandEvent &event);
};

class wxAddIntegralPage : public wxWizardPageSimple
{
public:
    wxAddIntegralPage(wxWizard *parent);
	void fileNameEntered(wxCommandEvent& event);
	void OpenFileButtonCLicked(wxCommandEvent& event);
	void refreshSensitivities(std::string fileName);
	void listboxToggled(wxCommandEvent& event);
	intReac fillData;

private:
	void onFinishEvent(wxWizardEvent& event);
	AddRunWizard* wizard;
	wxFileDialog* openFileDialog;
	wxTextCtrl* IntegFileBox;
	wxButton* FileBrowseButton;
	wxCheckListBox* checklist;
	wxTextCtrl* bmeasBox;
	wxTextCtrl* stdBox;
	wxTextCtrl* matnamBox;
	std::string fileName = "./dice_in.dat";
	wxArrayInt checkboxes;
	wxArrayString names;


};

#endif

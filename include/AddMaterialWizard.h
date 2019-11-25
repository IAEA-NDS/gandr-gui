/***************************************************************
 * Name:      AddMaterialWizard.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Define a wizard that allows the user to add
 *            experiments to the "STANDARDS run".
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#ifndef ADDMATWIZARD_H
#define ADDMATWIZARD_H
#pragma once


#include <wx/wx.h>
#include "wx/textctrl.h"
#include "wx/wizard.h"
#include "wx/sizer.h"
#include "wx/frame.h"
#include "wx/stattext.h"
#include "wx/radiobox.h"
#include "wx/grid.h"

#include "ExforReader.h"
#include "ENDFReader.h"
#include "GanrunConfiguration.h"
#include "AddCovDialog.h"
#include "AddSensDialog.h"
#include <wx/filefn.h> 

class AddMaterialWizard : public wxWizard {
public:
    AddMaterialWizard(wxWindow *parent, std::string filename, imodes imode, standards_experiment *experiment);
    ~AddMaterialWizard();
	
    wxWizardPage *GetFirstPage() const { return m_page1; }
    standards_experiment *experiment;
    bool finished = false;
    imodes imode;
    cs_data csData;
    std::string fileName;
    wxWizardPage *GetSecondPage(short unsigned int page);
    wxWizardPage *GetPrevPage();

    wxWizardPageSimple *m_page1;
    wxWizardPageSimple *m_conv;
    wxWizardPageSimple *m_integ1;
    wxWizardPageSimple *m_addsens;

    wxSize wSize = wxSize(570, 600);

    void redoDataGrid();
	
};

class StandardExpInitPage : public wxWizardPageSimple {
public:
    AddMaterialWizard *wizard;
    StandardExpInitPage(wxWizard *parent);

private:
    wxRadioBox *m_radio;
    wxTextCtrl *matnumTextCtrl;
    wxTextCtrl *mteTextCtrl;
    wxTextCtrl *gandMatnumTextCtrl;
    void onFinishEvent(wxWizardEvent& event);
    wxWizardPage *m_next;

    void onResize(wxSizeEvent& event) {
        Layout();
        event.Skip();
    }
};

class AddConvPage : public wxWizardPageSimple {
public:
    AddConvPage(wxWizard *parent);
    virtual wxWizardPage *GetNext() const override {
        return wizard->GetSecondPage(wizard->experiment->irm);
    }

    virtual wxWizardPage *GetPrev() const override {
        return wizard->GetFirstPage();
    }

private:
    wxListBox *sCovarianceList;
    AddMaterialWizard *wizard;
    wxButton *AddCovarianceButton;
    wxButton *RemoveCovarianceButton;
    void onAddCovarianceButtonClicked(wxCommandEvent& event);
    void loadValues();
    void onRemoveCovarianceButtonClicked(wxCommandEvent& event);
    void onListItemDoubleClicked(wxCommandEvent & event);
    void onFinishEvent(wxWizardEvent& event);


    void onResize(wxSizeEvent& event) {
        Layout();
        event.Skip();
    }
};

class DataPointsPage : public wxWizardPageSimple {
public:
    DataPointsPage(wxWizard *parent);
    wxGrid *dataGrid;

private:
    void onFinishEvent(wxWizardEvent& event);
    AddMaterialWizard *wizard;
    int nps;


    void onResize(wxSizeEvent& event) {
        Layout();
        event.Skip();
    }
};

class AdditionalSensPage : public wxWizardPageSimple {
public:
    AdditionalSensPage(wxWizard *parent);

private:
    AddMaterialWizard *wizard;
    wxButton *AddSensButton;
    wxButton *RemoveSensButton;
    wxListBox *sensListBox;
    void onAddSensButtonClicked(wxCommandEvent& event);
    void onRemoveSensButtonClicked(wxCommandEvent& event);
    void onFinishEvent(wxWizardEvent& event);
    void loadValues();
    void onListItemDoubleClicked(wxCommandEvent & event);

    virtual wxWizardPage *GetPrev() const override {
        return wizard->GetPrevPage();
    }


    void onResize(wxSizeEvent& event) {
        Layout();
        event.Skip();
    }
};

void setGridRowNumbers(wxGrid * grid, int no_rows);

#endif
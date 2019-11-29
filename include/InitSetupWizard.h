/***************************************************************
 * Name:      InitSetupWizard.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Define IinitSetupWizard class.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#ifndef INITSETUPWIZARD_H
#define INITSETUPWIZARD_H

#include <wx/wx.h>
#include "wx/textctrl.h"
#include "wx/wizard.h"
#include "wx/sizer.h"
#include "wx/frame.h"
#include "wx/stattext.h"
#include "wx/radiobox.h"
#include "wx/filefn.h"
#include <fstream>

#include "ExforReader.h"
#include "GanrunConfiguration.h"
#include "GridEditPanel.h"
#include "EditGridWindow.h"
#include "GlobalConfiguration.h"
 //#include "csv.h"


class InitSetupWizard : public wxWizard {
public:
    InitSetupWizard(wxWindow *parent, GanrunConfiguration *config);
    void UpdateReactions();
    ~InitSetupWizard();
    wxWizardPage *GetFirstPage() const { return m_page1; };
    void onFinishEvent(wxWizardEvent& event);
    GanrunConfiguration *config;
    wxWizardPage *GetNextPage(short unsigned int page);
    wxWizardPage * GetPrevPage();

    wxString threshMatName;
    bool finished = false;

private:
    wxWizardPageSimple *m_page1;
    wxWizardPageSimple *m_reactions;
    wxWizardPageSimple *m_gridEdit;
};

class wxInitFilesPage : public wxWizardPageSimple
{
public:

    wxInitFilesPage(wxWizard *parent);
    InitSetupWizard *wizard;

    void PENDFCustomButtonClick(wxCommandEvent& event);
    void PENDFBrowseButtonClick(wxCommandEvent& event);
    void PENDFFileNameEntered(wxCommandEvent& event);
    void COVCustomButtonClick(wxCommandEvent& event);
    void COVBrowseButtonClick(wxCommandEvent& event);
    void COVFileNameEntered(wxCommandEvent& event);


private:
    wxFileDialog *openFileDialog;

    wxTextCtrl *PENDFFileBox;
    wxTextCtrl *COVFileBox;

    wxButton *CustomButtonPENDF;
    wxButton *CustomButtonCOV;
    wxRadioButton *ReacSetupBox[2];
    wxTextCtrl *matReacBox;

    wxButton *FileBrowseButtonPENDF;
    wxButton *FileBrowseButtonCOV;
    void onNextEvent(wxWizardEvent& event);

    std::string PENDFFileName = "./92235.pendf";
    std::string COVFileName = "./92235.cov";

    virtual wxWizardPage *GetNext() const override {
        int reacSelection = 0;
        if (ReacSetupBox[1]->GetValue() == 1)
            reacSelection = 1;
        return wizard->GetNextPage(reacSelection);
    }

};

class ScrolledWidgetsPane : public wxScrolledWindow
{
public:
    ScrolledWidgetsPane(wxWindow* parent, wxWindowID id, std::array<reaction, NRMAX> *reactions);

    wxFlexGridSizer *gs;
    wxWizardPageSimple *page;
    std::vector<wxTextCtrl*> nameBoxes;
    std::vector<wxTextCtrl*> mtBoxes;
    std::vector<wxTextCtrl*> descBoxes;
    std::vector<wxStaticText*> gandrNumbers;
};


class wxSetReactionsPage : public wxWizardPageSimple
{
public:

    wxSetReactionsPage(wxWizard *parent);
    void update_reactions();
    InitSetupWizard *wizard;
    std::array<reaction, NRMAX> reactions;
    ScrolledWidgetsPane *reacPane;

    ~wxSetReactionsPage();

private:
    wxFileDialog *openFileDialog;
    int toTotal[25] = { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                        1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1 };
    wxTextCtrl *COVFileBox;
    std::string threshLocation = "../test_files/92233.thresh";

    

    void onFinishEvent(wxWizardEvent& event);

    void reactionsFromBoxes();

};

class wxGridEditPage : public wxWizardPageSimple
{
public:

    wxGridEditPage(wxWizard *parent);
	void listboxToggled(wxCommandEvent& event);
    InitSetupWizard *wizard;
    ~wxGridEditPage();
    wxRadioBox *gridRadioBox;
    wxRadioBox *thinRadioBox;
    wxTextCtrl *thinValue;
    void onRadioBox(wxCommandEvent &event);
    void onFinishEvent(wxWizardEvent & event);

    virtual wxWizardPage *GetPrev() const override {
        return wizard->GetPrevPage();
    }

};



#endif
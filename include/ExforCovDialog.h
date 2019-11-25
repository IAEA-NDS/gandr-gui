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

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/radiobut.h>
#include "GanrunConfiguration.h"

class ExforCovDialog : public wxDialog
{
public:
    ExforCovDialog(const wxString& title, int exp_no, int data_len, exfor_cov_data *data, int *thinning);
    exfor_cov_data *data;
    ~ExforCovDialog();
private:
    int expno, data_len;
    int *thinning;
    wxRadioButton *diagButtons[3];
    wxTextCtrl *sigmaTextBox;
    wxTextCtrl *bandTextBox;

    wxRadioButton *covminButtons[3];
    wxTextCtrl *covminTextBox;
    wxTextCtrl *varianceTextBox;
    wxTextCtrl *thinningTextBox;


    wxRadioBox *absRelRadioBox;
    wxRadioBox *enableDisableRadioBox;

    wxGrid *sysFuncGrid;

    void createGeneralControl();
    void createFunctionControl();

    void loadData();
    void setData();

    wxButton *confirmButton;

    wxBoxSizer *mainSizer;
    wxBoxSizer *leftSizer;
    wxBoxSizer *rightSizer;

    void onConfirm(wxCommandEvent& event);

    void OnRadioBoxClickedDiag(wxCommandEvent& event);

    void OnRadioBoxClickedCovmin(wxCommandEvent& event);
    void OnRadioBoxClickedRelAbs(wxCommandEvent& event);
    void OnRadioBoxClickedRelFunc(wxCommandEvent& event);
};
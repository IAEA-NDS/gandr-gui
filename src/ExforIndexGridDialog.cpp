/***************************************************************
 * Name:      ExforIndexGridDialog.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for custom dialog used to add correlations 
 *			  when fitting differential data from exfor.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-11-7
 * License:   GPL-3
 **************************************************************/

#include "ExforIndexGridDialog.h"


ExforIndexGridFrame::ExforIndexGridFrame(wxWindow *parent, std::vector<int> &index_array, std::vector<std::string> &titles_array)
    :wxDialog(parent, wxID_ANY, "Title") {
    
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
  
    mainSizer = new wxBoxSizer(wxVERTICAL);
    int no_sel_exp = index_array.size();
    grid = new wxGrid(this, wxID_ANY);
    grid->CreateGrid(no_sel_exp-1, 3);
    if (no_sel_exp) {
        for (size_t i = 1; i < no_sel_exp; i++) {
            grid->SetCellValue(i - 1, 0, titles_array.at(i - 1).c_str());
            grid->SetCellValue(i - 1, 1, std::to_string(index_array.at(i - 1) + 1).c_str());
            grid->SetCellValue(i - 1, 2, std::to_string(index_array.at(i)).c_str());
        }
    }

    grid->HideRowLabels();
    mainSizer->Add(grid, 1, wxALL | wxEXPAND);
    SetSizerAndFit(mainSizer);
    SetSize(wxSize(600, 400));
    auto titleSize = GetClientSize().GetWidth() - grid->GetColSize(1) - grid->GetColSize(2);
    grid->SetColSize(0, titleSize);
    grid->SetColLabelValue(0, "Experiment");
    grid->SetColLabelValue(1, "Start index");
    grid->SetColLabelValue(2, "End index");

}

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


ExforIndexGridFrame::ExforIndexGridFrame(wxWindow *parent, std::vector<int> &index_array, std::vector<std::string> &titles_array, std::vector<std::pair<double, double>> &energies)
    :wxDialog(parent, wxID_ANY, "Title") {
    
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
  
    mainSizer = new wxBoxSizer(wxVERTICAL);
    int no_sel_exp = index_array.size();
    grid = new wxGrid(this, wxID_ANY);
    grid->CreateGrid(no_sel_exp-1, 5);
	std::ostringstream s1, sn;
	s1.precision(4); sn.precision(4);
	s1 << std::scientific; sn << std::scientific;
    if (no_sel_exp) {
        for (size_t i = 1; i < no_sel_exp; i++) {
			s1.str(""); sn.str("");
			s1.clear(); sn.clear();
			s1 << energies.at(i-1).first;
			sn << energies.at(i-1).second;
            grid->SetCellValue(i - 1, 0, titles_array.at(i - 1).c_str());
            grid->SetCellValue(i - 1, 1, std::to_string(index_array.at(i - 1) + 1).c_str());
            grid->SetCellValue(i - 1, 2, std::to_string(index_array.at(i)).c_str());
			grid->SetCellValue(i - 1, 3, std::string(s1.str()));
			grid->SetCellValue(i - 1, 4, std::string(sn.str()));
        }
    }

    grid->HideRowLabels();
    mainSizer->Add(grid, 1, wxALL | wxEXPAND);
    SetSizerAndFit(mainSizer);
    SetSize(wxSize(600, 400));
    auto titleSize = GetClientSize().GetWidth() - grid->GetColSize(1) - 
		grid->GetColSize(2) - grid->GetColSize(3) - grid->GetColSize(4);
    grid->SetColSize(0, titleSize);
    grid->SetColLabelValue(0, "Experiment");
    grid->SetColLabelValue(1, "Start index");
    grid->SetColLabelValue(2, "End index");
	grid->SetColLabelValue(3, "E1");
	grid->SetColLabelValue(4, "En");
}

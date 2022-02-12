/***************************************************************
 * Name:      EventListCtrl.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for CalculationList class, derived from
 *            wxCheckListBox class.
 *            CalculationList is a custom checkbox that allows
 *            the user to manage the defined GANDR runs.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#include "EventListCtrl.h"
#include "GlobalConfiguration.h"

CalculationList::CalculationList(wxWindow *parent,
    const wxWindowID id,
    const wxPoint& pos,
    const wxSize& size,
    long style) : wxCheckListBox(parent, id, pos, size, style) {
}

CalculationList::CalculationList(wxWindow *parent, const wxWindowID id)
    :wxCheckListBox(parent, id) {
}

CalculationList::~CalculationList() {
}

void CalculationList::updateFirstRun(GanrunConfiguration run) {
    auto setting_string = wxString("Setup run");

    if (runs.size()) {
        if (runs.front().label == "Setup run") {
            runs.at(0) = run;
        }
        else {
            runs.insert(runs.begin(), run);
            InsertItems(1, &setting_string, 0);
        }
    }
    else {
        runs.push_back(run);
        AppendString("Setup run");
    }
    Check(0, true);
}

GanrunConfiguration CalculationList::getInitConfig() {
    if (runs.size()) {
        if (runs.front().label == "Setup run")
            return runs.at(0);
    }
    GanrunConfiguration config = GanrunConfiguration();
    return config;
}

void CalculationList::appendRun(GanrunConfiguration run) {
    runs.push_back(run);
    AppendString(run.label);
    Check(runs.size() - 1, true);
}

void CalculationList::removeRun(int run_no) {
    if (runs.size() > run_no) {
        runs.erase(runs.begin() + run_no);
        Delete(run_no);
    }
}

bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

void CalculationList::saveRuns() {
    wxFileDialog
        saveFileDialog(this, _("Save configuration"), "", "",
            "Save files (*.sav)|*.sav", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...
    if (ends_with(std::string(saveFileDialog.GetPath()), ".sav")){
        std::ofstream os(std::string(saveFileDialog.GetPath()), std::ios::binary);
        cereal::BinaryOutputArchive oarchive(os);
        oarchive(runs);
        os.close();
    } else {
        std::ofstream os(std::string(saveFileDialog.GetPath() + ".sav"), std::ios::binary);
        cereal::BinaryOutputArchive oarchive(os);
        oarchive(runs);
        os.close();
    }
}

void CalculationList::loadRuns() {
    wxFileDialog
        loadFileDialog(this, _("Load configuration"), "", "",
            "Cereal files (*.sav)|*.sav", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (loadFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...
    int run_size = runs.size();
    for (int i = 0; i < run_size; i++) {
        removeRun(0);
    }
    std::ifstream is(std::string(loadFileDialog.GetPath()), std::ios::binary);
    cereal::BinaryInputArchive iarchive(is);
    try
    {
        iarchive(runs);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        wxMessageBox("Could not load file");
        is.close();
        return;
    }
    
    is.close();

    for (auto run : runs) {
        AppendString(run.label);
    }
}

void CalculationList::runSelected() {
    wxArrayInt  checked;
    GetCheckedItems(checked);
    auto outFile = std::ofstream("run.k");
	size_t index = 0, totalCounter = 0;
    for (auto c : checked) {
		runs.at(c).inopt = index;
		if (runs.at(c).label != "Setup run") {
			index++;
			if (index == 1 && totalCounter == 1) {
				runs.at(c).ipend = runs.at(checked.at(0)).ipend;
				runs.at(c).imoc = runs.at(checked.at(0)).imoc;
			}
			if (runs.at(c).imode == INTEGRAL) {
				std::string fn = "dicer_" + std::to_string(runs.at(c).inopt) + ".dat";
				if (wxFileExists(fn) == true) {
					wxMessageDialog dialog(this,
						"File " + fn + " exist, copy to " + fn + ".b",
						"file exists", wxYES_NO);
					if (dialog.ShowModal() == wxID_YES) {
						wxRenameFile(fn, fn + ".b", true);
						runs.at(c).sData.writeSens(fn, &runs.at(c).checkedInts);
					}
				}
				else {
					runs.at(c).sData.writeSens(fn, &runs.at(c).checkedInts);
				}

			}
		}
		totalCounter++;

		// Actually generate the input
		runs.at(c).generate_input();
		outFile << runs.at(c).input_string;
    }
    outFile.close();
    
    Singleton* s = Singleton::getInstance();
    if (s->runGandr) {

        wxShell("chmod +x run.k && ./run.k");
    }
}

void CalculationList::deleteSelected() {
    wxArrayInt  checked;
    GetCheckedItems(checked);
    Freeze();
    for (std::size_t i = 0; i < checked.size(); i++) {
        removeRun(checked[i] - i);
    }
    Thaw();
}
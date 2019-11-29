/***************************************************************
 * Name:      EventListCtrl.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Define CalculationList class, derived from
 *            wxCheckListBox class.
 *            CalculationList is a custom checkbox that allows
 *            the user to manage the defined GANDR runs.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#ifndef EVENTLIST_H
#define EVENTLIST_H

#include "AddRunWizard.h"
#include <wx/checklst.h>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

#include <fstream>

class CalculationList : public wxCheckListBox
{
public:
    CalculationList(wxWindow *parent, const wxWindowID id);
    CalculationList(wxWindow *parent,
        const wxWindowID id,
        const wxPoint& pos,
        const wxSize& size,
        long style);

    virtual ~CalculationList();
    std::vector<GanrunConfiguration> runs;

    void updateFirstRun(GanrunConfiguration run);
    GanrunConfiguration getInitConfig();
    void appendRun(GanrunConfiguration run);
    void removeRun(int run_no);
    void saveRuns();
    void loadRuns();

    void runSelected();
    void deleteSelected();

private:
};

#endif // EVENTLIST

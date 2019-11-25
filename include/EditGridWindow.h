/***************************************************************
 * Name:      EditGridWindow.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Define a custom dialog used to edit grid elemetns.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#include <wx/wx.h>
#include "GanrunConfiguration.h"
#include "GridEditPanel.h"

class GridEditDialog : public wxDialog
{
public:
    GridEditDialog(const wxString& title, std::array<int, GRID_LEN> *iflex);
    ~GridEditDialog();
    GridEditPanel *gridPanel;
    wxButton *confirmButton;

    std::array<int, GRID_LEN> *iflex;

    void onConfirm(wxCommandEvent& event);


};
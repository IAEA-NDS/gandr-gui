#include "EditGridWindow.h"
/***************************************************************
 * Name:      EditGridWindow.cpp
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for a custom dialog used to edit GANDR grid
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

enum {
    M_ABSOLUTE,
    M_RELATIVE
};

GridEditDialog::GridEditDialog(const wxString & title, std::array<int, GRID_LEN> *iflex)
    :wxDialog(NULL, wxID_ANY, "Covariance data", wxDefaultPosition, wxSize(200, 200),
        wxRESIZE_BORDER | wxDEFAULT_DIALOG_STYLE | wxMAXIMIZE), iflex(iflex) {
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *bottomSizer = new wxBoxSizer(wxVERTICAL);

    this->Maximize(true);
    mainSizer->Add(topSizer, 1, wxEXPAND);
    gridPanel = new GridEditPanel(this, *iflex);
    topSizer->Add(gridPanel, 1, wxEXPAND);
    mainSizer->Add(bottomSizer, 1, wxEXPAND);
    confirmButton = new wxButton(this, wxID_OK, "Confirm");
    // The window seems to overflow the screen size, so 
    // the button is lost when placed on the bottom.
    // TODO: Investigate and fix
    //bottomSizer->Add(0, 1, wxEXPAND);
    bottomSizer->Add(confirmButton, 0, wxALIGN_RIGHT);

    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(GridEditDialog::onConfirm));

    SetSizer(mainSizer);

}

GridEditDialog::~GridEditDialog() {
}

void GridEditDialog::onConfirm(wxCommandEvent & event) {
    wxDialog::EndModal(wxID_OK);
}

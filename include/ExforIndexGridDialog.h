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

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/radiobut.h>
#include "GanrunConfiguration.h"

class ExforIndexGridFrame : public wxDialog
{
public:
    ExforIndexGridFrame(wxWindow *parent, std::vector<int> &index_array, 
        std::vector<std::string> &titles_array, std::vector<std::pair<double, double>>& energies);
private:
    wxGrid *grid;

};
/***************************************************************
 * Name:      GridEditPanel.cpp
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for the GridEditPanel class.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#include "GridEditPanel.h"


GridEditPanel::GridEditPanel(wxWindow * parent, std::array<int, GRID_LEN> iflex) : wxPanel(parent, wxID_ANY, 
    wxDefaultPosition, wxDefaultSize, wxTRANSPARENT_WINDOW), parent(parent), iflex(iflex) {

    lw = GetSize().GetWidth()*0.95;

    for (int i = 0; i < GRID_LEN; i++) {
        auto pos = etox(gandr_grid[i], wxPoint(4, 4));
        gButtons[i] = new gridButton(this, "", pos, 700+i);
        Connect(700 + i, GRID_BUTTON_EVENT,
            wxCommandEventHandler(GridEditPanel::onButtonPressed));
        Connect(700 + i, GRID_BUTTON_RCLICK_EVENT,
            wxCommandEventHandler(GridEditPanel::onButtonRclick));
    }
}

void GridEditPanel::paintEvent(wxPaintEvent & evt) {
    wxPaintDC dc(this);
    render(dc);
}

void GridEditPanel::paintNow() {
    wxClientDC dc(this);
    render(dc);
}

// Rendering of the graphics, the entire window has to be redrawable at any given time
void GridEditPanel::render(wxDC&  dc) {

    lw = GetClientSize().GetWidth()*0.95;
    
    dc.SetPen(wxPen(wxColor(0, 0, 0), 3)); // black line, 3 pixels thick
    lines = ((GetClientSize().GetHeight() - sep) / sep);

    for (int i = 0; i < GRID_LEN; i++) {
        auto pos = etox(gandr_grid[i], wxPoint(4, 4));
        gButtons[i]->Move(pos);
    }

    // Horizontal lines
    for (int i = 0; i < lines; i++) {
        dc.DrawLine(side_padding, sep + sep*i,  side_padding + lw, sep + sep * i); // draw line across the rectangle
    }

    // Text for energy value
    for (int i = 0; i < GRID_LEN; i++) {
        dc.DrawText(sci_str(gandr_grid[i]), etox(gandr_grid[i], wxPoint(20, -10)));
    }

    // This draws the points and labels for intermediate points
    for (int i = 0; i < GRID_LEN; i++) {
		if (iflex[i] <= 0) {
			dc.SetBrush(*wxWHITE_BRUSH);
            gButtons[i]->Enable(false);
			gButtons[i]->setZero(true);
		}
        else if (iflex[i] > 1) {
            dc.SetBrush(*wxBLUE_BRUSH);
            for (int j = 1; j < iflex[i]; j++) {
                double x_dif = (double)j *(log10(gandr_grid[i + 1]) - log10(gandr_grid[i])) / (double)iflex[i];
                auto midPoint = etox(pow(10, log10(gandr_grid[i]) + x_dif), wxPoint(4, 4));
                midPoint.x += 5;
                dc.DrawCircle(midPoint, 3);
            }
            double x_mid = pow(10, 0.5 * (log10(gandr_grid[i]) + log10(gandr_grid[i + 1])));
            auto textPoint = etox(x_mid, wxPoint(0, 30));
            dc.SetBrush(*wxBLACK_BRUSH);
            dc.DrawText("+" + std::to_string(iflex[i]-1), textPoint);
        }
    }
}

wxPoint GridEditPanel::etox(double energy, wxPoint offset = wxPoint(0, 0)) {
    // Numerical closed interval for upper point, I know it is bad,
    // will fix later
    double emax = gandr_grid[GRID_LEN-1] + 1e-5, emin=gandr_grid[0];
    double xscale = 100.;
    double relpos = (log10(energy) - log10(emin)) / (log10(emax) - log10(emin));
    double totalx = lw * lines;
    int line = (int) std::floor(relpos * lines);
    double x = side_padding + std::fmod(relpos * totalx, lw) - offset.x;
    double y = sep + line * sep - offset.y;
    return wxPoint(x, y);
}

void GridEditPanel::buttonProc(int button, int action) {
    // Check for limits, the last button does nothing
    if (button >= GRID_LEN || button < 0)
        return;
    if (action > 0 && iflex[button] < 10) {
        iflex[button]++;
        gButtons[button]->setZero(false);
        gButtons[button]->paintNow();
    }
    else if (action == -1) {
        gButtons[button]->setZero(true);
        iflex[button] = 0;
        gButtons[button]->paintNow();
        //Refresh();
    }

    Refresh();
    Update();
}

void GridEditPanel::onButtonPressed(wxCommandEvent &event) {
    buttonProc(event.GetId() % 100, 1);
}

void GridEditPanel::onButtonRclick(wxCommandEvent & event) {
    buttonProc(event.GetId() % 100, -1);
}

BEGIN_EVENT_TABLE(GridEditPanel, wxPanel)

 // catch paint events
    EVT_PAINT(GridEditPanel::paintEvent)

    END_EVENT_TABLE()

std::string sci_str(double num, int precision) {
    std::stringstream ss;
    ss << std::setprecision(precision) << std::scientific << num;
    return ss.str();

}
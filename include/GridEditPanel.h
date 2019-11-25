/***************************************************************
 * Name:      GridEditPanel.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Headers for the GridEditPanel class.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#ifndef GRIDEDITPANEL_H
#define GRIDEDITPANEL_H

#include "wx/wx.h"
#include "wx/sizer.h"
#include "GridButton.h"
#include "GandrGrid.h"

#include <iomanip>
#include <string> 
#include <iostream>
#include <sstream>
#include <algorithm>
#include <array>

#define GRID_LEN 74

class GridEditPanel : public wxPanel
{

public:
    GridEditPanel(wxWindow* parent, std::array<int, GRID_LEN> iflex);

    void paintEvent(wxPaintEvent & evt);
    void paintNow();

    void render(wxDC& dc);
    wxBitmap gridButtonBitmap;
    wxButton *bpmgridButton;
    gridButton *gButtons[GRID_LEN];

    wxPoint etox(double energy, wxPoint offset);
    
    // Geomentry constants, will probably have to 
    // recalculate some on the fly
    int lines = 12.;
    // Space between lines
    double sep = 80.;
    // Line width
    double lw = 500;
    // left padding
    double side_padding = 40;
    //std::array<int, GRID_LEN> iflex = { 0 };
    std::array<int, GRID_LEN> iflex;

    void buttonProc(int button, int action);

    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */
    void onButtonPressed(wxCommandEvent &event);
    void onButtonRclick(wxCommandEvent &event);


    DECLARE_EVENT_TABLE()
private:
    wxWindow *parent;

};

std::string sci_str(double num, int precision = 2);

#endif


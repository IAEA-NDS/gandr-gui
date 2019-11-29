/***************************************************************
 * Name:      GridButton.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Headers for the GridButton class.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#ifndef GRIDBUTTON_H
#define GRIDBUTTON_H



#include <wx/wx.h>
#include <wx/sizer.h>

wxDECLARE_EVENT(GRID_BUTTON_EVENT, wxCommandEvent);
wxDECLARE_EVENT(GRID_BUTTON_RCLICK_EVENT, wxCommandEvent);


class gridButton : public wxWindow
{

    bool pressedDown;
    wxString text;

    static const int buttonWidth  = 4;
    static const int buttonHeight = 4;
    bool zero = false;

public:
    gridButton(wxWindow* parent, wxString text, wxPoint position=wxDefaultPosition, wxWindowID=wxID_ANY);

    void paintEvent(wxPaintEvent & evt);
    void paintNow();

    void render(wxDC& dc);

    // some useful events
    void mouseMoved(wxMouseEvent& event);
    void mouseDown(wxMouseEvent& event);
    void mouseWheelMoved(wxMouseEvent& event);
    void mouseReleased(wxMouseEvent& event);
    void rightClick(wxMouseEvent& event);
    void mouseLeftWindow(wxMouseEvent& event);
    void keyPressed(wxKeyEvent& event);
    void keyReleased(wxKeyEvent& event);
    void setZero(bool choice);

//private:
    //GridEditPanel *panel;

    DECLARE_EVENT_TABLE()
};

#endif // GRIDBUTTON_H